#include "DataPacker.h"

uint16_t crc16(uint8_t *data_p, uint8_t length){
    uint8_t x;
    uint16_t crc = 0xFFFF;

    while (length--){
        x = crc >> 8 ^ *data_p++;
        x ^= x >> 4;
        crc = (crc << 8) ^ ((uint16_t)(x << 12)) ^ ((uint16_t)(x << 5)) ^ ((uint16_t)x);
    }

    return crc;
}

uint16_t getId(uint8_t* source_mac, uint8_t* dest_mac, uint8_t* data, uint8_t length, uint16_t crc16) {
    if (length == 0 || data == NULL || source_mac == NULL || dest_mac == NULL) {
        return 0;
    }

    uint16_t id = 0x0;
    int mac_idx = data[length / 2] % 6;
    id |= ((source_mac[mac_idx] + dest_mac[mac_idx]) & 0xFF) << 8;
    id |= ((crc16 & 0xFF) + ((crc16 & 0xFF00) >> 8));

    return id;
}
