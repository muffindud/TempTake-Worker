#ifndef WORKERPAIR_H
#define WORKERPAIR_H

#include <Arduino.h>
#include <Preferences.h>
#include <Wire.h>

#define PAIR_ADDR 0x55
#define PAIR_BUTTON_PIN 2

extern Preferences preferences;

struct MAC_ADDRESS_T{
    uint8_t mac[6];
};

/*
* @brief Exchange the MAC address with the manager module over the I2C bus
* @param worker_mac: The MAC address of the worker module
*/
void pairManager(MAC_ADDRESS_T worker_mac);

/*
* @brief Set the MAC address of the manager in the EEPROM
* @param manager_mac: The MAC address of the manager
*/
void setManagerMac(MAC_ADDRESS_T manager_mac);

/*
* @brief Get the MAC address of the manager from the EEPROM
* @return The MAC address of the manager
*/
MAC_ADDRESS_T getManagerMac();

#endif
