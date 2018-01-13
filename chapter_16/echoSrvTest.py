import sys
import threading
import socket
import random
import time


def echoCli(num, count):
    time.sleep(random.randint(1, 10) / 100)
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((sys.argv[1], int(sys.argv[2])))
    print('echo client %d start' % num)
    for i in range(count):
        data = 'a' * 1024
        sock.send(data.encode('utf-8'))
        sock.recv(4096)
    print('echo client %d finish' % num)
    sock.close()


if __name__ == '__main__':
    if len(sys.argv) < 5:
        print('echoSrvTest ip port threadNum count')
        sys.exit(-1)
    threadNum = int(sys.argv[3])
    count = int(sys.argv[4])
    threads = []
    for i in range(threadNum):
        t = threading.Thread(target=echoCli, args=(i, count))
        t.start()
        threads.append(t)

    for t in threads:
        t.join()
