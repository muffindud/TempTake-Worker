#include "ManagerPairLib.h"

#define PREFERENCES_NAMESPACE "pair"
#define MANAGER_MAC_KEY "MAN_MAC"

Preferences preferences;

bool pinSetup = false;
uint8_t localWorkerMac[6];

bool isPairingMode(){
    if(!pinSetup){
        esp_efuse_mac_get_default(localWorkerMac);
        pinMode(PAIR_BUTTON_PIN, INPUT_PULLUP);
        pinSetup = true;
    }

    return digitalRead(PAIR_BUTTON_PIN) == LOW;
}

void setManagerMac(uint8_t* managerMac){
    preferences.begin(PREFERENCES_NAMESPACE, false);
    preferences.putBytes(MANAGER_MAC_KEY, managerMac, 6);
    preferences.end();
}

void onReceive(int numBytes){
    if(numBytes != 6) return;

    uint8_t managerMac[6];

    for(int i = 0; i < 6; i++) managerMac[i] = Wire.read();

    setManagerMac(managerMac);

    // TODO: Send a wireless packet for check
    // TODO: Add status LED
}

void onRequest(){
    Wire.write(localWorkerMac, 6);
}

void exchangeManagerCreds(){
    Wire.begin(PAIR_ADDR);

    Wire.onReceive(onReceive);
    Wire.onRequest(onRequest);

    while(isPairingMode()) delay(10);

    Wire.end();
}

uint8_t* getManagerMac(){
    static uint8_t managerMac[6];

    preferences.begin(PREFERENCES_NAMESPACE, true);
    // If no MAC is present use a broadcast address
    if(preferences.getBytes(MANAGER_MAC_KEY, &managerMac, 6) != 6) memset(managerMac, 0xFF, 6);
    preferences.end();

    return managerMac;
}

uint8_t* getWorkerMac(){
    if(!pinSetup) isPairingMode();

    return localWorkerMac;
}
