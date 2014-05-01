#!/usr/bin/env python3
import asyncio
import io

msg = b"HTTP/1.0 200 OK\r\nContent-Length: 5\r\n\r\nPong!\r\n"


class HTTPServer(asyncio.Protocol):

    def connection_made(self, transport):
        self.transport = transport

    def data_received(self, data):
        # throw away data
        self.transport.write(msg)
        self.transport.close()


def run(host, port):
    loop = asyncio.get_event_loop()
    coroutine = loop.create_server(HTTPServer, host, port)
    server = loop.run_until_complete(coroutine)
    loop.run_forever()


if __name__ == '__main__':
    run('localhost', '8000')
