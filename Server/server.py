from ctypes import sizeof
from email.headerregistry import Address
import socket
import struct
import time


class InvalidPacketSize(Exception):
    pass

class DosAttemptDetected(Exception):
    pass

class InvalidAuthField(Exception):
    pass

class Player:

    def __init__(self, signature, cport):
        self.signature = signature
        self.x = 0
        self.y = 0
        self.last_update = None
        self.auth = None
        self.address = (signature[0], cport[0])
        self.last_update = time.time()
        print('new player', self.address)

    def update(self, auth, x, y):
        self.x = x
        self.y = y
        if self.auth is None:
            self.auth = auth
            #print('auth for {0} is {1}', self.signature, self.auth)
        elif self.auth != auth:
            raise InvalidAuthField()
        self.last_update = time.time()
        #print(self.signature, self.x, self.y)


class Server:

    def __init__(self, address='127.0.0.1', port=9999, tolerance=660):
        self.address = address
        self.port = port
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.socket.bind((address, port))
        self.players = {}
        self.tolerance = tolerance

    def run_once(self):
        try:
            packet, sender = self.socket.recvfrom(64)
            if len(packet) != 12 and len(packet) != 4:
                raise InvalidPacketSize()

            if sender in self.players:
                if self.players[sender] is None:  # banned?
                    return
                now = time.time()
                if now - self.players[sender].last_update < (1 / self.tolerance):
                    raise DosAttemptDetected()
                auth, x, y = struct.unpack('Iff', packet)
                self.players[sender].update(auth, x, y)
                self.broadcast(sender, auth, x, y)
            else:
                cport = struct.unpack('I',packet) 
                self.players[sender] = Player(sender, cport)
                self.broadcast_handshake(sender, 1)
            self.check_dead_clients()
            #print(self.players)
        except DosAttemptDetected:
            print('Dos detected from {0}, kicking it out'.format(sender))
            #del(self.players[sender])
            self.players[sender] = None  # banned!
        except InvalidAuthField:
            print('Invalid packet auth detected from {0}'.format(sender))
        except InvalidPacketSize:
            print('Invalid packet size detected from {0}'.format(sender))
        except OSError:
            import sys
            print(sys.exc_info())
            print('packet discarded')

    def broadcast(self, sender, auth, x, y):
        for signature in self.players:
            if self.players[signature] is not None:
                if self.players[signature].auth != auth:  # avoid loop
                    self.socket.sendto(struct.pack(
                        'Iff', auth, x, y), self.players.get(signature).address)
                    
    def broadcast_handshake(self, sender, handshake):
        self.socket.sendto(struct.pack(
            'I', handshake), self.players.get(sender).address)

    def check_dead_clients(self):
        dead_clients = []
        now = time.time()
        for signature in self.players:
            if self.players[signature] is not None:
                if self.players[signature].last_update + 10 < now:
                    dead_clients.append(signature)

        for dead_client in dead_clients:
            print('{0} is dead, removing from the list of players'.format(dead_client))
            del(self.players[dead_client])

    def run(self):
        print('running Bomberman server...')
        while True:
            self.run_once()

if __name__ == '__main__':
    server = Server()
    server.run()
