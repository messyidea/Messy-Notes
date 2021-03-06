# -*- coding: UTF-8 -*-
import urllib
import urllib2
import json
import sys
import telepot
from telepot.delegate import per_from_id, create_open
import re
import sys
import time

reload(sys)
sys.setdefaultencoding("utf-8")

KEY = 'xxxxxxxxxxxxxxxxxxxxxxxxxx'
URL = 'http://www.tuling123.com/openapi/api'

def get_autoresponse(msg):
    values = {}
    values['key'] = KEY
    values['info'] = msg
    data = urllib.urlencode(values)
    request = urllib2.Request(URL, data)
    response = urllib2.urlopen(request)
    mess = response.read()
    decoded = json.loads(mess)
    return decoded['text']

class UserTracker(telepot.helper.UserHandler):
    def __init__(self, seed_tuple, timeout):
        super(UserTracker, self).__init__(seed_tuple, timeout)

        # keep track of how many messages of each flavor
        self._counts = {'normal': 0,
                        'inline_query': 0,
                        'chosen_inline_result': 0}

    def on_message(self, msg):
        now = time.time()
        if now - msg['date'] > 16:
            return
        rst = 0
        print msg
        print "id ====", msg['from']['id']
        #bot.sendMessage(msg['chat']['id'], 'hehe')
        flavor = telepot.flavor(msg)
        self._counts[flavor] += 1
        rst = self.on_sign(msg)
        if rst == 1:
            return
        rst = self.on_echo(msg)
        if rst == 1:
            return
        if rst == 0:
            resp = get_autoresponse(msg['text'])
            bot.sendMessage(msg['chat']['id'], resp)

    def on_echo(self, msg):
        if re.match(r'^/echo.*$', msg['text']):
            bot.sendMessage(msg['chat']['id'], msg['text'][5:])
            return 1
        return 0

    def on_sign(self, msg):
        if re.match(r'^/sign.*$', msg['text']):
            if round(msg['date']) % 7 == 0:
                bot.sendMessage(msg['chat']['id'],'拜托啦, ' + msg['from']['username'] + '! 不要老是签到哦~ 会吵到大家哒！')
            return 1
        return 0


TOKEN = 'xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx'

bot = telepot.DelegatorBot(TOKEN, [
    (per_from_id(), create_open(UserTracker, timeout=20)),
])
bot.notifyOnMessage(run_forever=True)
