#include "ManagerPair.h"

void pairManager(MAC_ADDRESS_T worker_mac){
    MAC_ADDRESS_T manager_mac;

    Wire.begin(PAIR_ADDR);

    Wire.requestFrom(PAIR_ADDR, 6);
    while(Wire.available()){
        Wire.readBytes(worker_mac.mac, 6);
    }

    Wire.beginTransmission(PAIR_ADDR);
    Wire.write(worker_mac.mac, 6);
    Wire.endTransmission();

    setManagerMac(manager_mac);
}

void setManagerMac(MAC_ADDRESS_T manager_mac){
    for(int i = 0; i < 6; i++){
        EEPROM.write(i, manager_mac.mac[i]);
    }
}

MAC_ADDRESS_T getManagerMac(){
    MAC_ADDRESS_T manager_mac;

    for(int i = 0; i < 6; i++){
        manager_mac.mac[i] = EEPROM.read(i);
    }

    return manager_mac;
}
