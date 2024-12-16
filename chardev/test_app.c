#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define DEVICE_PATH "/dev/mychardev"
#define BUFFER_SIZE 1024

void menu() {
    printf("\n=== Menu ===\n");
    printf("1. Write to Device\n");
    printf("2. Read from Device\n");
    printf("3. Exit\n");
    printf("Choose an option: ");
}

int main() {
    int fd;
    char write_buffer[BUFFER_SIZE];
    char read_buffer[BUFFER_SIZE];
    int option;

    /* Open the device */
    fd = open(DEVICE_PATH, O_RDWR);
    if (fd < 0) {
        perror("Failed to open the device");
        return errno;
    }

    while (1) {
        menu();
        scanf("%d", &option);

        switch (option) {
            case 1:
                /* Write to the device */
                printf("Enter data to write: ");
                scanf(" %[^\n]s", write_buffer); // Read input including spaces
                if (write(fd, write_buffer, strlen(write_buffer)) < 0) {
                    perror("Failed to write to the device");
                } else {
                    printf("Data written successfully.\n");
                }
                break;

            case 2:
                /* Read from the device */
                printf("Reading from device...\n");
                ssize_t bytes_read = read(fd, read_buffer, BUFFER_SIZE - 1);
                if (bytes_read < 0) {
                    perror("Failed to read from the device");
                } else {
                    read_buffer[bytes_read] = '\0'; // Null-terminate the string
                    printf("Data read from device: %s\n", read_buffer);
                }
                break;

            case 3:
                /* Exit the application */
                printf("Exiting...\n");
                close(fd);
                return 0;

            default:
                printf("Invalid option. Please try again.\n");
                break;
        }
    }

    close(fd);
    return 0;
}
