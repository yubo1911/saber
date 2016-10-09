# -*- coding: utf-8 -*-
from concurrent import futures
import grpc
import time
import echo_server_pb2


class EchoServer(echo_server_pb2.EchoServerServicer):
    def Echo(self, request, context):
        return echo_server_pb2.EchoReply(msg='echo:%s' % request.msg)


def serve():
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    echo_server_pb2.add_EchoServerServicer_to_server(EchoServer(), server)
    server.add_insecure_port('[::]:50015')
    server.start()
    try:
        while True:
            time.sleep(60*60)
    except KeyboardInterrupt:
        server.stop(0)

if __name__ == "__main__":
    serve()
