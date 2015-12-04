# -*- coding: UTF-8 -*-
from bs4 import BeautifulSoup
import urllib
import urllib2
import cookielib
import re

class SPIDER:
    def __init__(self):
        self.url = "http://210.32.205.60/Search.aspx"
        self.headers = {}
        self.headers['User-Agent'] =  'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/45.0.2454.101 Safari/537.36'
        self.headers['Referer'] = 'http://210.32.205.60/Search.aspx'

        page = urllib2.urlopen(self.url).read()
        soup = BeautifulSoup(page, 'html.parser')
        self.values = {}
        self.values['ctl00$ScriptManager1'] = 'ctl00$ContentPlaceHolder1$UpdatePanel1|ctl00$ContentPlaceHolder1$SearchButton'
        self.values['ctl00$ContentPlaceHolder1$TBSerchWord'] = 'hadoop'
        self.values['ctl00$ContentPlaceHolder1$AspNetPager1_input'] = '1'
        self.values['__ASYNCPOST'] = 'true'
        self.values['ctl00$ContentPlaceHolder1$SearchButton'] = '检索'
        self.values['__VIEWSTATE'] = self.getViewState(soup)
        self.values['__EVENTVALIDATION'] = self.getEventValidation(soup)
        self.values['__VIEWSTATEGENERATOR'] = 'BBBC20B8'
        self.values['__EVENTTARGET'] = 'ctl00$ContentPlaceHolder1$AspNetPager1'
        self.values['__EVENTARGUMENT'] = ''
        self.values['__VIEWSTATEENCRYPTED'] = ''

    def getViewState(self, soup):
        view_input = soup.find(id="__VIEWSTATE")
        return view_input.get("value")

    def getEventValidation(self, soup):
        event_input = soup.find(id="__EVENTVALIDATION")
        return event_input.get("value")

    def hasTitle(self, tag):
        return tag.has_attr("title")

    def getPageNum(self, keyWord):
        self.values['ctl00$ContentPlaceHolder1$TBSerchWord'] = keyWord
        data = urllib.urlencode(self.values)
        try:
            request = urllib2.Request(self.url, data, self.headers)
            response = urllib2.urlopen(request)
        except urllib2.URLError, e:
            if hasattr(e, "code"):
                print e.code
            if hasattr(e, "reason"):
                print e.reason
        html = response.read()
        soup = BeautifulSoup(html, "html.parser")
        opt = soup.findAll("option")
        optlast = opt[-1].get("value")
        return int(optlast)


    def findList(self, keyWord, pagenum):
        #if pagenum is 2:
        #    del self.values['ctl00$ContentPlaceHolder1$SearchButton']
        self.values['ctl00$ContentPlaceHolder1$TBSerchWord'] = keyWord
        self.values['__EVENTARGUMENT'] = pagenum
        self.values['ctl00$ContentPlaceHolder1$AspNetPager1_input'] = pagenum - 1
        print pagenum, pagenum - 1
        data = urllib.urlencode(self.values)
        try:
            request = urllib2.Request(self.url, data, self.headers)
            response = urllib2.urlopen(request)
        except urllib2.URLError, e:
            if hasattr(e, "code"):
                print e.code
            if hasattr(e, "reason"):
                print e.reason
        html = response.read()
        #print html
        listsoup = BeautifulSoup(html, "html.parser")
        # self.values['__VIEWSTATE'] = self.getViewState(listsoup)
        # self.values['__EVENTVALIDATION'] = self.getEventValidation(listsoup)
        hreflist = listsoup.findAll(self.hasTitle)
        return hreflist

    def getInfo(self, href):
        link = 'http://210.32.205.60/' + href.get("href")
        # print link
        html = urllib2.urlopen(link).read()
        soup = BeautifulSoup(html, "html.parser")
        name = soup.find("span",id = 'ctl00_ContentPlaceHolder1_DetailsView1_Label1').get("title")
        bookid = soup.find("span", id = "ctl00_ContentPlaceHolder1_DetailsView1_Label4").get("title")
        table = soup.find(id = "ctl00_ContentPlaceHolder1_GridView1")
        tds = table.findAll("tr")
        flag = False
        isP = False
        isP2 = True
        for item in tds:
            #print item
            isP = re.search("屏峰", str(item))
            #print isP
            if isP:
                isP = False
                isP = re.search("可借", str(item))
                isP2 = re.search("不可借", str(item))
                #print isP, isP2
                if (isP) and (not isP2):
                    flag = True
                    break
        if flag is True:
            print name, bookid, "可借"
        else:
            print name, bookid, "不可借"

    def start(self):
        keyWord = raw_input("请输入要搜索的关键词")
        pagenum = self.getPageNum(keyWord);
        hreflist = []
        for i in range(1, pagenum + 1):
            hreflist[:] = []
            hreflist = self.findList(keyWord, i)
            for href in hreflist:
                self.getInfo(href)


spider = SPIDER()
spider.start()