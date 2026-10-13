import os
from pydantic_settings import BaseSettings

class Settings(BaseSettings):
    PROJECT_NAME: str = "SENTINEL-BELT Cloud Backend"
    API_V1_STR: str = "/api/v1"
    
    # MQTT Configuration
    MQTT_BROKER_HOST: str = os.getenv("MQTT_BROKER_HOST", "localhost")
    MQTT_BROKER_PORT: int = int(os.getenv("MQTT_BROKER_PORT", "1883"))
    MQTT_CLIENT_ID: str = "sentinel_backend_subscriber"
    MQTT_KEEPALIVE: int = 60
    
    # Database
    DATABASE_URL: str = os.getenv("DATABASE_URL", "sqlite:///./sentinel_belt.db")
    
    # Alert Dispatch Settings
    SUPERVISOR_SMS_PHONE: str = os.getenv("SUPERVISOR_SMS_PHONE", "+91-9876543210")
    SUPERVISOR_EMAIL: str = os.getenv("SUPERVISOR_EMAIL", "safety_control@confinedspaces.org")
    
    class Config:
        case_sensitive = True

settings = Settings()
