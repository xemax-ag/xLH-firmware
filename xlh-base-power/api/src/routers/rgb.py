import subprocess
import time
from typing import Annotated
from fastapi import FastAPI, Request, Form, APIRouter
from fastapi.middleware.cors import CORSMiddleware
from fastapi.responses import HTMLResponse
from fastapi.staticfiles import StaticFiles
from starlette.routing import Router
from rgb.rgb_strip import RgbMatrixValues, RgbPixel, rgb_cmd_pixel, rgb_cmd_wipe, eRgbRemoteState
from toolbox.templates import templates
from fastapi.responses import HTMLResponse, PlainTextResponse, JSONResponse

router = APIRouter()

@router.post("/values/", response_class=JSONResponse, include_in_schema=True)
async def rgb_values(rgb_matrix: RgbMatrixValues):
    response = {'msg': 'none'}
    try:
        len_check = True
        if len(rgb_matrix.values) != 8:
            len_check = False
        for row in rgb_matrix.values:
            if len(row) != 8:
                len_check = False
        if len_check:
            rgb_state = rgb_cmd_pixel(rgb_matrix.values)
            if rgb_state == eRgbRemoteState.API:
                response['msg'] = 'ok'
            elif rgb_state == eRgbRemoteState.CODESYS:
                response['msg'] = 'info codesys is rgb-writer'
            else:
                response['msg'] = 'error'
        else:
            response['msg'] = 'error wrong array dimensions'
    except Exception as exc:
        response['msg'] = f'error => {exc}'
    return response

@router.get('/wipe/{r}/{g}/{b}/', response_class=JSONResponse, include_in_schema=True)
async def rgb_wipe(r: int = 0, g: int = 0, b: int = 0):
    """
    Control of the RGB matrix via GET request
    """
    rgb_pixel = RgbPixel()
    rgb_pixel.r = r
    rgb_pixel.g = g
    rgb_pixel.b = b

    rgb_state = rgb_cmd_wipe(rgb_pixel)
    if rgb_state == eRgbRemoteState.API:
        response = {'msg': 'ok'}
    elif rgb_state == eRgbRemoteState.CODESYS:
        response = {'msg': 'info codesys is rgb-writer'}
    else:
        response = {'msg': 'error'}
    return response
