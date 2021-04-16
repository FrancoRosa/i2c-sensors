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
  'd': {'addr': 0x50, 'len': 4, 'data': data_set_4450_continuous[0:4]},
  'm': {'addr': 0x60, 'len': 1, 'data': data_set_4450_continuous[4:5]},
  's': {'addr': 0x68, 'len': 4, 'data': data_set_4450_continuous[5:9]},
  'r': {'addr': 0x99, 'len': 2}
}

zmod44xxi = {
  'start': 0x80,
  'h': {'addr': 0x40, 'len': 2},
  'd': {'addr': 0x50, 'len': 2, 'data': data_set_4450i[0:2]},
  'm': {'addr': 0x60, 'len': 2, 'data': data_set_4450i[2:4]},
  's': {'addr': 0x68, 'len': 4, 'data': data_set_4450i[4:8]},
  'r': {'addr': 0x97, 'len': 4}
}

# continuos mode
init_conf = zmod44xxi
meas_conf = zmod4450_continuous

hspf = 0
pid = 0
config = 0
product = 0
mox_lr = 0
mox_er = 0

data = [0,0,0,0]

def read_byte(reg):
  return i2cbus.read_byte_data(dev, reg)

def write_byte(reg, data):
  return i2cbus.write_byte_data(dev, reg, data)

def write_bytes(reg, data, length):
  for i in range(length):
    write_byte(reg + i, data[i])

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
  global pid, config, product
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
  
def init_sensor():
  global hspf, data, mox_lr, mox_er

  result = read_byte(ZMOD44XX_ADDR_PREPARE)
  print('PREPARE:', result)
  
  hspf = (0-(config[2] * 256.0 + config[3]) * ((config[4] + 640.0) * (config[5] + 80.0) - 512000.0)) / 12288000.0;
  print('hspf:', hspf)
  if (0.0 > hspf) or (4096.0 < hspf):
    print('... init out of range')
  
  data[0] = int(hspf) >> 8
  data[1] = int(hspf) & 0x00FF

  write_bytes(init_conf['h']['addr'], data, init_conf['h']['len'])
  write_bytes(init_conf['d']['addr'], init_conf['d']['data'], init_conf['d']['len'])
  write_bytes(init_conf['m']['addr'], init_conf['m']['data'], init_conf['m']['len'])
  write_bytes(init_conf['s']['addr'], init_conf['s']['data'], init_conf['s']['len'])
  write_byte(ZMOD44XX_ADDR_CMD, init_conf['start'])

  sleep(0.02)

  read_status()
  
  result = read_bytes(init_conf['r']['addr'], init_conf['r']['len'])
  mox_lr = result[0] << 8 | result[1]
  mox_er = result[2] << 8 | result[3]

def init_measurement():
  global data
  result = read_byte(ZMOD44XX_ADDR_PREPARE)
  print('PREPARE:',result)
  hspf = (-(config[2] * 256.0 + config[3]) *
                 ((config[4] + 640.0) * (config[5] - 600.0) - 512000.0)) / 12288000.0;

  data[0] = int(hspf) >> 8
  data[1] = int(hspf) & 0x00FF
  write_bytes(meas_conf['h']['addr'], data, meas_conf['h']['len'])
  write_bytes(meas_conf['d']['addr'], meas_conf['d']['data'], meas_conf['d']['len'])
  write_bytes(meas_conf['m']['addr'], meas_conf['m']['data'], meas_conf['m']['len'])
  write_bytes(meas_conf['s']['addr'], meas_conf['s']['data'], meas_conf['s']['len'])


print('_______________')
read_status()
read_sensor_info()
init_sensor()
init_measurement()
print('_______________')

# print(i2cbus.write_byte_data(dev1, test_add1, 0xff))
# print(i2cbus.write_byte_data(dev1, test_add2, 0xff))
# print(i2cbus.read_byte_data(dev1, test_add1))
# print(i2cbus.read_byte_data(dev1, test_add2))
# print(i2cbus.read_byte_data(0x59, 0x26))