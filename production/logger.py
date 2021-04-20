import time

######### SFA30 RELATED CODE #################
from sensirion_shdlc_driver import ShdlcSerialPort, ShdlcConnection
from sensirion_shdlc_sfa3x import Sfa3xShdlcDevice
port = ShdlcSerialPort(port='/dev/ttyS0', baudrate=115200)
device = Sfa3xShdlcDevice(ShdlcConnection(port), slave_address=0)
device.device_reset()
print("SFA30 Device Marking: {}".format(device.get_device_marking()))
device.start_measurement()
print("SFA30 Measurement started... ")
time.sleep(1)
###########################################

######### BME680 RELATED CODE #################
import bme680
try:
    sensor = bme680.BME680(bme680.I2C_ADDR_PRIMARY)
except IOError:
    sensor = bme680.BME680(bme680.I2C_ADDR_SECONDARY)

# These calibration data can safely be commented
# out, if desired.

print('Calibration data:')
for name in dir(sensor.calibration_data):

    if not name.startswith('_'):
        value = getattr(sensor.calibration_data, name)

        if isinstance(value, int):
            print('{}: {}'.format(name, value))

# These oversampling settings can be tweaked to
# change the balance between accuracy and noise in
# the data.

sensor.set_humidity_oversample(bme680.OS_2X)
sensor.set_pressure_oversample(bme680.OS_4X)
sensor.set_temperature_oversample(bme680.OS_8X)
sensor.set_filter(bme680.FILTER_SIZE_3)
sensor.set_gas_status(bme680.ENABLE_GAS_MEAS)

print('\n\nInitial reading:')
for name in dir(sensor.data):
    value = getattr(sensor.data, name)

    if not name.startswith('_'):
        print('{}: {}'.format(name, value))

sensor.set_gas_heater_temperature(320)
sensor.set_gas_heater_duration(150)
sensor.select_gas_heater_profile(0)

###########################################

######### CCS811 RELATED CODE #################
from ccs811 import *
ccs811Begin(CCS811_driveMode_1sec)                                      
###########################################

######### SGP40 RELATED CODE #################
import board
import busio
import adafruit_sgp40

i2c = busio.I2C(board.SCL, board.SDA)
sgp = adafruit_sgp40.SGP40(i2c, address = 0x59)                                   
###########################################

############ DATA LOGGER CODE###############
from datetime import datetime
import csv
import signal
import sys
directory = '/home/pi/i2c-sensors'

class GracefulKiller:
  kill_now = False
  def __init__(self):
    signal.signal(signal.SIGINT, self.exit_gracefully)
    signal.signal(signal.SIGTERM, self.exit_gracefully)

  def exit_gracefully(self,signum, frame):
    self.kill_now = True

killer = GracefulKiller()

def timestamp():
  z = datetime.now()
  return z.strftime("%H:%M:%S")

def timestart():
  z = datetime.now()
  return z.strftime("%Y%m%d_%H:%M:%S")

try:
  filename = directory + timestart() + '_' + str(sys.argv[1]) + '.csv'
except:
  filename = directory + timestart() + '.csv'

header = [
         'timestamp', 
         'sfa_temp(°C)', 'sfa_hum(%)', 'sfa_fmdh(ppb)',
         'bme_temp(°C)', 'bme_pressure(hPa)', 'bme_hum(%)', 'bme_gas_resis(Ohms)',
         'ccs_co2(ppm)', 'ccs_tvoc(ppb)',
         'spg_raw'
         ]

values = [header]

############################################
t = 1
while not killer.kill_now:
    ################  CSV Related code  ########
    row = []
    sampletime = timestamp()
    row.append(sampletime)
    ############################################
    
    ######### SFA30 RELATED CODE #################
    try:
      hcho, humidity, temperature = device.read_measured_values()
      row.append(temperature.degrees_celsius)
      row.append(humidity.percent_rh)
      row.append(hcho.ppb)
    except:
      row.append('err')
      row.append('err')
      row.append('err')
      print('... SFA30 sensor error')
    time.sleep(0.1)
    ############################################

    ######### BME680 RELATED CODE #################
    try:
      if sensor.get_sensor_data():
        row.append(sensor.data.temperature)
        row.append(sensor.data.pressure)
        row.append(sensor.data.humidity)

        if sensor.data.heat_stable:
          row.append(sensor.data.gas_resistance)
        else:
          row.append(output)
    except:
      print('... BME680 sensor error')
      row.append('err')
      row.append('err')
      row.append('err')
      row.append('err')
    time.sleep(0.1)
    ############################################
    ######### CCS811 RELATED CODE #################
    try:
      ccs811SetEnvironmentalData(sensor.data.temperature, sensor.data.humidity)                       #replace with temperature and humidity values from HDC2010 sensor
      if ccs811CheckDataAndUpdate():
        CO2 = ccs811GetCO2()
        tVOC = ccs811GetTVOC()
        row.append(CO2)
        row.append(tVOC)
      elif ccs811CheckForError():
        ccs811PrintError()
    except:
      print('... CCS811 sensor error')
      row.append('err')
      row.append('err')
    time.sleep(0.1)
    ############################################
    
    ######### SGP40 RELATED CODE #################
    try:
        row.append(sgp.raw)
    except:
        print('... SGP40 sensor error')
        row.append('err')

    ############################################
    print('sample:', t, row)
    print('hreaders:', len(header), "values:", len(row))
    if len(row) == len(header):
      values.append(row)
    t = t + 1
    time.sleep(2)


print('\n\n\n... creating file >> %s'%(filename))
with open(filename, 'w', newline='') as f:
  writer = csv.writer(f)
  writer.writerows(values)
print('... file created.')
print("Have a nice day :)")
