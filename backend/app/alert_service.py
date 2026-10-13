import logging
import datetime
from .config import settings
from .database import SessionLocal, AlertLog, DeviceRecord

logger = logging.getLogger("sentinel.alert_service")

class AlertService:
    @staticmethod
    def process_and_dispatch(device_id: str, alert_type: str, severity: str, message: str) -> dict:
        timestamp = datetime.datetime.utcnow()
        db = SessionLocal()
        try:
            # 1. Persist alert in database
            alert_entry = AlertLog(
                device_id=device_id,
                timestamp=timestamp,
                alert_type=alert_type,
                severity=severity,
                message=message,
                acknowledged=False
            )
            db.add(alert_entry)

            # 2. Update device status
            dev = db.query(DeviceRecord).filter(DeviceRecord.device_id == device_id).first()
            if dev:
                dev.status = severity
                dev.last_seen = timestamp

            db.commit()
            db.refresh(alert_entry)

            # 3. Trigger simulated SMS / Email / Webhook to Safety Supervisor
            sms_dispatch = f"[SMS SENT TO {settings.SUPERVISOR_SMS_PHONE}] ALERT: {device_id} - {alert_type}: {message}"
            email_dispatch = f"[EMAIL TO {settings.SUPERVISOR_EMAIL}] URGENT INCIDENT REPORT for {device_id}: {message}"
            
            logger.warning(f"🚨 DISPATCHING EMERGENCY NOTIFICATION: {sms_dispatch}")
            logger.warning(f"🚨 EMAIL ESCALATION: {email_dispatch}")

            return {
                "alert_id": alert_entry.id,
                "device_id": device_id,
                "timestamp": timestamp.isoformat(),
                "alert_type": alert_type,
                "severity": severity,
                "message": message,
                "sms_dispatched": True,
                "email_dispatched": True
            }
        finally:
            db.close()

alert_service = AlertService()
