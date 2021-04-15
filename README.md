# I2C Sensors
>> Read sensors from RPi

## Enable circuit python
https://github.com/adafruit/Adafruit_CircuitPython_Bundle
```
pip3 install adafruit-circuitpython-lis3dh
```

## CSS811
Address: 0x5B
https://itbrainpower.net/a-gsm/RaspberryPI-CCS811-sensor_howto

## Bosh BME680 
Address: 0x77
https://github.com/pimoroni/bme680-python
```
sudo pip3 install bme680
```

## Sensirion SFA30
Address: 0x5D (Not available)
UART: /dev/ttyS0 (UART available on RPI GPIO)

https://sensirion.github.io/python-shdlc-sfa3x/
```
pip install sensirion-shdlc-sfa3x
```

## Renesas ZMOD4450
Address: 0x32
https://github.com/sparkfun/SparkFun_Refrigeration_Gas_Sensor_ZMOD4450_Qwiic


3AiRemedy