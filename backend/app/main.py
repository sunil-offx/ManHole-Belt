import os
import logging
from fastapi import FastAPI, WebSocket, WebSocketDisconnect
from fastapi.middleware.cors import CORSMiddleware
from fastapi.staticfiles import StaticFiles

from .config import settings
from .database import init_db
from .routes import router as api_router
from .mqtt_service import mqtt_service
from .websocket_hub import ws_hub

logging.basicConfig(level=logging.INFO, format="%(asctime)s [%(levelname)s] %(name)s: %(message)s")
logger = logging.getLogger("sentinel.main")

app = FastAPI(
    title=settings.PROJECT_NAME,
    version="1.0.0",
    description="Central Safety Monitoring & Alert Gateway for SENTINEL-BELT Confined-Space Protection"
)

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

@app.on_event("startup")
def startup_event():
    logger.info("Initializing database...")
    init_db()
    logger.info("Starting MQTT Subscriber Service...")
    mqtt_service.start()

@app.on_event("shutdown")
def shutdown_event():
    logger.info("Stopping MQTT Subscriber...")
    mqtt_service.stop()

# Include REST API
app.include_router(api_router, prefix=settings.API_V1_STR)

# Real-Time WebSocket stream
@app.websocket("/ws/telemetry")
async def websocket_telemetry_endpoint(websocket: WebSocket):
    await ws_hub.connect(websocket)
    try:
        while True:
            # Keep connection alive & handle incoming pings
            data = await websocket.receive_text()
    except WebSocketDisconnect:
        ws_hub.disconnect(websocket)
    except Exception as e:
        logger.error(f"WebSocket error: {e}")
        ws_hub.disconnect(websocket)

# Health endpoint
@app.get("/health")
def health_check():
    return {
        "status": "healthy",
        "mqtt_connected": mqtt_service.connected,
        "active_websockets": len(ws_hub.active_connections)
    }

# Mount supervisor dashboard static files
dashboard_path = os.path.abspath(os.path.join(os.path.dirname(__file__), "../../dashboard"))
if os.path.exists(dashboard_path):
    app.mount("/", StaticFiles(directory=dashboard_path, html=True), name="dashboard")
