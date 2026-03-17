from fastapi import Request, APIRouter, Form
from fastapi.responses import HTMLResponse
from toolbox.templates import templates
from typing import Annotated
import subprocess

router = APIRouter()

@router.get("/", response_class=HTMLResponse, include_in_schema=False)
async def root(request: Request):
    response = templates.TemplateResponse(request=request,
                                          name='wifi.html',
                                          context={'nav_page': 'wifi'})

    return response

def connect_to_wifi(ssid: str, psk: str):
    # cd /etc/NetworkManager/system-connections/
    try:
        out = subprocess.check_output(['nmcli', 'device', 'wifi', 'connect', ssid, 'password', psk])
    except:
        out = 'ERROR'
    return out

@router.post('/config/', response_class=HTMLResponse, include_in_schema=True)
async def wifi_config(request: Request,
                      ssid: Annotated[str, Form()],
                      psk: Annotated[str, Form()],):
    """
    Automatic connection to a WiFi network with SSID and PSK
    """
    response = connect_to_wifi(ssid=ssid, psk=psk)
    html_content = f'{response}'
    return HTMLResponse(content=html_content, status_code=200)