import datetime
from fastapi import APIRouter, Depends, HTTPException, Query
from sqlalchemy.orm import Session
from typing import List, Optional

from .database import SessionLocal, DeviceRecord, TelemetryLog, AlertLog, DeviceConfig
from .models import TelemetryPayload, AlertPayload, DeviceConfigUpdate, AlertAcknowledge
from .mqtt_service import mqtt_service
from .alert_service import alert_service
from .websocket_hub import ws_hub

router = APIRouter()

def get_db():
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()

@router.get("/devices")
def list_devices(db: Session = Depends(get_db)):
    devices = db.query(DeviceRecord).all()
    return [{
        "device_id": d.device_id,
        "worker_name": d.worker_name,
        "location": d.manhole_location,
        "status": d.status,
        "battery_percent": d.battery_percent,
        "last_seen": d.last_seen.isoformat() if d.last_seen else None,
        "firmware": d.firmware_version
    } for d in devices]

@router.get("/devices/{device_id}/telemetry")
def get_device_telemetry(device_id: str, limit: int = Query(50, le=500), db: Session = Depends(get_db)):
    logs = db.query(TelemetryLog).filter(
        TelemetryLog.device_id == device_id
    ).order_by(TelemetryLog.timestamp.desc()).limit(limit).all()
    
    return [{
        "id": l.id,
        "timestamp": l.timestamp.isoformat(),
        "O2": l.o2,
        "H2S": l.h2s,
        "CO": l.co,
        "CH4": l.ch4,
        "temp": l.temp,
        "hum": l.hum,
        "water_level": l.water_level,
        "ax": l.ax,
        "ay": l.ay,
        "az": l.az,
        "risk_score": l.risk_score,
        "status": l.status,
        "anomaly": l.anomaly,
        "fall_detected": l.fall_detected
    } for l in reversed(logs)]

@router.get("/devices/{device_id}/alerts")
def get_device_alerts(device_id: str, limit: int = Query(20, le=100), db: Session = Depends(get_db)):
    alerts = db.query(AlertLog).filter(
        AlertLog.device_id == device_id
    ).order_by(AlertLog.timestamp.desc()).limit(limit).all()
    
    return [{
        "id": a.id,
        "timestamp": a.timestamp.isoformat(),
        "alert_type": a.alert_type,
        "severity": a.severity,
        "message": a.message,
        "acknowledged": a.acknowledged,
        "acknowledged_by": a.acknowledged_by
    } for a in alerts]

@router.post("/alerts/{alert_id}/acknowledge")
def acknowledge_alert(alert_id: int, ack: AlertAcknowledge, db: Session = Depends(get_db)):
    alert = db.query(AlertLog).filter(AlertLog.id == alert_id).first()
    if not alert:
        raise HTTPException(status_code=404, detail="Alert not found")
        
    alert.acknowledged = True
    alert.acknowledged_by = ack.acknowledged_by
    db.commit()
    return {"message": "Alert acknowledged", "alert_id": alert_id}

@router.get("/devices/{device_id}/config")
def get_device_config(device_id: str, db: Session = Depends(get_db)):
    cfg = db.query(DeviceConfig).filter(DeviceConfig.device_id == device_id).first()
    if not cfg:
        cfg = DeviceConfig(device_id=device_id)
        db.add(cfg)
        db.commit()
        db.refresh(cfg)
        
    return {
        "device_id": cfg.device_id,
        "weights": {
            "o2": cfg.weight_o2,
            "h2s": cfg.weight_h2s,
            "co": cfg.weight_co,
            "ch4": cfg.weight_ch4,
            "water": cfg.weight_water,
            "motion": cfg.weight_motion
        },
        "thresholds": {
            "safe_max": cfg.threshold_safe_max,
            "warning_max": cfg.threshold_warning_max,
            "o2_override": cfg.o2_override_threshold
        },
        "updated_at": cfg.updated_at.isoformat()
    }

@router.post("/devices/{device_id}/config")
def update_device_config(device_id: str, config_update: DeviceConfigUpdate, db: Session = Depends(get_db)):
    cfg = db.query(DeviceConfig).filter(DeviceConfig.device_id == device_id).first()
    if not cfg:
        cfg = DeviceConfig(device_id=device_id)
        db.add(cfg)

    cfg.weight_o2 = config_update.weight_o2
    cfg.weight_h2s = config_update.weight_h2s
    cfg.weight_co = config_update.weight_co
    cfg.weight_ch4 = config_update.weight_ch4
    cfg.weight_water = config_update.weight_water
    cfg.weight_motion = config_update.weight_motion
    cfg.threshold_safe_max = config_update.threshold_safe_max
    cfg.threshold_warning_max = config_update.threshold_warning_max
    cfg.o2_override_threshold = config_update.o2_override_threshold
    cfg.updated_at = datetime.datetime.utcnow()
    db.commit()

    # Dispatch to device via MQTT
    mqtt_service.publish_config(device_id, {
        "weights": {
            "o2": cfg.weight_o2,
            "h2s": cfg.weight_h2s,
            "co": cfg.weight_co,
            "ch4": cfg.weight_ch4,
            "water": cfg.weight_water,
            "motion": cfg.weight_motion
        },
        "thresholds": {
            "safe_max": cfg.threshold_safe_max,
            "warning_max": cfg.threshold_warning_max,
            "o2_override": cfg.o2_override_threshold
        }
    })

    return {"message": "Configuration updated and pushed to device", "device_id": device_id}

@router.post("/telemetry")
async def ingest_telemetry_http(payload: TelemetryPayload):
    """HTTP fallback when MQTT is unavailable in subterranean environments."""
    mqtt_service.handle_telemetry(payload.model_dump())
    return {"status": "ingested"}

@router.post("/alerts")
async def ingest_alert_http(payload: AlertPayload):
    """HTTP fallback for emergency alerts."""
    res = alert_service.process_and_dispatch(
        payload.device_id, payload.alert_type, payload.severity, payload.message
    )
    await ws_hub.broadcast({"type": "alert", "data": res})
    return res

@router.post("/simulate/scenario")
async def simulate_scenario(scenario_id: str = Query("T1", description="Scenario ID T1 through T13")):
    """
    Simulates testing scenarios T1 - T13 live on device SB-001 and broadcasts telemetry.
    """
    scenarios = {
        "T1": {"name": "Normal Atmospheric", "o2": 20.9, "h2s": 0.1, "co": 1.0, "ch4": 0.0, "water": 5.0, "status": "SAFE", "risk": 3.2, "anom": False, "fall": False, "ax": 0.05, "ay": 0.02, "az": 0.98},
        "T2": {"name": "Oxygen Reduction", "o2": 15.2, "h2s": 0.5, "co": 2.0, "ch4": 0.0, "water": 8.0, "status": "CRITICAL", "risk": 78.4, "anom": True, "fall": False, "ax": 0.04, "ay": 0.01, "az": 0.99},
        "T3": {"name": "Increasing H2S", "o2": 20.4, "h2s": 24.5, "co": 3.0, "ch4": 1.0, "water": 10.0, "status": "CRITICAL", "risk": 72.0, "anom": True, "fall": False, "ax": 0.06, "ay": 0.02, "az": 0.98},
        "T4": {"name": "Increasing CO", "o2": 20.2, "h2s": 1.0, "co": 85.0, "ch4": 0.0, "water": 7.0, "status": "CRITICAL", "risk": 68.5, "anom": True, "fall": False, "ax": 0.05, "ay": 0.02, "az": 0.98},
        "T5": {"name": "Increasing CH4", "o2": 19.8, "h2s": 2.0, "co": 5.0, "ch4": 35.0, "water": 12.0, "status": "CRITICAL", "risk": 65.0, "anom": True, "fall": False, "ax": 0.07, "ay": 0.01, "az": 0.97},
        "T6": {"name": "Rising Water Level", "o2": 20.6, "h2s": 2.5, "co": 4.0, "ch4": 2.0, "water": 75.0, "status": "CRITICAL", "risk": 62.0, "anom": True, "fall": False, "ax": 0.04, "ay": 0.03, "az": 0.98},
        "T7": {"name": "High Temperature Heat Stress", "o2": 20.8, "h2s": 0.2, "co": 1.5, "ch4": 0.0, "water": 6.0, "temp": 42.5, "hum": 92.0, "status": "WARNING", "risk": 44.0, "anom": True, "fall": False, "ax": 0.05, "ay": 0.02, "az": 0.98},
        "T8": {"name": "Worker Fall Event", "o2": 20.9, "h2s": 0.2, "co": 1.0, "ch4": 0.0, "water": 8.0, "status": "CRITICAL", "risk": 85.0, "anom": True, "fall": True, "ax": 0.85, "ay": 0.52, "az": 0.08},
        "T9": {"name": "Worker Inactivity", "o2": 20.8, "h2s": 0.1, "co": 1.0, "ch4": 0.0, "water": 6.0, "status": "WARNING", "risk": 38.0, "anom": True, "fall": False, "ax": 0.01, "ay": 0.01, "az": 0.99},
        "T10": {"name": "Compound Hazards", "o2": 14.8, "h2s": 18.5, "co": 32.0, "ch4": 15.0, "water": 55.0, "status": "CRITICAL", "risk": 96.5, "anom": True, "fall": False, "ax": 0.10, "ay": 0.05, "az": 0.95},
        "T11": {"name": "Sensor Failure", "o2": -2.0, "h2s": 0.0, "co": 0.0, "ch4": 0.0, "water": 5.0, "status": "WARNING", "risk": 45.0, "anom": True, "fall": False, "ax": 0.05, "ay": 0.02, "az": 0.98},
        "T12": {"name": "Communication Failure", "o2": 18.2, "h2s": 8.0, "co": 15.0, "ch4": 5.0, "water": 20.0, "status": "WARNING", "risk": 42.0, "anom": False, "fall": False, "ax": 0.05, "ay": 0.02, "az": 0.98},
        "T13": {"name": "Low Battery Condition", "o2": 20.9, "h2s": 0.1, "co": 1.0, "ch4": 0.0, "water": 5.0, "status": "SAFE", "risk": 12.0, "anom": False, "fall": False, "ax": 0.05, "ay": 0.02, "az": 0.98, "battery": 8.5}
    }
    
    cfg = scenarios.get(scenario_id.upper(), scenarios["T1"])
    now_ts = int(datetime.datetime.utcnow().timestamp())
    
    payload = {
        "device_id": "SB-001",
        "timestamp": now_ts,
        "sensors": {
            "O2": cfg.get("o2", 20.9),
            "H2S": cfg.get("h2s", 0.0),
            "CO": cfg.get("co", 1.0),
            "CH4": cfg.get("ch4", 0.0),
            "temp": cfg.get("temp", 26.5),
            "hum": cfg.get("hum", 68.0),
            "water_level": cfg.get("water", 5.0),
            "ax": cfg.get("ax", 0.05),
            "ay": cfg.get("ay", 0.02),
            "az": cfg.get("az", 0.98)
        },
        "risk_score": cfg.get("risk", 5.0),
        "status": cfg.get("status", "SAFE"),
        "anomaly": cfg.get("anom", False),
        "fall_detected": cfg.get("fall", False)
    }
    
    mqtt_service.handle_telemetry(payload)
    if cfg.get("status") == "CRITICAL" or cfg.get("fall"):
        alert_service.process_and_dispatch("SB-001", cfg["name"], "CRITICAL", f"Scenario {scenario_id} triggered hazard escalation!")

    return {"scenario": scenario_id, "name": cfg["name"], "telemetry": payload}
