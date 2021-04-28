from time import sleep
import spidev
from gpiozero import LED


cs = LED(16)
spi_ch = 0

# Enable SPI
spi = spidev.SpiDev(0, spi_ch)
spi.max_speed_hz = 500000
cs.on()
sleep(0.1)
while True:
    msg = [0x04, 0xA9]
    reply = spi.xfer2(msg)

    print(reply)
    sleep(1)
