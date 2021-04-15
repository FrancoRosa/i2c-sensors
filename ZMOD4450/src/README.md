SparkX ZMOD4450 I2C Driver
================
The ZMOD4450 Refrigeration Gas Sensor is an I2C sensor designed to detect gases from rotting food in refrrigerator temperature environments. Renesas employs closed source software that took some highly unusal methods to get it to compile with Arduino.

Prior to attempting this Arduino Port, please read the ZMOD4450 Programming Manual included with [Renasas' proprietary firmware.](https://www.idt.com/us/en/products/sensor-products/gas-sensors/zmod4450-refrigeration-air-quality-sensor-platform)

Once unzipped, the ZMOD4450 folder should be moved to `./Documents/Arduino/Libraries/`

Now, add all of the necessary files from [Renasas' proprietary firmware](https://www.idt.com/us/en/products/sensor-products/gas-sensors/zmod4450-refrigeration-air-quality-sensor-platform) into `./Documents/Arduino/Libraries/ZMOD4450/src/`. Grab everything from `/src/` in the Renesas `.ZIP` and move it to your libraries `/src/`

Here things get tricky, we can't tell you exactly what to do as the firmware is proprietary. Attempting to compile your Arduino Sketch at this point will result in some errors from trying to include libraries for the Renesas Evaluation Kit, go ahead and comment these out. Now you may see that functions aren't defined. We were able to get simple RMox values out of the sensor by moving function definitions from their `.c` files to live with the function prototypes in the `.h` files and then simply deleting the `.c` files. Also make sure to replace your `printf` statements with appropriate `Serial.print` statements. Refer to the documents included with Renesas bundle for additional assistance.