## Python adapter to read BME680 with bosh algorithm
# This code needs the following files:
# bsec_bme680
# bsec_iaq.config
# bsec_iaq.state

from time import sleep, ctime
from threading import Thread
import json, subprocess

bme_data = {};  

def run_bme680():
  global bme_data
  command = './bsec_bme680'
  process = subprocess.Popen(command, stdout=subprocess.PIPE)
  while True:
    output = process.stdout.readline()
    if output:
      print(output)
      bme_data = json.loads(output)
    else:
      print('... adquisition stopped')
      break

Thread(target=run_bme680, args=[]).start()

# Parameters you can pick from dictionary:
# IAQa: IAQ Accuracy
# IAQ: IAQ index
# temp: Temperature
# hum: humidity
# press: pressure
# res: sensor resistence (Ohm)
# status: Sensor status
# eCO2_ppm: eCO2
# bVOCe_ppm: bVOC

while True:
  sleep(5)
  print(
    ctime(),
    "IAQ:", bme_data["IAQ"],
    "Pressure:", bme_data["press"],
    "eCO2_ppm:", bme_data["eCO2_ppm"],
    "bVOCe_ppm:", bme_data["bVOCe_ppm"]
  )