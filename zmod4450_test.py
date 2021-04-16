from time import sleep
from smbus import SMBus
i2cbus = SMBus(1)

#device address
dev = 0x32            #renesas zmod4450

#device registers on library
ZMOD4450_I2C_ADDRESS    = 0x32
ZMOD44XX_ADDR_PID       = 0x00
ZMOD44XX_ADDR_CONF      = 0x20
ZMOD44XX_ADDR_PROD_DATA = 0x26
ZMOD44XX_ADDR_CMD       = 0x93
ZMOD44XX_ADDR_STATUS    = 0x94

#device registers resque
ZMOD44XX_ADDR_PREPARE   = 0xB7


ZMOD44XX_LEN_PID        = 2
ZMOD44XX_LEN_CONF       = 6
ZMOD44XX_LEN_PROD_DATA  = 5

STATUS_SEQUENCER_RUNNING_MASK   = 0x80 # Sequencer is running */
STATUS_SLEEP_TIMER_ENABLED_MASK = 0x40 # SleepTimer_enabled */
STATUS_ALARM_MASK               = 0x20 # Alarm */
STATUS_LAST_SEQ_STEP_MASK       = 0x1F # Last executed sequencer step */
STATUS_POR_EVENT_MASK           = 0x80 # POR_event */
STATUS_ACCESS_CONFLICT_MASK     = 0x40 # AccessConflict */

#device configuration defaults
data_set_4450_continuous = [ 0x20, 0x04, 0x40, 0x09,
                             0x03, 0x00, 0x00, 0x80, 0x08 ]

data_set_4450i = [0x00, 0x28, 0xC3, 0xE3,
                  0x00, 0x00, 0x80, 0x40 ]

zmod4450_continuous = {
  'start': 0xC0,
  'h': {'addr': 0x40, 'len': 2},
  'd': {'addr': 0x50, 'len': 4, 'data': data_set_4450_continuous[0]},
  'm': {'addr': 0x60, 'len': 1, 'data': data_set_4450_continuous[4]},
  's': {'addr': 0x68, 'len': 4, 'data': data_set_4450_continuous[5]},
  'r': {'addr': 0x99, 'len': 2}
}

zmod44xxi = {
  'start': 0x80,
  'h': {'addr': 0x40, 'len': 2},
  'd': {'addr': 0x50, 'len': 2, 'data': data_set_4450i[0]},
  'm': {'addr': 0x60, 'len': 2, 'data': data_set_4450i[2]},
  's': {'addr': 0x68, 'len': 4, 'data': data_set_4450i[4]},
  'r': {'addr': 0x97, 'len': 4}
}

# continuos mode
init_conf = zmod44xxi
meas_conf = zmod4450_continuous

def read_byte(reg):
  return i2cbus.read_byte_data(dev, reg)

def write_byte(reg, data):
  return i2cbus.write_byte_data(dev, reg, data)

def write_bytes(reg, data, length):
  for i in range(length):
    result.append(write_byte(reg + i, data[i]))

def read_bytes(reg, length):
  result = []
  for i in range(length):
    result.append(read_byte(reg + i))
  return result

def print_bytes(arr):
  print(list(map(hex,arr)))

def read_status():
  result = read_byte(ZMOD44XX_ADDR_STATUS)
  print('STATUS:', result)
  
def read_sensor_info():
  print(write_byte(ZMOD44XX_ADDR_CMD, 0))
  sleep(0.2)
  pid = read_bytes(ZMOD44XX_ADDR_PID, ZMOD44XX_LEN_PID)
  print('PID:')
  print_bytes(pid)
  
  config = read_bytes(ZMOD44XX_ADDR_CONF, ZMOD44XX_LEN_CONF)
  print('CONF:')
  print_bytes(config)
  
  product = read_bytes(ZMOD44XX_ADDR_PROD_DATA, ZMOD44XX_LEN_PROD_DATA)
  print('PROD_DATA:')
  print_bytes(product)
  
  hspf = (0-(config[2] * 256.0 + config[3]) * ((config[4] + 640.0) * (config[5] + 80.0) - 512000.0)) / 12288000.0;
  print('hspf:', hspf)
  if (0.0 > hspf) or (4096.0 < hspf):
    print('... init out of range')
  
def init_sensor():
  result = read_byte(ZMOD44XX_ADDR_PREPARE)
  print('PREPARE:', result)

print('...start script')
print('_______________')
read_status()
read_sensor_info()
# init_sensor()
print('_______________')
print('...end script')

# print(i2cbus.write_byte_data(dev1, test_add1, 0xff))
# print(i2cbus.write_byte_data(dev1, test_add2, 0xff))
# print(i2cbus.read_byte_data(dev1, test_add1))
# print(i2cbus.read_byte_data(dev1, test_add2))
# print(i2cbus.read_byte_data(0x59, 0x26))