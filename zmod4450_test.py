from smbus import SMBus
i2cbus = SMBus(1)

dev1 = 0x32 #renesas zmod4450


test_add1 = 0x88
test_add2 = 0x8B
print(i2cbus.write_byte_data(dev1, test_add1, 0xff))
print(i2cbus.write_byte_data(dev1, test_add2, 0xff))
print(i2cbus.read_byte_data(dev1, test_add1))
print(i2cbus.read_byte_data(dev1, test_add2))