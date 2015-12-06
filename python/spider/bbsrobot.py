# -*- coding: UTF-8 -*-
from bs4 import BeautifulSoup
import urllib
import urllib2
import json
import cookielib
import re
import sqlite3

class BBSRobot:
    def __init__(self):
        self.login_url = 'http://bbs.zjut.edu.cn/account/ajax/login_process/'
        self.surf_url = 'http://bbs.zjut.edu.cn/'
        self.cookie = cookielib.CookieJar()
        self.handler=urllib2.HTTPCookieProcessor(self.cookie)
        self.opener = urllib2.build_opener(self.handler)
        self.values = {}
        self.headers = {}
        self.headers['User-Agent'] =  'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/45.0.2454.101 Safari/537.36'
        self.headers['Host'] = 'bbs.zjut.edu.cn'
        self.headers['Connection'] = 'keep-alive'
        self.con = sqlite3.connect("./visit.db")
        self.cu = self.con.cursor()
        self.cu.execute("create table if not exists visit(id integer primary key)")
        """
        #self.cu.execute("insert into visit values(1)")
        self.con.commit()
        self.cu.execute("select * from visit where id = 3")
        rows = self.cu.fetchall()
        print len(rows)
        self.cu.execute("select * from visit where id = 1")
        rows = self.cu.fetchall()
        print len(rows)
        #self.cu = self.con.cursor()
        #self.cu.execute("create table IF NOT EXISTS visit(id integer primary key) ")
        """


    def login(self):
        values = {}
        values['return_url'] = 'http://bbs.zjut.edu.cn/'
        values['user_name'] = '***********'
        values['password'] = '******'
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
        requests = urllib2.Request(self.surf_url, headers = self.headers)
        html = self.opener.open(requests).read()
        #print html
        #print html.read()
        soup = BeautifulSoup(html, "html.parser")
        theme_link_list = soup.findAll(href = re.compile("^http://bbs.zjut.edu.cn/question/\d+$"))
        #print theme_link_list
        ids = []
        #print theme_link_list[0]['href'].split('/')[-1]
        flag = False
        for link in theme_link_list:
            num = link['href'].split('/')[-1]
            self.cu.execute("select * from visit where id = %s" %(num))
            rows = self.cu.fetchall()
            if len(rows) == 0:
                self.cu.execute("insert into visit values(%s)" %(num))
                ids.append(num)
            #ids.append(link['href'].split('/')[-1])
        self.con.commit()
        print ids


    def start(self):
        print "start"
        self.login()
        self.surf()


robot = BBSRobot()
robot.start()
