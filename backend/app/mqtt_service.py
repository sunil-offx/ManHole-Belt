import json
import logging
import asyncio
import threading
import datetime
import paho.mqtt.client as mqtt
from .config import settings
from .database import SessionLocal, TelemetryLog, DeviceRecord
from .websocket_hub import ws_hub
from .alert_service import alert_service

logger = logging.getLogger("sentinel.mqtt")

class MQTTService:
    def __init__(self):
        self.client = mqtt.Client(client_id=settings.MQTT_CLIENT_ID)
        self.client.on_connect = self.on_connect
        self.client.on_message = self.on_message
        self.client.on_disconnect = self.on_disconnect
        self.connected = False
        self.loop_thread = None

    def on_connect(self, client, userdata, flags, rc):
        if rc == 0:
            self.connected = True
            logger.info("Connected to MQTT Broker successfully.")
            client.subscribe("sentinel/+/telemetry")
            client.subscribe("sentinel/+/alert")
            logger.info("Subscribed to sentinel/+/telemetry and sentinel/+/alert")
        else:
            self.connected = False
            logger.warning(f"Failed to connect to MQTT broker with rc={rc}")

    def on_disconnect(self, client, userdata, rc):
        self.connected = False
        logger.warning(f"Disconnected from MQTT broker (rc={rc})")

    def on_message(self, client, userdata, msg):
        try:
            topic = msg.topic
            payload_str = msg.payload.decode("utf-8")
            data = json.loads(payload_str)
            logger.debug(f"MQTT message on {topic}: {payload_str[:80]}...")

            if topic.endswith("/telemetry"):
                self.handle_telemetry(data)
            elif topic.endswith("/alert"):
                self.handle_alert(data)
        except Exception as e:
            logger.error(f"Error processing MQTT message: {e}")

    def handle_telemetry(self, data: dict):
        device_id = data.get("device_id", "SB-001")
        sensors = data.get("sensors", {})
        risk_score = data.get("risk_score", 0.0)
        status = data.get("status", "SAFE")
        anomaly = data.get("anomaly", False)
        fall_detected = data.get("fall_detected", False)

        db = SessionLocal()
        try:
            # 1. Update device state
            dev = db.query(DeviceRecord).filter(DeviceRecord.device_id == device_id).first()
            if dev:
                dev.status = status
                dev.last_seen = datetime.datetime.utcnow()
            
            # 2. Insert telemetry log
            log_entry = TelemetryLog(
                device_id=device_id,
                o2=sensors.get("O2", 20.9),
                h2s=sensors.get("H2S", 0.0),
                co=sensors.get("CO", 0.0),
                ch4=sensors.get("CH4", 0.0),
                temp=sensors.get("temp", 25.0),
                hum=sensors.get("hum", 60.0),
                water_level=sensors.get("water_level", 0.0),
                ax=sensors.get("ax", 0.0),
                ay=sensors.get("ay", 0.0),
                az=sensors.get("az", 1.0),
                risk_score=risk_score,
                status=status,
                anomaly=anomaly,
                fall_detected=fall_detected
            )
            db.add(log_entry)
            db.commit()
        finally:
            db.close()

        # 3. Broadcast to all active Supervisor WebSocket clients
        try:
            loop = asyncio.get_event_loop()
            if loop.is_running():
                asyncio.run_coroutine_threadsafe(
                    ws_hub.broadcast({
                        "type": "telemetry",
                        "data": data
                    }),
                    loop
                )
        except Exception as ws_err:
            logger.debug(f"WS Broadcast notice: {ws_err}")

    def handle_alert(self, data: dict):
        device_id = data.get("device_id", "SB-001")
        alert_type = data.get("alert_type", "HAZARD")
        severity = data.get("severity", "CRITICAL")
        message = data.get("message", "Immediate attention required")

        alert_info = alert_service.process_and_dispatch(device_id, alert_type, severity, message)

        try:
            loop = asyncio.get_event_loop()
            if loop.is_running():
                asyncio.run_coroutine_threadsafe(
                    ws_hub.broadcast({
                        "type": "alert",
                        "data": alert_info
                    }),
                    loop
                )
        except Exception:
            pass

    def publish_config(self, device_id: str, config_dict: dict):
        topic = f"sentinel/{device_id}/config"
        payload = json.dumps(config_dict)
        if self.connected:
            self.client.publish(topic, payload, qos=1)
            logger.info(f"Published remote config to {topic}")
        else:
            logger.warning(f"MQTT client offline. Cannot publish config to {topic}")

    def start(self):
        def _connect_worker():
            try:
                self.client.connect(settings.MQTT_BROKER_HOST, settings.MQTT_BROKER_PORT, settings.MQTT_KEEPALIVE)
                self.client.loop_forever()
            except Exception as e:
                logger.warning(f"MQTT connect exception (is Mosquitto running?): {e}")

        self.loop_thread = threading.Thread(target=_connect_worker, daemon=True)
        self.loop_thread.start()

    def stop(self):
        if self.connected:
            self.client.disconnect()

mqtt_service = MQTTService()
