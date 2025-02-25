#include "ManagerPair.h"

uint8_t local_worker_mac[6];
Preferences preferences;

void onReceive(int num_bytes){
    MAC_ADDRESS_T manager_mac;

    if(num_bytes != 6){
        return;
    }

    for(int i = 0; i < 6; i++){
        manager_mac.mac[i] = Wire.read();
    }

    setManagerMac(manager_mac);
}

void onRequest(){
    Wire.write(local_worker_mac, 6);
}

void pairManager(MAC_ADDRESS_T worker_mac){
    Wire.end();

    Wire.begin(PAIR_ADDR);

    for(int i = 0; i < 6; i++){
        local_worker_mac[i] = worker_mac.mac[i];
    }

    Wire.onReceive(onReceive);
    Wire.onRequest(onRequest);

    Wire.begin();
}

void setManagerMac(MAC_ADDRESS_T manager_mac){
    preferences.clear();
    preferences.putBytes("manager_mac", manager_mac.mac, 6);
}

MAC_ADDRESS_T getManagerMac(){
    MAC_ADDRESS_T manager_mac;

    preferences.getBytes("manager_mac", manager_mac.mac, 6);

    // if empty return an array of 0x0
    if(manager_mac.mac[0] == 0){
        for(int i = 0; i < 6; i++){
            manager_mac.mac[i] = 0;
        }
    }

    return manager_mac;
}
