import datetime as dt
import typing
from functools import lru_cache
from fastapi import Request
from fastapi.templating import Jinja2Templates

@lru_cache
def get_datetime():
    return f'''{dt.datetime.now().strftime('%d.%m.%y-%H:%M:%S')}'''

def app_context(request: Request) -> typing.Dict[str, typing.Any]:
    return {'app': request.app}

def active(page: str, page_cmp: str):
    if page == page_cmp:
        return 'active'
    else:
        return ''

def show(page: str, lst_page_cmp: list[str]):
    for page_cmp in lst_page_cmp:
        if page == page_cmp:
            return 'show'
    return ''

# function injection
template_base = Jinja2Templates(directory='./app/templates', context_processors=[app_context])
template_base.env.globals.update({
    'show': show,
    'active': active,
    'get_datetime': get_datetime,
})


