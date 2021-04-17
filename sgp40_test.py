# from time import sleep
# from smbus import SMBus
# i2cbus = SMBus(1)

# #device address
# dev = 0x5b 

# def read_byte(reg):
#   return i2cbus.read_byte_data(dev, reg)

# def write_byte(reg, data):
#   return i2cbus.write_byte_data(dev, reg, data)

import time
import board
import busio
import adafruit_sgp40

i2c = busio.I2C(board.SCL, board.SDA)
sgp = adafruit_sgp40.SGP40(i2c, address = 0x59)

while True:
    print("Raw Gas: ", sgp.raw)
    print("")
    time.sleep(1)