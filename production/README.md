# I2C Sensors
> Read air quality sensors from RPi

## Group Test
After installing the requiremens stated on each sensor run the following command:

```bash
$ python3 group_test.py
```

## CSS811
I2C Bus Address: 0x5B

Documentation: https://itbrainpower.net/a-gsm/RaspberryPI-CCS811-sensor_howto

Requirements:

```bash
$ pip3 install smbus2
```

Test script:

```bash
$ python3 ccs811_test.py
```

## Bosh BME680 

I2C Bus Address: 0x77

Documentation: https://github.com/pimoroni/bme680-python

Requirements:

```bash
$ pip3 install bme680
```

Test script:

```bash
$ python3 ccs811_test.py
```

## Sensirion SFA30

UART port name: /dev/ttyS0 (UART available on RPI GPIO)

Documentation: https://sensirion.github.io/python-shdlc-sfa3x/

Requirements:

```bash
$ pip3 install sensirion-shdlc-sfa3x
```

Test script:

```bash
$ python3 sfa30_test.py
```

## Sensirion SGP40 

I2C Bus Address: 0x59 (disclaimer: i2cdetect wont recognize it)

Documentation: This device uses 16bit addresses, the easiest way to communicate with it is to use, circuit-python. 
https://github.com/adafruit/Adafruit_CircuitPython_Bundle
https://github.com/adafruit/Adafruit_CircuitPython_SGP40

Requirements:
Circuit python:

```bash
$ pip3 install adafruit-circuitpython-lis3dh
```
Sensor library:

```bash
$ pip3 install adafruit-circuitpython-sgp40
```

Test script:

```bash
$ python3 sgp40_test.py
```

## Logger
This script captures data in continus mode, once we stop the program with ```ctrl + c``` it creates a cvs file with all the stored data.

It can be used with 'file name' or without it.

```bash
$ python3 logger.py 'sample with fan'
```

```bash
$ python3 logger.py
```
