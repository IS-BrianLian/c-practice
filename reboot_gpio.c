#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

void register_gpio(int gpioNum, char *direction, int size);
void set_gpio(int gpioNum, int value);

int main(int argc, const char * argv[]) {
    int gpioNum;
    char direction[16];
    int setValue;

    gpioNum = 21;
    sprintf(direction, "high");
    setValue = 0;

    if (argc > 1 && strstr(argv[1], "help") != NULL) {
        printf("Usage:\n\treboot [gpio_num] [high|low]\n\tDefault is set gpio 21 to high\n");
        return 0;
    }
    if (argc == 2) {
        gpioNum = atoi(argv[1]);
    } else if (argc == 3) {
        gpioNum = atoi(argv[1]);
        printf("%s\n", strstr(argv[2], "low"));
        if (strstr(argv[2], "low") != NULL) {
            sprintf(direction, "low");
            setValue = 1;
        }
    }

    // printf("argc is %d\n", argc);
    // printf("gpioNum is %d\n", gpioNum);
    // printf("direction is %s\n", direction);
    // printf("setValue is %d\n", setValue);

    register_gpio(gpioNum, direction, strlen(direction));
    set_gpio(gpioNum, setValue);

    return 0;
}


void register_gpio(int gpioNum, char *direction, int size) {
    int fd;
    char gpio_id[2];
    char path[32];

    sprintf(path, "/sys/class/gpio/export");
    fd = open(path, O_WRONLY);
    if (fd < 0) {
        printf("open '%s' error return < 0 , %d\n", path, fd);
        return;
    }
    sprintf(gpio_id, "%d", gpioNum);
    write(fd, gpio_id, 2);
    close(fd);

    sprintf(path, "/sys/class/gpio/gpio%d/direction", gpioNum);
    fd = open(path, O_WRONLY);
    if (fd < 0) {
        printf("open '%s' error return < 0 , %d\n", path, fd);
        return;
    }
    write(fd, direction, size);
    close(fd);
}

void set_gpio(int gpioNum, int value) {
    int fd;
    char path[32];
    char rtn[1];

    rtn[0] = '1';
    if (value == 0) {
        rtn[0] = '0';
    }

    sprintf(path, "/sys/class/gpio/gpio%d/value", gpioNum);
    fd = open(path, O_WRONLY);
    if (fd < 0) {
        printf("open '%s' error return < 0 , %d\n", path, fd);
        return;
    }
    write(fd, rtn, 1);
    close(fd);
}
