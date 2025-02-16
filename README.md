# TempTake Worker

## About

This is the worker module for the TempTake project. The worker module is responsible for collecting data from the sensors and sending it to the manager module.

## Installation and setup

### 1. Make sure you have platformio installed

**Ubuntu:**
```bash
sudo apt install platformio
```

**Arch:**
```bash
sudo pacman -S platformio-core
```

### 2. Configure the src/config.h file

Configure the `src/config.h` file with the correct values for your setup following the `src/config.h.example` file (copy, paste and rename the file if using the same ports).

### 3. Connect the MCU and flash the firmware

(replace `/dev/ttyUSB0` with the correct port)
```bash
pio run -t upload --upload-port /dev/ttyUSB0
```

### 4. Pair the MCU with the manager module

Connect the two modules using the exposed I2C pins and the manager module will automatically pair with the worker module.
