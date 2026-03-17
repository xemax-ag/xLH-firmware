import requests
import json

URL = 'http://192.168.1.31:8080'

def rgb_values(values):
    payload = {
        'values': values
    }
    headers = {
        'Content-Type': 'application/json',
        'User-Agent': 'xLH'
    }
    response = requests.request('POST', f'{URL}/rgb/values/', json=payload, headers=headers)
    return response.json()

def rgb_wipe(r: int = 0, g: int = 0, b: int = 0):
    print(r, g, b)
    headers = {
        'User-Agent': 'xLH'
    }
    response = requests.request('GET', f'{URL}/rgb/wipe/{r}/{g}/{b}/', headers=headers)
    # print(response)
    return response.json()

values = [[{'r': 0, 'g': 0, 'b': 0} for i in range(8)] for j in range(8)]
#json.dumps(values)
values[0][1] = {'r': 0, 'g': 10, 'b': 0}
print(rgb_values(values))
print(rgb_wipe(r=0, g=0, b=0))
