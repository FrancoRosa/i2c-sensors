#include "main.h"

int main()
{
    int8_t ret;

    ret = zmod44xx_read_sensor_info();
    if(ret) {
        printf("Error %d, exiting program!\n", ret);
    }

    ret = zmod44xx_init_sensor();
    if(ret) {
        printf("Error %d, exiting program!\n", ret);
    }

    ret = zmod44xx_init_measurement();
    if(ret) {
        printf("Error %d, exiting program!\n", ret);
    }
}

