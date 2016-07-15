# -*- coding: UTF-8 -*-
import urllib
import urllib2
import requests
import json

def get_location3(ip):
    try:
        url = "http://ipinfo.io/" + ip
        json_text = requests.get(url, timeout=16).text
        # headers = {}
        # headers['User-Agent'] =  'python-requests/2.9.1'
        # reques = urllib2.Request(url, headers=headers)
        # json_text = urllib2.urlopen(reques).read()
        # print json_text
        real_json = json.loads(json_text)

        if real_json.has_key('loc'):
            loc = real_json.get("loc")
        else:
            return 'undef'

        url2 = 'https://maps.googleapis.com/maps/api/geocode/json?latlng=' + loc
        headers = {}
        headers['User-Agent'] =  'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/45.0.2454.101 Safari/537.36'
        headers['Connection'] = 'keep-alive'
        headers['Accept-Language'] = 'zh-CN,zh;q=0.8,en;q=0.6,zh-TW;q=0.4,und;q=0.2'
        headers['Accept-Encoding'] = 'deflate'
        headers['Accept'] = 'application/json, text/javascript, */*; q=0.01'
        req = urllib2.Request(url2, headers=headers)
        response = urllib2.urlopen(req)
        html = response.read()
        rst = json.loads(html)
        # print rst['results'][0]['formatted_address']
        return rst['results'][0]['formatted_address']
    except:
        return 'undef'
    


get_location_ipv6("2607:f140:8801::1:23")
get_location_ipv6("2001:da8:207:e182:dad3:85ff:fefa:6446")
# 2001:da8:207:e182:dad3:85ff:fefa:6446
# get_location_ipv6("123")