from time import sleep
from threading import Thread
import serial

ser = serial.Serial('/dev/ttyUSBfan', timeout=1)

def send_command(text):
  ser.write(b'%s\r\n'%text.encode())

def fan_on():
	send_command('E')

def fan_off():
	send_command('D')

def fan_speed(speed):
  max_speed = 100
  min_speed = 0
  motor = '0'
  val = int(speed)
  if (speed > max_speed):
    val = max_speed
  if (speed < min_speed):
    val = max_speed
  send_command('M%sF%d'%(motor, val))

fan_on()
fan_speed(0)