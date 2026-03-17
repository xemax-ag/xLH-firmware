from fastapi import Request, APIRouter
from fastapi.responses import HTMLResponse, JSONResponse
from fastapi import Request, APIRouter, Form
from app.core.templates.base import template_base
import subprocess
from typing import Annotated
from pydantic import BaseModel
import time

router = APIRouter()

# =====================================================================================================================
@router.get('/', response_class=HTMLResponse, include_in_schema=False)
@router.get('/index/', response_class=HTMLResponse, include_in_schema=False)
async def index(request: Request):
    response = template_base.TemplateResponse(request=request,
                                              name='index.html',
                                              context={'nav_page': 'index'})
    return response

# =====================================================================================================================
# @router.get('/wifi/', response_class=HTMLResponse, include_in_schema=False)
# async def wifi(request: Request):
#     response = template_base.TemplateResponse(request=request,
#                                               name='wifi.html',
#                                               context={'nav_page': 'wifi'})
#     return response

# def connect_to_wifi(ssid: str, psk: str):
#     # cd /etc/NetworkManager/system-connections/
#     try:
#         # print(ssid, psk)
#         out = subprocess.check_output(['nmcli', 'device', 'wifi', 'connect', ssid, 'password', psk])
#     except:
#         out = 'error'
#     return out

# @router.post('/wifi-config/', response_class=HTMLResponse, include_in_schema=False)
# async def wifi_config(request: Request,
#                       ssid: Annotated[str, Form()],
#                       psk: Annotated[str, Form()],):
#     """
#     Automatic connection to a WiFi network with SSID and PSK
#     """
#     response = connect_to_wifi(ssid=ssid, psk=psk)
#     html_content = f'{response}'
#     return HTMLResponse(content=html_content, status_code=200)

# def set_wifi_country_code(cc: str):
#     # sudo iw reg set CH
#     try:
#         print(cc)
#         out = subprocess.check_output(['sudo', 'iw', 'reg', 'set', cc])
#         time.sleep(2.5)
#         out = subprocess.check_output(['sudo', 'reboot'])
#     except:
#         out = 'error'
#     return out

# class Cc(BaseModel):
#     country_code: str

# @router.post('/wifi-country-code/', response_class=HTMLResponse, include_in_schema=False)
# async def wifi_country_code(request: Request, cc: Cc):
#     # print(cc.country_code)
#     if cc.country_code != '':
#         response = set_wifi_country_code(cc=cc.country_code)
#     else:
#         response = 'country code invalid'
#     return HTMLResponse(content=response, status_code=200)

# =====================================================================================================================

@router.get('/terminal/', response_class=HTMLResponse, include_in_schema=False)
async def terminal(request: Request):
    response = template_base.TemplateResponse(request=request,
                                              name='terminal.html',
                                              context={'nav_page': 'terminal'})
    return response

# =====================================================================================================================
@router.get('/.well-known/appspecific/com.chrome.devtools.json')
async def chrome_devtools():
    # chrome devtools
    # return JSONResponse(content=[], status_code=200)
    return []

# =====================================================================================================================
