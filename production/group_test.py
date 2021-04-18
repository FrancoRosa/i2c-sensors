import time

temperature = 0
humidity = 0

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

while True:
    
    ######### SFA30 RELATED CODE #################
    hcho, humidity, temperature = device.read_measured_values()
    print("__________________________")
    print("\n___    SFA30 VALUES    ___\n")
    print("{}, {}, {}".format(hcho, humidity, temperature))
    ############################################

    ######### BME680 RELATED CODE #################
    if sensor.get_sensor_data():
            print("\n___    BME680 VALUES    ___\n")
            output = '{0:.2f} C,{1:.2f} hPa,{2:.2f} %RH'.format(
                sensor.data.temperature,
                sensor.data.pressure,
                sensor.data.humidity)

            if sensor.data.heat_stable:
                print('{0},{1} Ohms'.format(
                    output,
                    sensor.data.gas_resistance))

            else:
                print(output)
    ############################################
    
    ######### CCS811 RELATED CODE #################
    ccs811SetEnvironmentalData(sensor.data.temperature, sensor.data.humidity)                       #replace with temperature and humidity values from HDC2010 sensor

    if ccs811CheckDataAndUpdate():
            CO2 = ccs811GetCO2()
            tVOC = ccs811GetTVOC()
            print("\n___    CCS811 VALUES    ___\n")
            print("CO2 : %d ppm" %CO2)
            print("tVOC : %d ppb" %tVOC)
    elif ccs811CheckForError():
            ccs811PrintError()

    ############################################
    
    ######### CCS811 RELATED CODE #################
    print("\n___    SGP40 VALUES    ___\n")
    print("Raw Gas: ", sgp.raw)
    
    ############################################

    print('\n\n\n')
    time.sleep(2)