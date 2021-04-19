import smbus
import time
bus = smbus.SMBus(1)
address = 0x32
register = 0x88

def read_zmod4450():
    zmod4450 = bus.read_byte_data(address, register)
    return zmod4450

def write_zmod4450(val):
    zmod4450 = bus.write_byte_data(address, register, val)

# def bearing3599():
#         bear1 = bus.read_byte_data(address, 2)
#         bear2 = bus.read_byte_data(address, 3)
#         bear = (bear1 << 8) + bear2
#         bear = bear/10.0
#         return bear

t = 0
while True:
    t = t + 1
    write_zmod4450(t)
    time.sleep(1)
    print(read_zmod4450())
    time.sleep(1)