from pydantic import BaseModel, Field
from typing import Optional, Dict
import datetime

class SensorData(BaseModel):
    O2: float = Field(..., description="Oxygen percentage (normal ~20.9%)")
    H2S: float = Field(..., description="Hydrogen Sulphide ppm")
    CO: float = Field(..., description="Carbon Monoxide ppm")
    CH4: float = Field(..., description="Methane % LEL")
    temp: float = Field(..., description="Ambient temperature in C")
    hum: float = Field(..., description="Relative humidity %")
    water_level: float = Field(..., description="Water level % in shaft")
    ax: float = Field(..., description="IMU X acceleration (g)")
    ay: float = Field(..., description="IMU Y acceleration (g)")
    az: float = Field(..., description="IMU Z acceleration (g)")

class TelemetryPayload(BaseModel):
    device_id: str
    timestamp: int
    sensors: SensorData
    risk_score: float
    status: str
    anomaly: bool = False
    fall_detected: bool = False

class AlertPayload(BaseModel):
    device_id: str
    timestamp: int
    alert_type: str
    severity: str = "CRITICAL"
    message: str

class DeviceConfigUpdate(BaseModel):
    weight_o2: Optional[float] = 0.30
    weight_h2s: Optional[float] = 0.25
    weight_co: Optional[float] = 0.15
    weight_ch4: Optional[float] = 0.15
    weight_water: Optional[float] = 0.10
    weight_motion: Optional[float] = 0.05
    threshold_safe_max: Optional[float] = 30.0
    threshold_warning_max: Optional[float] = 60.0
    o2_override_threshold: Optional[float] = 19.5

class AlertAcknowledge(BaseModel):
    acknowledged_by: str = "Supervisor on Duty"
