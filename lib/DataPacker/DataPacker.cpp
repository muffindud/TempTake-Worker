#include "DataPacker.h"


#define DAT_MODE 0x0D
#define ACK_MODE 0x0A

u8_t* dataToStream(DAT_T data){
    u8_t* stream = (u8_t*)malloc(DAT_SIZE + data.length);
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

ACK_T streamToAck(u8_t* stream){
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

u16_t crc16(u8_t *data_p, u8_t length){
    u8_t x;
    u16_t crc = 0xFFFF;

    while (length--){
        x = crc >> 8 ^ *data_p++;
        x ^= x >> 4;
        crc = (crc << 8) ^ ((u16_t)(x << 12)) ^ ((u16_t)(x << 5)) ^ ((u16_t)x);
    }

    return crc;
}

u16_t getId(u8_t* source_mac, u8_t* dest_mac, u8_t* data, u8_t length, u16_t crc16) {
    if (length == 0 || data == NULL || source_mac == NULL || dest_mac == NULL) {
        return 0; // Return 0 for invalid input
    }

    u16_t id = 0x0;
    int mac_idx = data[length / 2] % 6;
    id |= ((source_mac[mac_idx] + dest_mac[mac_idx]) & 0xFF) << 8;
    id |= ((crc16 & 0xFF) + ((crc16 & 0xFF00) >> 8));

    return id;
}
