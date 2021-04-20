from time import sleep
from datetime import datetime
from random import random as rd
import csv
import signal
import sys

class GracefulKiller:
  kill_now = False
  def __init__(self):
    signal.signal(signal.SIGINT, self.exit_gracefully)
    signal.signal(signal.SIGTERM, self.exit_gracefully)

  def exit_gracefully(self,signum, frame):
    self.kill_now = True

def timestamp():
  z = datetime.now()
  return z.strftime("%H:%M:%S")

def timestart():
  z = datetime.now()
  return z.strftime("%Y%m%d_%H:%M:%S")

values = [['timestamp', 'temperature', 'humidity']]


if __name__ == '__main__':
  killer = GracefulKiller()
  try:
    filename = timestart() + '_' + str(sys.argv[1]) + '.csv'
  except:
    filename = timestart() + '.csv'
  while not killer.kill_now:
    sampletime = timestamp()
    temperature = 25 + round(2*rd(),2)
    humidity = 50 + round(2*rd(),1)
    values.append([sampletime, temperature, humidity])
    print("%s >> Temp: %2.2f, Hum: %2.1f, "%(sampletime, temperature, humidity), end= '\r')
    sleep(1)
  print('\n\n\n... creating file >> %s'%(filename))
  with open(filename, 'w', newline='') as f:
    writer = csv.writer(f)

    writer.writerows(values)
  print('... file created.')
  print("Have a nice day :)")
