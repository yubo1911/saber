# -*- coding: utf-8 -*-
from __future__ import print_function
import grpc
import echo_server_pb2
import sys


if sys.version_info.major == 3:
    raw_input = input
else:
    raw_input = raw_input


def run():
    channel = grpc.insecure_channel('localhost:50015')
    stub = echo_server_pb2.EchoServerStub(channel)
    while True:
        msg = raw_input('you say:')
        reply = stub.Echo(echo_server_pb2.EchoRequest(msg=msg))
        print(reply.msg)
    pass

if __name__ == "__main__":
    run()
