#!/usr/bin/python3

import time
import socket
import argparse
import threading
import subprocess

# test data
class test_data:
    def __init__(self, data, result):
        self.data = data
        self.result = result

# test server
server = None
server_run = False
server_data = ""

# server receive method
def server_receive():
    while server_run:
        try:
            global server_data
            server_data = server.recv(1024).decode()
            print(server_data)
        except:
            # ignore all errors
            pass

if __name__ == "__main__":
    # parse program arguments
    parser = argparse.ArgumentParser(description="Utility for module version search")
    parser.add_argument("--executable", help="Executable file performing search", required=True)
    args = parser.parse_args()

    # define test cases
    test_suite = [
        test_data('./testcase/1/', '{"module1":"v1.0", "module2":"v2.2", "module3":"v3.1"}'),
        #test_data('./testcase/2/', '{"module1":"v1.0", "module2":"v2.2", "module3":"v3.1"}'),
        #test_data('./testcase/3/', '{"module1":"v1.0", "module2":"v2.2", "module3":"v3.1"}'),
    ]

    # create server
    server = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    server.bind(('127.0.0.1', 5555))
    server.settimeout(1)
    print(server.getsockname())

    # start server
    server_run = True
    server_thread = threading.Thread(target=server_receive)
    server_thread.start()

    # run test cases
    try:
        for test in test_suite:
            # execute binary under test
            process = subprocess.Popen([args.executable, test.data, server.getsockname()[0] + ":" + str(server.getsockname()[1])])
            process.wait()

            # check received data
            if server_data == test.result:
                print("OK")
            else:
                print("FAIL")
                print("expected: " + test.result)
                print("received: " + server_data)
    finally:
        # clean up
        time.sleep(5)
        server_run = False
        server_thread.join()
