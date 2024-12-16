# Linux Device Driver Development

This repository showcases my work on Linux kernel module and device driver development. It demonstrates my understanding of low-level programming concepts, system calls, and interaction between user space and kernel space. The main focus is on character device drivers and related kernel module development.

## Features
- **Character Device Driver**:
  - Dynamic allocation of major and minor numbers.
  - Basic file operations: open, read, write, and close.
  - Clean error handling and resource management.
- **Hello Kernel Module**:
  - A simple kernel module to understand kernel programming basics.
- **Parameterized Kernel Module**:
  - Demonstrates the use of parameters passed during module insertion.
- **User-Space Test Applications**:
  - Test applications written in C to interact with the character device driver.

## Directory Structure
- chardev/: Character device driver source code and user-space test application.
- hello_kernel/: Simple kernel module printing messages to the kernel log.
- mod_parameter/: Kernel module with user-configurable parameters.
- README.md: Documentation for the project.

## Setup and Installation

### Prerequisites
Ensure the following tools and headers are installed on your Linux system:
- Linux kernel development headers (`linux-headers` package).
- GCC compiler.
- Make utility.

### Clone the Repository
To get started, clone this repository:

git clone https://github.com/yourusername/linux_device_driver.git
cd linux_device_driver

