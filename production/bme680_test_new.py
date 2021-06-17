## Python adapter to read BME680 with bosh algorithm
# This code needs the following files:
# bsec_bme680
# bsec_iaq.config
# bsec_iaq.state
from time import sleep
import os
import json

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
def get_bme680():
  stream = os.popen('./bsec_bme680')
  return json.loads(stream.read())

while True:
  values = get_bme680();
  print("IAQ:", values["IAQ"], "Presure:", values["press"])
  sleep(5)
