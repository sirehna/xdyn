# -*- coding: utf-8 -*-
import argparse
import random
import os

import cherrypy

from ws4py.server.cherrypyserver import WebSocketPlugin, WebSocketTool
from ws4py.websocket import WebSocket
from ws4py.messaging import TextMessage

class ChatPlugin(WebSocketPlugin):
    def __init__(self, bus):
        WebSocketPlugin.__init__(self, bus)
        self.clients = {}

    def start(self):
        WebSocketPlugin.start(self)
        self.bus.subscribe('add-client', self.add_client)
        self.bus.subscribe('get-client', self.get_client)
        self.bus.subscribe('del-client', self.del_client)

    def stop(self):
        WebSocketPlugin.stop(self)
        self.bus.unsubscribe('add-client', self.add_client)
        self.bus.unsubscribe('get-client', self.get_client)
        self.bus.unsubscribe('del-client', self.del_client)

    def add_client(self, name, websocket):
        self.clients[name] = websocket

    def get_client(self, name):
        return self.clients[name]

    def del_client(self, name):
        del self.clients[name]

class ChatWebSocketHandler(WebSocket):
    def opened(self):
        cherrypy.engine.publish('add-client', self.username, self)

    def received_message(self, m):
        text = m.data
        if text.find("@") == -1:
            # echo to all
            cherrypy.engine.publish('websocket-broadcast', m)
        else:
            # or echo to a single user
            left, message = text.rsplit(':', 1)
            print left
            from_username, to_username = left.split('@')
            print repr(from_username), repr(to_username), repr(self.username)
            client = cherrypy.engine.publish('get-client', to_username.strip()).pop()
            print client
            client.send("@@%s: %s" % (from_username.strip()[:-1], message.strip()))

    def closed(self, code, reason = "A client left the room without a proper explanation."):
        cherrypy.engine.publish('del-client', self.username)
        cherrypy.engine.publish('websocket-broadcast', TextMessage(reason))

class Root(object):
    def __init__(self, host, port, ssl = False):
        self.host = host
        self.port = port
        self.scheme = 'wss' if ssl else 'ws'

    @cherrypy.expose
    def index(self):
        return """<html>
    <head>
      <script type='application/javascript' src='js/third_party/jquery.min.js'></script>
      <script type='application/javascript'>
        $(document).ready(function() {

          websocket = '%(scheme)s://%(host)s:%(port)s/ws?username=%(username)s';
          if (window.WebSocket) {
            ws = new WebSocket(websocket, ['mytest']);
          }
          else if (window.MozWebSocket) {
            ws = MozWebSocket(websocket);
          }
          else {
            console.log('WebSocket Not Supported');
            return;
          }

          window.onbeforeunload = function(e) {
            $('#chat').val($('#chat').val() + 'Bye bye...\\n');
            ws.close(1000, '%(username)s left the room');

            if(!e) e = window.event;
            e.stopPropagation();
            e.preventDefault();
          };
          ws.onmessage = function (evt) {
             $('#chat').val($('#chat').val() + evt.data + '\\n');
          };
          ws.onopen = function() {
             ws.send("%(username)s entered the room");
          };
          ws.onclose = function(evt) {
             $('#chat').val($('#chat').val() + 'Connection closed by server: ' + evt.code + ' \"' + evt.reason + '\"\\n');
          };

          $('#send').click(function() {
             console.log($('#message').val());
             ws.send('%(username)s: ' + $('#message').val());
             $('#message').val("");
             return false;
          });
        });
      </script>
    </head>
    <body>
    <form action='#' id='chatform' method='get'>
      <textarea id='chat' cols='35' rows='10'></textarea>
      <br />
      <label for='message'>%(username)s: </label><input type='text' id='message' />
      <input id='send' type='submit' value='Send' />
      </form>
    </body>
    </html>
    """ % {'username': "User%d" % random.randint(0, 100), 'host': self.host,
           'port': self.port, 'scheme': self.scheme}

    @cherrypy.expose
    def ws(self, username):
        # let's track the username we chose
        cherrypy.request.ws_handler.username = username
        cherrypy.log("Handler created: %s" % repr(cherrypy.request.ws_handler))

if __name__ == '__main__':
    import logging
    from ws4py import configure_logger
    configure_logger(level = logging.DEBUG)

    parser = argparse.ArgumentParser(description = 'Echo CherryPy Server')
    parser.add_argument('--host', default = '127.0.0.1')
    # parser.add_argument('--host', default='130.66.124.210')
    parser.add_argument('-p', '--port', default = 9002, type = int)
    parser.add_argument('--ssl', action = 'store_true')
    args = parser.parse_args()

    cherrypy.config.update({'server.socket_host': args.host,
                            'server.socket_port': args.port,
                            'tools.staticdir.root': os.path.abspath(os.path.join(os.path.dirname(__file__), '.'))})

    if args.ssl:
        cherrypy.config.update({'server.ssl_certificate': './server.crt',
                                'server.ssl_private_key': './server.key'})

    ChatPlugin(cherrypy.engine).subscribe()
    cherrypy.tools.websocket = WebSocketTool()

    cherrypy.quickstart(Root(args.host, args.port, args.ssl), '', config = {
        '/': {'tools.proxy.on':True, },
        '/ws': {
            'tools.websocket.on': True,
            'tools.websocket.handler_cls': ChatWebSocketHandler
            },
        '/js': {
              'tools.staticdir.on': True,
              'tools.staticdir.dir': 'js'
            }
        }
    )
