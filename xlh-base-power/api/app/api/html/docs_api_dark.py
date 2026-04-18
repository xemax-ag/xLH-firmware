import fastapi_swagger_dark as fsd
from fastapi import APIRouter

router = APIRouter()
fsd.install(router)