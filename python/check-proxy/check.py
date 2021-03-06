#!/usr/bin/env python
# coding=utf-8
import urllib2, socket

socket.setdefaulttimeout(5)

# read the list of proxy IPs in proxyList
# proxyList = ['172.30.1.1:8080', '172.30.3.3:8080'] # there are two sample proxy ip

def is_bad_proxy(pip):    
    try:        
        proxy_handler = urllib2.ProxyHandler({'http': pip})        
        opener = urllib2.build_opener(proxy_handler)
        opener.addheaders = [('User-agent', 'Mozilla/5.0')]
        urllib2.install_opener(opener)        
        req=urllib2.Request('http://www.baidu.com')  # change the url address here
        sock=urllib2.urlopen(req)
        # print sock.read()
        content=sock.read()
        # print content
        if "baidu" in content:
            return False
        else:
            return True
    except urllib2.HTTPError, e:        
        # print 'Error code: ', e.code
        return e.code
    except Exception, detail:
        # print "ERROR:", detail
        return 1
    return 0

def main():
    proxyList = []

    input_file = open("nmapout", "r")
    for line in input_file:
        proxyList.append(line)
    input_file.close()

    for item in proxyList:
        item = item[:len(item) - 1]
        # print item
        if is_bad_proxy(item):
            print "Bad Proxy", item
        else:
            print "is working", item



if __name__ == '__main__':
    main()