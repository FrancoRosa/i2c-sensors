
#include <stdio.h>
#include <errno.h>
#include <wiringPiI2C.h>

int t = 10;
int main() {
  int fd;
  int result;
  fd = wiringPiI2CSetup(0x32);
  result = wiringPiI2CReadReg8(fd, 0x88);
  wiringPiI2CWriteReg8(fd, 0x88, 99);
  printf("Hello Pi\n");
  printf("%d\n", result);
  return 0;
}
