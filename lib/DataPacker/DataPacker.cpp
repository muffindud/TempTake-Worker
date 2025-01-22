#include "DataPacker.h"

uint8_t* dataToStream(DAT_T data){
    uint8_t* stream = (uint8_t*)malloc(DAT_SIZE + data.length);
    stream[0] = data.type;
    stream[1] = (data.meta.crc16 >> 8) & 0xFF;
    stream[2] = data.meta.crc16 & 0xFF;
    for (int i = 0; i < 6; i++){
        stream[3 + i] = data.meta.manager_mac[i];
    }
    stream[9] = (data.meta.id >> 8) & 0xFF;
    stream[10] = data.meta.id & 0xFF;
    stream[11] = data.meta.index_packet;
    stream[12] = data.meta.total_packet_s;
    stream[13] = data.length;
    for (int i = 0; i < data.length; i++){
        stream[14 + i] = data.data[i];
    }
    return stream;
}

ACK_T streamToAck(uint8_t* stream){
    ACK_T ack;
    ack.meta.crc16 = (stream[1] << 8) | stream[2];
    for (int i = 0; i < 6; i++){
        ack.meta.manager_mac[i] = stream[3 + i];
    }
    ack.meta.id = (stream[9] << 8) | stream[10];
    ack.meta.index_packet = stream[11];
    ack.meta.total_packet_s = stream[12];
    return ack;
}

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
        return 0; // Return 0 for invalid input
    }

    uint16_t id = 0x0;
    int mac_idx = data[length / 2] % 6;
    id |= ((source_mac[mac_idx] + dest_mac[mac_idx]) & 0xFF) << 8;
    id |= ((crc16 & 0xFF) + ((crc16 & 0xFF00) >> 8));

    return id;
}
