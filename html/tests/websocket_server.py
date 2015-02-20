#!/usr/bin/env python

import socket, struct, hashlib, threading, cgi

def decode_key (key):
	num = ""
	spaces = 0
	for c in key:
		if c.isdigit():
			num += c
	 	if c.isspace():
			spaces += 1
	return int(num) / spaces

def create_hash (key1, key2, code):
	a = struct.pack(">L", decode_key(key1))
	b = struct.pack(">L", decode_key(key2))
	md5 = hashlib.md5(a + b + code)
	return md5.digest()

def recv_data (client, length):
	data = client.recv(length)
	if not data: return data
	return data.decode('utf-8', 'ignore')

def send_data (client, data):
	message = "\x00%s\xFF" % data.encode('utf-8')
	return client.send(message)

def parse_headers (data):
	headers = {}
	lines = data.splitlines()
	for l in lines:
		parts = l.split(": ", 1)
		if len(parts) == 2:
			headers[parts[0]] = parts[1]
	headers['code'] = lines[len(lines) - 1]
	return headers

def handshake (client):
	print 'Handshaking...'
	data = client.recv(1024)
	headers = parse_headers(data)
	print 'Got headers:'
	for k, v in headers.iteritems():
		print k, ':', v
	digest = create_hash(
		headers['Sec-WebSocket-Key1'],
		headers['Sec-WebSocket-Key2'],
		headers['code']
	)
	shake = "HTTP/1.1 101 Web Socket Protocol Handshake\r\n"
 	shake += "Upgrade: WebSocket\r\n" 
	shake += "Connection: Upgrade\r\n"
	shake += "Sec-WebSocket-Origin: %s\r\n" % (headers['Origin'])
	shake += "Sec-WebSocket-Location: ws://%s/stuff\r\n" % (headers['Host'])
	shake += "Sec-WebSocket-Protocol: sample\r\n\r\n"
	shake += digest
	return client.send(shake)

def handle (client, addr):
	handshake(client)
	#lock = threading.Lock()
	while 1:
		data = recv_data(client, 1024)
		if not data: break
		data = cgi.escape(data)
		#lock.acquire()
		[send_data(c, data) for c in clients]
		#lock.release()
	print 'Client closed:', addr
	#lock.acquire()
	clients.remove(client)
	#lock.release()
	client.close()
	
def start_server ():
	s = socket.socket()
	s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
	s.bind(('', 9876))
	s.listen(5)
	while 1:
		conn, addr = s.accept()
		print 'Connection from:', addr
		clients.append(conn)
		#threading.Thread(target = handle, args = (conn, addr)).start()

clients = []
start_server()
