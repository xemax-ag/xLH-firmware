from fastapi import Request, APIRouter
from fastapi.responses import HTMLResponse
from toolbox.templates import templates

router = APIRouter()

@router.get('/', response_class=HTMLResponse, include_in_schema=False)
@router.get('/index/', response_class=HTMLResponse, include_in_schema=False)
async def index(request: Request):
    response = templates.TemplateResponse(request=request,
                                              name='index.html',
                                              context={'nav_page': 'index'})
    # print('session key', request.cookies.get('session_key'))
    return response
