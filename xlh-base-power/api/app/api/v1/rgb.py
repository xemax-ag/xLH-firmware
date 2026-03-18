from fastapi import APIRouter
from fastapi.responses import JSONResponse
from app.services.rgb.rgb_strip import RgbMatrixValues, RgbPixel, rgb_cmd_pixel, rgb_cmd_wipe, eRgbRemoteState

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
                response['msg'] = 'codesys is rgb-writer'
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
    response = {'msg': 'none'}
    if rgb_state == eRgbRemoteState.API:
        response['msg'] = 'ok'
    elif rgb_state == eRgbRemoteState.CODESYS:
        response['msg'] = 'codesys is rgb-writer'
    else:
        response['msg'] = 'error'
    return response
