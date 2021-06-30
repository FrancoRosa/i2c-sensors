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

* Set the /dev/ttyS0 as a UART device with ```raspi-config```, go to _Interface Options_ then _Serial port_, there disable the serial shell, then enable the hardware serial port. If set correctly it should ask for a reboot (reboot is required only once) 

```bash
$ sudo raspi-config
```

* Install libraries

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

## Fan

The fan controller will be connected to the PI using a ttl-USB adapter, in order to have that resource available as '/dev/ttyUSBFan' it is required to do the following configuration 
Append the new rules on `/etc/udev/rules.d/99-com.rules` 

1. Add a rule that maps the USB-ttl adapter to the specific product and vendor.
  + Append the following line to `99-com.rules`

```BASH
SUBSYSTEM=="tty", ATTRS{idVendor}=="0403", ATTRS{idProduct}=="6001", SYMLINK+="ttyUSBfan"
```

    

```BASH
sudo nano /etc/udev/rules.d/99-com.rules 
```

2. Reload the udev rules with the following commands:

```BASH
sudo udevadm control -R
```

3. Connect and disconnect the USB-ttl adapter or reboot to see a new `/dev/ttyUSBFan` device.

The fan.py file is a module to be called from other scripts

```Python
from time import sleep
from fan import *

fan_on()
fan_speed(50)
sleep(10)
fan_off()
```

## Logger

This script captures data in continus mode, once we stop the program with ` ` ` ctrl + c ` ` ` it creates a cvs file with all the stored data.

It can be used with 'file name' or without it.

```bash
$ python3 logger.py 'sample with fan'
```

```bash
$ python3 logger.py
```

## Remote Server

root@remoteserver.com password

## Steps to set SSH tunnels with this server

### On Master or on the RemoteServer

```bash
sudo nano /etc/ssh/sshd_config
```

add the following line:

```bash
GatewayPorts yes
```

### On Slave or on the Raspberries

```bash
ssh-keygen -t rsa
ssh-copy-id -i ~/.ssh/id_rsa.pub root@remoteserver.com

#After installing autossh (sudo apt install autossh)
autossh -M 21001 -fN                                           \
-o "PubkeyAuthentication=yes" -o "StrictHostKeyChecking=false" \
-o "PasswordAuthentication=no" -o "ServerAliveInterval 60"     \
-o "ServerAliveCountMax 3"                                     \
-R healthrecord.in:20001:localhost:22 -i                       \
/home/pi/.ssh/id_rsa root@remoteserver.com
