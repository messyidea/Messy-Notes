#!/usr/bin/env python
# coding=utf-8
import urllib2, socket
from multiprocessing import Pool

socket.setdefaulttimeout(5)

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
            print "is working", pip
            return False
        else:
            print "Bad Proxy", pip
            return True
    except urllib2.HTTPError, e:        
        # print 'Error code: ', e.code
        print "Bad Proxy", pip
        return e.code
    except Exception, detail:
        # print "ERROR:", detail
        print "Bad Proxy", pip
        return 1
    return 0

def main():
    proxyList = []

    input_file = open("nmapout", "r")
    for line in input_file:
        proxyList.append(line)
    input_file.close()

    pool = Pool(processes=10)

    for item in proxyList:
        item = item[:len(item) - 1]
        pool.apply_async(is_bad_proxy, (item,))
    pool.close()
    pool.join()
    print "finished"



if __name__ == '__main__':
    main()