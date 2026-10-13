import datetime
from sqlalchemy import create_engine, Column, Integer, String, Float, Boolean, DateTime, Text
from sqlalchemy.orm import declarative_base, sessionmaker
from .config import settings

engine = create_engine(
    settings.DATABASE_URL,
    connect_args={"check_same_thread": False} if "sqlite" in settings.DATABASE_URL else {}
)
SessionLocal = sessionmaker(autocommit=False, autoflush=False, bind=engine)
Base = declarative_base()

class DeviceRecord(Base):
    __tablename__ = "devices"

    id = Column(Integer, primary_key=True, index=True)
    device_id = Column(String(32), unique=True, index=True, nullable=False)
    worker_name = Column(String(64), default="Confined Space Worker")
    manhole_location = Column(String(128), default="Zone 4 - Section B")
    status = Column(String(16), default="SAFE")
    last_seen = Column(DateTime, default=datetime.datetime.utcnow)
    battery_percent = Column(Float, default=100.0)
    firmware_version = Column(String(16), default="v1.2.0-esp32s3")

class TelemetryLog(Base):
    __tablename__ = "telemetry_logs"

    id = Column(Integer, primary_key=True, index=True)
    device_id = Column(String(32), index=True, nullable=False)
    timestamp = Column(DateTime, default=datetime.datetime.utcnow)
    o2 = Column(Float)
    h2s = Column(Float)
    co = Column(Float)
    ch4 = Column(Float)
    temp = Column(Float)
    hum = Column(Float)
    water_level = Column(Float)
    ax = Column(Float)
    ay = Column(Float)
    az = Column(Float)
    risk_score = Column(Float)
    status = Column(String(16))
    anomaly = Column(Boolean, default=False)
    fall_detected = Column(Boolean, default=False)

class AlertLog(Base):
    __tablename__ = "alert_logs"

    id = Column(Integer, primary_key=True, index=True)
    device_id = Column(String(32), index=True, nullable=False)
    timestamp = Column(DateTime, default=datetime.datetime.utcnow)
    alert_type = Column(String(64), nullable=False)
    severity = Column(String(16), default="CRITICAL")
    message = Column(Text, nullable=False)
    acknowledged = Column(Boolean, default=False)
    acknowledged_by = Column(String(64), nullable=True)

class DeviceConfig(Base):
    __tablename__ = "device_configs"

    id = Column(Integer, primary_key=True, index=True)
    device_id = Column(String(32), unique=True, index=True, nullable=False)
    weight_o2 = Column(Float, default=0.30)
    weight_h2s = Column(Float, default=0.25)
    weight_co = Column(Float, default=0.15)
    weight_ch4 = Column(Float, default=0.15)
    weight_water = Column(Float, default=0.10)
    weight_motion = Column(Float, default=0.05)
    threshold_safe_max = Column(Float, default=30.0)
    threshold_warning_max = Column(Float, default=60.0)
    o2_override_threshold = Column(Float, default=19.5)
    updated_at = Column(DateTime, default=datetime.datetime.utcnow)

def init_db():
    Base.metadata.create_all(bind=engine)
    db = SessionLocal()
    # Pre-seed default device if not present
    if not db.query(DeviceRecord).filter(DeviceRecord.device_id == "SB-001").first():
        dev = DeviceRecord(
            device_id="SB-001",
            worker_name="Ravi Kumar (Worker #14)",
            manhole_location="MH-402 (Central Interceptor, 4.5m)",
            status="SAFE",
            battery_percent=88.5
        )
        cfg = DeviceConfig(device_id="SB-001")
        db.add(dev)
        db.add(cfg)
        db.commit()
    db.close()
