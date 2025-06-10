Other Language Versions:
- [English](README.md)
- [中文](README_zh.md)
# Simple-RVOS

A simple Real-Time Operating System (RTOS) project.

## Overview

This project is a lightweight Real-Time Operating System (RTOS) specifically designed for the RISC-V architecture. Its primary goal is to provide a foundational platform for learning and researching RTOS principles, encompassing a complete kernel and user-space implementation, along with a comprehensive build and debugging environment.

## Key Features

- **Task Management**: Supports creation, deletion, suspension, resumption, and switching of tasks.
- **Task Scheduling**: Implements preemptive or time-slice round-robin scheduling to ensure fair execution of tasks.
- **Memory Management**: Provides mechanisms for memory allocation and deallocation, supporting heap and stack management.
- **Interrupt Handling**: Implements interrupt vector tables and interrupt service routines to respond to external events.
- **Synchronization and Communication**: Offers mechanisms such as semaphores, mutexes, and message queues for inter-task synchronization and communication.

## Build, Run, and Debug

### Building the Project

To build the project, run:

```bash
make
```

### Running the Project

To run the project, run:

```bash
make run
```

### Debugging the Project

To debug the project using GDB, run:

```bash
make debug
```
