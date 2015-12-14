
from bs4 import BeautifulSoup
import urllib
import urllib2
import re
import math
import sys
import random
reload(sys)
sys.setdefaultencoding('utf-8')

def get_ip2(n):
    url = 'http://www.kuaidaili.com/free/intr/' + str(n) + '/'
    headers = {}
    headers['User-Agent'] = 'Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/46.0.2490.71 Safari/537.36'
    request = urllib2.Request(url, headers=headers)
    respond = urllib2.urlopen(request)
    html = respond.read()
    soup = BeautifulSoup(html)
    datas = soup.findAll("tr")
    datas = datas[1:]
    alldata = []
    for data in datas:
        data = data.findAll('td')
        alldata.append((data[0].string, data[1].string))
    return alldata

def get_ip(n):
    url = "http://www.xicidaili.com/nn/" + str(n)
    headers = {}
    headers['User-Agent'] = 'Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/46.0.2490.71 Safari/537.36'
    request = urllib2.Request(url, headers=headers)
    respond = urllib2.urlopen(request)
    html = respond.read()
    soup = BeautifulSoup(html)
    datas = soup.findAll('tr', class_='odd')
    alldata = []
    for data in datas:
        tds = data.findAll('td')
        list = (tds[2].string, tds[3].string, tds[6].string)
        alldata.append(list)

    return alldata
    #ttt = datas[0].findAll('td')
    #print ttt[2].string,ttt[3].string

def toupiao(datas):

    for data in datas:
        print data
        if data[2] == u'HTTPS':
            continue
        try:
            proxyConfig = 'http://%s' % (data[0] + ':' + data[1])
            print proxyConfig
            opener = urllib2.build_opener( urllib2.ProxyHandler({'http':proxyConfig}))
            #urllib2.install_opener(opener)
            inforMation = opener.open("http://1.jyxk2015.sinaapp.com/jyja/index.php", timeout=5)
            html = inforMation.read()
            #print html
            soup = BeautifulSoup(html)
            data = soup.findAll("a", href="javascript:void(0)")[2]
            print data.get('onclick')
            tttt = data.get('onclick')[14:46]
            #inforMation = urllib2.urlopen("http://1.jyxk2015.sinaapp.com/jyja/end.php?sid=0.48513295291922987&bh=3&name=%E6%A2%81%E6%9C%9D%E5%87%AF&" + "t=" + tttt)
            uuuu = "http://1.jyxk2015.sinaapp.com/jyja/end.php?sid=" + str(random.random()) + str(random.random())[2:7] + "&bh=3&name=%E6%A2%81%E6%9C%9D%E5%87%AF&" + "t=" + tttt
            print uuuu
            inforMation = opener.open(uuuu, timeout=5)
            print inforMation.read()
        except :
            continue


    """
    proxyConfig = 'http://119.253.252.27:3128'
    print proxyConfig
    #headers = {}
    #headers['User-Agent'] = 'Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/46.0.2490.71 Safari/537.36'
    #request = urllib2.Request('http://1.jyxk2015.sinaapp.com/jyja/index.php', headers=headers)
    #inforMation = urllib2.urlopen(request, proxies={'http':proxyConfig})
    opener = urllib2.build_opener( urllib2.ProxyHandler({'http':proxyConfig}))
    urllib2.install_opener(opener)
    inforMation = urllib2.urlopen("http://1.jyxk2015.sinaapp.com/jyja/index.php")
    html = inforMation.read()
    print html
    soup = BeautifulSoup(html)
    data = soup.findAll("a", href="javascript:void(0)")[2]
    print data.get('onclick')
    tttt = data.get('onclick')[14:46]
    inforMation = urllib2.urlopen("http://1.jyxk2015.sinaapp.com/jyja/end.php?sid=0.48513295291922987&bh=3&name=%E6%A2%81%E6%9C%9D%E5%87%AF&" + "t=" + tttt)
    uuuu = "http://1.jyxk2015.sinaapp.com/jyja/end.php?sid=" + str(random.random()) + str(random.random())[2:7] + "&bh=3&name=%E6%A2%81%E6%9C%9D%E5%87%AF&" + "t=" + tttt
    print uuuu
    inforMation = urllib2.urlopen(uuuu)
    print inforMation.read()
    """




for i in range(2,20,1):
    alldata = get_ip(i)
    toupiao(alldata)
#alldata = get_ip(1)
#print alldata
#print random.random()
#alldata = []
#toupiao(alldata)
