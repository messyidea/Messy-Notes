# -*- coding: UTF-8 -*-
from bs4 import BeautifulSoup
import urllib
import urllib2
import json
import cookielib
import re

class BBSRobot:
    def __init__(self):
        self.login_url = 'http://bbs.zjut.edu.cn/account/ajax/login_process/'
        self.surf_url = 'http://bbs.zjut.edu.cn/'
        self.cookie = cookielib.CookieJar()
        self.handler=urllib2.HTTPCookieProcessor(self.cookie)
        self.opener = urllib2.build_opener(self.handler)
        self.values = {}


    def login(self):
        values = {}
        values['return_url'] = 'http://bbs.zjut.edu.cn/'
        values['user_name'] = '**********'
        values['password'] = '*****'
        values['_post_type'] = 'ajax'
        data = urllib.urlencode(values)
        headers = {}
        headers['User-Agent'] =  'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/45.0.2454.101 Safari/537.36'
        headers['Referer'] = 'http://bbs.zjut.edu.cn/account/login/'
        headers['Host'] = 'bbs.zjut.edu.cn'
        headers['Connection'] = 'keep-alive'
        headers['Accept-Language'] = 'zh-CN,zh;q=0.8,en;q=0.6,zh-TW;q=0.4,und;q=0.2'
        headers['Accept-Encoding'] = 'gzip, deflate'
        headers['Accept'] = 'application/json, text/javascript, */*; q=0.01'
        requests = urllib2.Request(self.login_url, data, headers)
        response = self.opener.open(requests)
        #self.cookie.save(ignore_discard=True, ignore_expires=True)
        #print html.read()

    def surf(self):
        headers = {}
        headers['User-Agent'] =  'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/45.0.2454.101 Safari/537.36'
        headers['Host'] = 'bbs.zjut.edu.cn'
        headers['Connection'] = 'keep-alive'
        requests = urllib2.Request(self.surf_url, headers = headers)
        html = self.opener.open(requests)
        soup = BeautifulSoup(html)
        
        print html.read()

    def start(self):
        self.login()
        self.surf()


robot = BBSRobot()
robot.start()
