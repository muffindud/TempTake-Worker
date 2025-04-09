#ifndef WORKERPAIRLIB_H
#define WORKERPAIRLIB_H

#include <Arduino.h>
#include <Preferences.h>
#include <Wire.h>

#define PAIR_ADDR 0x55
#define PAIR_BUTTON_PIN 2

#include "config.h"

extern Preferences preferences;

/**
 * Check if the module is in pairing mode
 *
 * @returns `true` if PAIR_BUTTON_PIN is LOW, `false` if otherwise
 */
bool isPairingMode();

/**
 * Exchange credentials with the manager module
 */
void exchangeManagerCreds();

/**
 * Get the manager mac stored into preferences
 *
 * @returns pointer to the manager mac address
 */
uint8_t* getManagerMac();

#endif
