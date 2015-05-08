import urllib, httplib
import json
from flask.ext.babel import gettext
from config import MS_TRANSLATOR_CLIENT_ID, MS_TRANSLATOR_CLIENT_SECRET

def microsoft_translate(text, sourceLang, destLang):
    if MS_TRANSLATOR_CLIENT_ID == "" or MS_TRANSLATOR_CLIENT_SECRET == "":
        return gettext('Error: translation service not configured.')
    try:
        # get access token
        params = urllib.urlencode({
            'client_id': MS_TRANSLATOR_CLIENT_ID,
            'client_secret': MS_TRANSLATOR_CLIENT_SECRET,
            'scope': 'http://api.microsofttranslator.com',
            'grant_type': 'client_credentials'
        })
        conn = httplib.HTTPSConnection("datamarket.accesscontrol.windows.net")
        conn.request("POST", "/v2/OAuth2-13", params)
        response = json.loads (conn.getresponse().read())
        token = response[u'access_token']

        # translate
        conn = httplib.HTTPConnection('api.microsofttranslator.com')
        params = {
            'appId': 'Bearer ' + token,
            'from': sourceLang,
            'to': destLang,
            'text': text.encode("utf-8")
        }
        conn.request("GET", '/V2/Ajax.svc/Translate?' + urllib.urlencode(params))
        response = json.loads("{\"response\":" + conn.getresponse().read().decode('utf-8-sig') + "}")
        return response["response"]
    except:
        return gettext('Error: Unexpected error.')
