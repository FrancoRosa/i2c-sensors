from time import sleep
from threading import Thread
import serial

ser = serial.Serial('/dev/ttyS0')

def read_uart():
  while True:
    frame = []
    while ser.in_waiting:
      c = ser.read()
      print(".",c)
    sleep(1)
  ser.close()     

Thread(target=read_uart, args=[]).start()

while True:
	ser.write(b'H\r\n')
	sleep(1)
	ser.write(b'?\r\n')
	sleep(1)

