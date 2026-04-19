from fastapi import APIRouter
from fastapi.responses import JSONResponse
from app.services.rgb.rgb_strip import RgbMatrixValues, RgbPixel, rgb_cmd_pixel, rgb_cmd_wipe, eRgbRemoteState
from pydantic import BaseModel
from app.core.config import config

router = APIRouter()

class AppVersion(BaseModel):
    version: str

@router.get('/app/', response_class=JSONResponse, include_in_schema=True)
async def app() -> AppVersion:
    return AppVersion(version=config.app_version)
