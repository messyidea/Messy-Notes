# -*- coding: UTF-8 -*-
import urllib
import urllib2
import json

KEY = 'your key'
URL = 'http://www.tuling123.com/openapi/api'

values = {}
values['key'] = KEY
values['info'] = '你好'
data = urllib.urlencode(values)
#print data
request = urllib2.Request(URL, data)
response = urllib2.urlopen(request)
mess = response.read()
decoded = json.loads(mess)
print decoded['text']
