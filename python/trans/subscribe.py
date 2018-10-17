# -*- coding: utf-8 -*-
import base64
import json
import re
import requests
import os
from xpinyin import Pinyin
from urllib.parse import unquote_plus, quote_plus

def print_dict(d):
    res = ''
    for key in d:
        res += '{}:{}\n'.format(key, d[key])
    return res[:-1]

def base64decode(s):
    if len(s) % 4 != 0:
        s = s + (4 - len(s) % 4)*'='
    return base64.urlsafe_b64decode(s.encode())

def decode_ssr(ssr):
    if ssr == '':
        return
    print(ssr)
    dec = base64decode(ssr[6:])
    server = {}
    dec = str(dec)[2:-1]
    print(dec)
    parts = str(dec[1:-1]).split("/?")

    part1 = parts[0].split(':')
    part2 = parts[1].split('&')

    # print(part1)
    # print(part2)
    server['server'] = part1[0]
    server['server_port'] = part1[1]
    server['method'] = part1[3]
    server['timeout'] = 300
    server['local_address'] = '0.0.0.0'
    server['local_port'] = 1080
    server['password'] = base64decode(part1[5]).decode('utf-8')
    print(server)

    p = Pinyin()
    part2 = part2[2]
    ts = part2.split('=')
    name = base64decode(ts[1]).decode('utf-8')
    engname = p.get_pinyin(name[:2])
    name = engname + name

    with open("configs/" + name + '.json', 'w') as f:
        f.write(json.dumps(server, indent=4, sort_keys=True))

def read_config():
    subscribe_config = json.load(open("subscribe.json", 'r'))
    servers = subscribe_config.get('servers', [])
    res = []
    for s in servers:
        url = s.get('url')
        resp = requests.get(url).text
        resp = base64decode(resp).decode('utf-8')
        links = resp.split('\n')
        print('解析链接({})成功,此URL包含{}条ss://链接:'.format(url, len(links)))
        for l in links:
            decode_ssr(l)

if __name__ == '__main__':
    os.mkdir("configs")
    read_config()
