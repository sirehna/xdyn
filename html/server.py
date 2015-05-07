#!/usr/bin/env python
# -*- coding: utf8 -*-
#
# Copyright (c) 2013 Baptiste LABAT
#
# Licensed under the MIT License,
# https://github.com/baptistelabat/robokite
# Authors: Baptiste LABAT
#
# Used http://www.linuxforu.com/2012/04/getting-started-with-html5-websockets/
 
import tornado.web
import tornado.websocket
import tornado.ioloop
import os
import datetime
import time
import json
import math

clients = []
global t0
global ser
t0 = time.time()

        
class MainHandler(tornado.web.RequestHandler):
    def get(self):
        self.render("websocket_test.html")
 
class WebSocketHandler(tornado.websocket.WebSocketHandler):
    def on_message(self, message):  
      print("message received: " + message)
      for client in clients:
        client.write_message(message)

    def open(self):
      clients.append(self)
      self.write_message(u"Connected")
      print("open")
      
    def on_close(self):
      clients.remove(self)
      print("close")

handlers = [
    (r"/main", MainHandler),
    (r"/", WebSocketHandler),
]
settings = {
#            static_path=os.path.join(os.path.dirname(__file__), "js"),
     #"static_path": os.path.join(os.path.dirname(__file__), "js"),
     "static_path": os.path.join(os.path.dirname(__file__), "static"),
}

application = tornado.web.Application(handlers, **settings)
 
if __name__ == "__main__":
    application.listen(9002)
    mainLoop = tornado.ioloop.IOLoop.instance()
    #scheduler = tornado.ioloop.PeriodicCallback(timer, 100, io_loop = mainLoop)
    #scheduler = tornado.ioloop.PeriodicCallback(checkSerial, 1, io_loop = mainLoop)
    #scheduler.start()
    mainLoop.start()
    

