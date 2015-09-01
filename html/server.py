#!/usr/bin/env python

import json
import os
import sys
import tornado.web
import tornado.websocket
import tornado.ioloop
import types
import uuid
import webbrowser
import subprocess
import yaml

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


def modify_stl(tree, stl_from_form):
    if 'bodies' in tree:
       if 'mesh' in tree['bodies'][0]: 
            original_mesh_file = tree['bodies'][0]['mesh']
            if original_mesh_file and stl_from_form:
                tree['bodies'][0]['mesh'] = stl_from_form
    return tree;

def make_output_section(list_of_things_to_output, type_of_output):
    return {'format': type_of_output,
            'filename': 'out.' + type_of_output,
            'data': ['t'] + list_of_things_to_output}

def make_ws_output_section(list_of_things_to_output, address, port):
    return {'format': 'ws',
            'address': address,
            'port': port,
            'data': ['t'] + list_of_things_to_output}

def personalize(states, body):
    L = []
    for state in states:
        L.append(state + '(' + body + ')')
    return L

def make_list_of_things_to_output(body, outputs):
    L = []
    output2states = {
            'position': ['x', 'y', 'z'],
            'attitude': ['phi', 'theta', 'psi'],
            'speed': ['u', 'v', 'w'],
            'quaternions': ['qr', 'qi', 'qj', 'qk'],
            'rotational-speed': ['p', 'q', 'r']
            }
    for output in outputs:
        L.extend(personalize(output2states[output], body))
    return L
        
def make_list_of_things_to_output_for_all_bodies(bodies, outputs):
    L = []
    for body in bodies:
        L.extend(make_list_of_things_to_output(body, outputs))
    return L

def get_body_names(tree):
    names = []
    if 'bodies' in tree:
        names.append(tree['bodies'][0]['name'])
    return names

def modify_outputs(tree, outputs, address, port):
    bodies = get_body_names(tree)
    list_of_things_to_output = make_list_of_things_to_output_for_all_bodies(bodies, outputs)
    output_section = [make_ws_output_section(list_of_things_to_output, address, port)]
    everything = make_list_of_things_to_output_for_all_bodies(bodies, ['position', 'attitude', 'speed', 'quaternions', 'rotational-speed'])
    output_section.append(make_output_section(everything, 'hdf5'))
    output_section.append(make_output_section(everything, 'csv'))
    tree['output'] = output_section
    return tree

def modify_yaml(form, address, port):
    with open(form.yaml, 'r') as stream:
        tree = yaml.load(stream)
    modified_yaml_file = open(form.yaml, 'w')
    tree = modify_stl(tree, form.stl)
    tree = modify_outputs(tree, form.outputs, address, port)
    yaml.dump(tree, modified_yaml_file, default_flow_style=False)


class MainHandler(tornado.web.RequestHandler):
    def initialize(self, main_url, port):
        self.port = port
        self.websocket_url = main_url.replace("http", "ws") + ":" + str(port)
        self.main_url = main_url

    def get(self):
        self.render("websocket_test.html", websocket_address=self.websocket_url)

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
        form.yaml    = self.upload_file('yaml_file')
        form.stl     = self.upload_file('stl_file') 
        form.solver  = self.get_body_argument('solver')
        form.outputs = filter(None,self.get_body_argument('outputs').split(','))
        form.dt      = float(self.get_body_argument('dt'))
        form.T       = float(self.get_body_argument('T'))
        form.csv     = self.get_checkbox('csv')
        form.tsv     = self.get_checkbox('tsv')
        form.hdf5    = self.get_checkbox('hdf5')
        return form

    def build_command_line(self, form):
        out = ['sim', str(form.yaml), '--dt', str(form.dt),  '--tend', str(form.T), '-s', form.solver]
        return out

    def post(self):
        form = self.get_form_contents()
        modify_yaml(form, self.main_url, self.port)
        command_line = self.build_command_line(form)
        proc = subprocess.Popen(command_line)

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
            (r"/" + suffix, MainHandler, dict(main_url = address, port = port)),
            (r"/", WebSocketHandler, dict(client_tracker = self.client_tracker)),
            (r'/outputs/(.*)', tornado.web.StaticFileHandler, {'path': dir_})
        ]
        settings = {
             "static_path": os.path.join(os.path.dirname(dir_), "static"),
        }
        self.application = tornado.web.Application(handlers, **settings)

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
