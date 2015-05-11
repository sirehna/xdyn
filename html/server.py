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
import webbrowser
        
class MainHandler(tornado.web.RequestHandler):
    def get(self):
        self.render("websocket_test.html")
 
class WebSocketHandler(tornado.websocket.WebSocketHandler):
    def initialize(self):
        self.clients = []

    def on_message(self, message):  
      print("message received: " + message)
      for client in self.clients:
        client.write_message(message)

    def open(self):
      self.clients.append(self)
      self.write_message("Connected")
      print("open")
      
    def on_close(self):
      self.clients.remove(self)
      print("close")



class SimulatorGUI:
    def __init__(self, address, port):
        suffix = "main"
        self.port = port
        handlers = [
            (r"/"+suffix, MainHandler),
            (r"/",        WebSocketHandler),
        ]
        settings = {
             "static_path": os.path.join(os.path.dirname(__file__), "static"),
        }
        self.application = tornado.web.Application(handlers, **settings)
        self.url = address + ":" + str(port) + "/" + suffix

    def open_html_page_in_browser(self, url):
        # Open in a new tab, if possible
        webbrowser.open(url,new=2)

    def run(self):
        self.application.listen(self.port)
        self.mainLoop = tornado.ioloop.IOLoop.instance()
        self.open_html_page_in_browser(self.url)
        self.mainLoop.start()
        

if __name__ == "__main__":
    gui = SimulatorGUI("http://127.0.0.1", 9002)
    gui.run()

