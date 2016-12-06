#!/usr/bin/env python
from mitmproxy import flow
from mitmproxy.script import concurrent
from netlib.http import decoded
from mitmproxy.models import HTTPResponse
from pprint import pprint
from flask import Flask
from flask import request as flask_request
import json
import base64
import traceback

poor_man_global_var = []

app = Flask("mitmproxy")

@app.route('/')
@app.route('/index')
def hello_world():
    #return json.dumps( poor_man_global_var, indent=4 )
    return 'hello, world'

@app.route('/clear')
def traffic_clear():
    global poor_man_global_var
    poor_man_global_var = []
    return json.dumps( {'status':'ok', 'result':poor_man_global_var}, indent=4)

@app.route('/record')
def traffic_record():
    global poor_man_global_var
    return json.dumps( {'status':'ok', 'result':poor_man_global_var}, indent=4)

# this domain and port combination will now be routed to the WSGI app instance.
def start(context, argv):
    context.app_registry.add(app, "mitm.com", 80)

def done ( context ):
    pprint ( poor_man_global_var )

@concurrent
def request( context, flow ):
    global poor_man_global_var
    req = flow.request
    url = req.url
    poor_man_global_var.append( { 'url':url, 'request':True, 'request_content':base64.b64encode(req.content), 'response':False} )

@concurrent
def response( context, flow ):
    req = flow.request
    res = flow.response
    url = req.url
    with decoded( res ):
        poor_man_global_var.append( 
            { 'url':url, 'request':True, 'response':True, 'response_content':base64.b64encode(res.content) } 
        )
