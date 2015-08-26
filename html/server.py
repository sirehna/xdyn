#!/usr/bin/env python

import os
import sys
import tornado.web
import tornado.websocket
import tornado.ioloop
import types
import uuid
import webbrowser
from subprocess import call

def get_ip():
    import socket
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        # doesn't even have to be reachable
        s.connect(('10.255.255.255', 0))
        IP = s.getsockname()[0]
    except:
        IP = '127.0.0.1'
    finally:
        s.close()
    return IP

class ClientTracker:
    def __init__(self, should_log):
        self.clients = []
        self.clients_id = {}
        self.should_log = should_log

    def broadcast(self, message):
        client_idx = 0
        for client in self.clients:
            client_idx += 1
            if (self.should_log):
                print("Broadcasting to client " + str(client_idx))
            client.write_message(message)

    def add(self, client):
        self.clients.append(client)
        self.clients_id[client] = len(self.clients)
        if (self.should_log):
            print("Connecting client " + str(len(self.clients)))

    def remove(self, client):
        self.clients.remove(client)
        if (self.should_log):
            print("Closing client " + str(self.clients_id[client]))
        self.clients_id.pop(client, None)


class MainHandler(tornado.web.RequestHandler):
    def get(self):
        self.render("websocket_test.html")

    def upload_file(self, key):
        if key in self.request.files:
            fileinfo = self.request.files[key][0]
            __UPLOADS__ = "uploads/"
            fname = fileinfo['filename']
            extn = os.path.splitext(fname)[1]
            filename = __UPLOADS__ + str(uuid.uuid4()) + extn
            fh = open(filename, 'w')
            fh.write(fileinfo['body'].decode('utf-8'))
            return filename
        else:
            return ""

    def get_checkbox(self, key):
        if key in self.request.arguments:
            return self.request.arguments[key]
        return False

    def get_form_contents(self):
        form = types.SimpleNamespace()
        form.yaml   = self.upload_file('yaml_file')
        form.stl    = self.upload_file('stl_file')
        form.solver = self.get_body_argument('solver')
        form.dt     = float(self.get_body_argument('dt'))
        form.T      = float(self.get_body_argument('T'))
        form.csv    = self.get_checkbox('csv')
        form.tsv    = self.get_checkbox('tsv')
        form.hdf5   = self.get_checkbox('hdf5')
        return form

    def build_command_line(self, form):
        out = ['./sim', '--dt ' + str(form.dt), '--tend ' + str(form.T), '-s ' + form.solver]
        if form.csv:
            out.append('-o ' + str(uuid.uuid4()) + '.csv')
        if form.tsv:
            out.append('-o ' + str(uuid.uuid4()) + '.tsv')
        if form.hdf5:
            out.append('-o ' + str(uuid.uuid4()) + '.h5')
        return out

    def post(self):
        form = self.get_form_contents()
        print(form)
        print(self.build_command_line(form))
        self.render("websocket_test.html")




class WebSocketHandler(tornado.websocket.WebSocketHandler):
    def check_origin(self, origin):
        return True

    def initialize(self, client_tracker):
        print("Initializing websocket handler")
        self.client_tracker = client_tracker

    def on_message(self, message):
        print("message received: " + message)
        self.client_tracker.broadcast(message)

    def open(self):
        self.client_tracker.add(self)

    def on_close(self):
        self.client_tracker.remove(self)


class SimulatorGUI:
    def __init__(self, address, port):
        self.client_tracker = ClientTracker(True)
        suffix = "main"
        self.port = port
        if getattr(sys, 'frozen', False):
            # frozen
            dir_ = os.path.dirname(sys.executable)
        else:
            # unfrozen
            dir_ = os.path.dirname(__file__)
        handlers = [
            (r"/" + suffix, MainHandler),
            (r"/", WebSocketHandler, dict(client_tracker = self.client_tracker)),
        ]
        settings = {
             "static_path": os.path.join(os.path.dirname(dir_), "static"),
        }
        self.application = tornado.web.Application(handlers, **settings)
        self.url = address + ":" + str(port) + "/" + suffix

    def open_html_page_in_browser(self, url):
        # Open in a new tab, if possible
        webbrowser.open(url, new = 2)

    def run(self):
        self.application.listen(self.port)
        self.mainLoop = tornado.ioloop.IOLoop.instance()
        # self.open_html_page_in_browser(self.url)
        print("Initialized: now starting main server loop.")
        self.mainLoop.start()

if __name__ == "__main__":
    import argparse
    from os.path import basename
    if sys.version_info.major < 3:
        sys.exit("Python server should be run with Python 3")
    progName = basename(sys.argv[0])
    parser = argparse.ArgumentParser(\
        prog = progName, \
        description = "Creating server\n")
    parser.add_argument("-a", "--address", \
        help = "IP. For example localhost. By default, current IP is used", \
        default = 'http://' + get_ip())
    parser.add_argument("-p", "--port", \
        help = "Port to open. Default value is 9002", default = 9002)
    args = parser.parse_args()
    gui = SimulatorGUI(address = args.address, port = args.port)
    gui.run()
