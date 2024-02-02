# Cortex-M7/M4 Operating System Project

## Overview

This project is an operating system designed for Cortex-M7/M4 microcontrollers, developed on a Raspberry Pi 5, and deployed on an ST Nucleo H755 board.

## Getting Started

### Prerequisites

- Raspberry Pi 5
- ST Nucleo H755 board

### Compilation

To compile the project, use the following command:

```bash
make -j4
```

This will build the operating system for both Cortex-M7 and Cortex-M4.

### Loading Hex Files onto the Board

To load the hex files onto the ST Nucleo H755 board, use the following commands:

For Cortex-M7:

```bash
cp CM7/build/rtossed2_CM7.hex /media/pi/NOD_H755ZIQ/
```

For Cortex-M4:

```bash
cp CM4/build/rtossed2_CM4.hex /media/pi/NOD_H755ZIQ/
```

Replace `/media/pi/NOD_H755ZIQ/` with the appropriate path to the mounted drive on your system.
