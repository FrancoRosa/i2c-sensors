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

def read_byte(reg):
  return i2cbus.read_byte_data(dev, reg)

def read_bytes(reg, length):
  result = []
  for i in range(length):
    result.append(read_byte(reg + i))
  
  return result

def read_status():
  result = read_byte(ZMOD44XX_ADDR_STATUS)
  print('STATUS:', result)
  
def read_sensor_info():
  pid = read_bytes(ZMOD44XX_ADDR_PID, ZMOD44XX_LEN_PID)
  print('PID:', pid)
  config = read_bytes(ZMOD44XX_ADDR_CONF, ZMOD44XX_LEN_CONF)
  print('CONF:', config)
  product = read_bytes(ZMOD44XX_ADDR_PROD_DATA, ZMOD44XX_LEN_PROD_DATA)
  print('PROD_DATA:', product)
  hspf = (0-(config[2] * 256.0 + config[3]) * ((config[4] + 640.0) * (config[5] + 80.0) - 512000.0)) / 12288000.0;
  print('hspf:', hspf)

def init_sensor():
  result = read_byte(ZMOD44XX_ADDR_PREPARE)

print('...start script')
read_status()
read_sensor_info()
# print(i2cbus.write_byte_data(dev1, test_add1, 0xff))
# print(i2cbus.write_byte_data(dev1, test_add2, 0xff))
# print(i2cbus.read_byte_data(dev1, test_add1))
# print(i2cbus.read_byte_data(dev1, test_add2))