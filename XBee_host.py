import serial
import matplotlib.pyplot as plt
import numpy as np
import serial
import time


# XBee setting

serdev = '/dev/ttyUSB0'

s = serial.Serial(serdev, 9600)


s.write("+++".encode())

s.write("ATMY 0x39\r\n".encode())

s.write("ATDL 0x40\r\n".encode())

s.write("ATID 0x1\r\n".encode())

s.write("ATWR\r\n".encode())

s.write("ATCN\r\n".encode())

print("start sending RPC...")

sampleNum = [0] * 20

t = np.arange(0,20,1)

i = 0

while True:

    # send RPC to remote

    s.write("/status/run\r".encode())

    line = s.read(2)

    if ('0' <= line.decode()[0] <= '1' and '0' <= line.decode()[1] <= '9'):
        sampleNum[i] = int(line.decode())
        i = i + 1
        print(i)
        print(sampleNum[i-1])


    if (i == 19):

        plt.plot(t, sampleNum, color = 'blue')

        plt.xlabel('Time')

        plt.ylabel('Sample number')

        plt.show()

        i = 0

    time.sleep(1.0)


s.close()
