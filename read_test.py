# Raw reads from i2c devices
# from smbus import SMBus
# i2cbus = SMBus(1)

# dev1 = 0x32 #bosh ME680
# dev2 = 0x77


#bosh ME680
# temp_msb = 0x22
# temp_lsb = 0x23
# temp_xlsb = 0x24

# hum_msb = 0x25
# hum_msb = 0x26

# print(i2cbus.read_byte_data(dev1, temp_msb))
# print(i2cbus.read_byte_data(dev1, temp_lsb))
# print(i2cbus.read_byte_data(dev2, 0x01))
from gpiozero import LED
from time import sleep

led = LED(23)

while True:
    print('.. set to low')
    led.off()
    sleep(1)