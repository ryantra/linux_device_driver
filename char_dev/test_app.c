#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int8_t write_buf[BUFFER_SIZE];
int8_t read_buf[BUFFER_SIZE];

int main() {
    int fd;
    char option;

    printf("Welcome to the demo of the character device driver...\n");

    // Open the device file
    fd = open("/dev/my_device", O_RDWR);
    if (fd < 0) {
        perror("Cannot open the device file");
        return EXIT_FAILURE;
    }

    while (1) {
        printf("\n***** Please select an option *****\n");
        printf("1. Write\n");
        printf("2. Read\n");
        printf("3. Exit\n");
        printf("Your choice: ");
        scanf(" %c", &option);

        switch (option) {
            case '1':
                printf("Enter the string to write into the driver (max %d chars):\n", BUFFER_SIZE - 1);
                scanf(" %[^\t\n]s", write_buf);

                // Write to the device
                ssize_t write_ret = write(fd, write_buf, strlen(write_buf) + 1);
                if (write_ret < 0) {
                    perror("Error writing to the device");
                } else {
                    printf("Data written successfully (%ld bytes).\n", write_ret);
                }
                break;

            case '2':
                printf("Reading data from the driver...\n");

                // Read from the device
                ssize_t read_ret = read(fd, read_buf, BUFFER_SIZE);
                if (read_ret < 0) {
                    perror("Error reading from the device");
                } else {
                    printf("Data read successfully (%ld bytes).\n", read_ret);
                    printf("Data: %s\n", read_buf);
                }
                break;

            case '3':
                printf("Exiting the application. Closing the device.\n");
                close(fd);
                return EXIT_SUCCESS;

            default:
                printf("Invalid option. Please try again.\n");
                break;
        }
    }

    // Close the device file
    close(fd);
    return EXIT_SUCCESS;
}
