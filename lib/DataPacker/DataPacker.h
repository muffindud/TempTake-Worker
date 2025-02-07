#ifndef DATAPACKER_H
#define DATAPACKER_H

#include <Arduino.h>

#define DAT_MODE 0xDD
#define ACK_MODE 0xAA

/*
* @brief Structure for the meta data of a packet
* @param crc16: CRC16 checksum of the data
* @param manager_mac: MAC address of the manager
* @param id: ID of the packet
* @param index_packet: Index of the packet
* @param total_packet_s: Total packets
*/
#pragma pack(push, 1)
struct META_T{
    uint16_t crc16;
    uint8_t manager_mac[6];
    uint8_t worker_mac[6];
    uint32_t id;
    uint8_t index_packet;
    uint8_t total_packet_s;
};
#pragma pack(pop)
const int META_SIZE = sizeof(META_T);

#pragma pack(push, 1)
struct RAW_DATA_T{
    uint64_t temperature;
    uint64_t humidity;
    uint64_t pressure;
    uint64_t ppm;
};
#pragma pack(pop)
const int RAW_DATA_SIZE = sizeof(RAW_DATA_T);

/*
* @brief Structure for a data packet
* @param type: Type of the packet
* @param meta: Meta data of the packet
* @param length: Length of the data
* @param data: Data of the packet
*/
#pragma pack(push, 1)
struct DAT_T{
    uint8_t type;
    META_T meta;
    uint8_t length;
    uint8_t data[32];
};
#pragma pack(pop)
const int DAT_SIZE = sizeof(DAT_T);

/*
* @brief Structure for an ACK packet
* @param type: Type of the packet
* @param meta: Meta data of the packet
*/
#pragma pack(push, 1)
struct ACK_T{
    uint8_t type;
    META_T meta;
};
#pragma pack(pop)
const int ACK_SIZE = sizeof(ACK_T);

/*
* @brief Calculate the CRC16 checksum of a data
* @param data_p: u8_t* data
* @param length: u16_t length
* @return u16_t crc16
*/
uint16_t crc16(RAW_DATA_T data_p);

/*
* @brief Get the ID of a packet
* @param source_mac: u8_t* source_mac
* @param dest_mac: u8_t* dest_mac
* @param data: u8_t* data
* @param length: u8_t length
* @param crc16: u16_t crc16
* @return u16_t id
*/
uint32_t getId(uint8_t* source_mac, uint8_t* dest_mac, RAW_DATA_T data_p, uint16_t crc16);

#endif
