extern "C"{
#include "C:\Users\user\Documents\Arduino\libraries\ZMOD4450\src\main.h"
};

void setup() {
  delay(5000);
  Serial.begin(115200);
  Wire.begin();
  Wire.setClock(100000);
  main();
}

void loop() {

}
