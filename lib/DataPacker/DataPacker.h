#ifndef DATAPACKER_H
#define DATAPACKER_H

#include <Arduino.h>

#define DAT_MODE 0x0D
#define ACK_MODE 0x0A

/*
* @brief Structure for the meta data of a packet
* @param crc16: CRC16 checksum of the data
* @param manager_mac: MAC address of the manager
* @param id: ID of the packet
* @param index_packet: Index of the packet
* @param total_packet_s: Total packets
*/
struct META_T{
    uint16_t crc16;
    uint8_t manager_mac[6];
    uint16_t id;
    uint8_t index_packet;
    uint8_t total_packet_s;
};
const int META_SIZE = sizeof(META_T);

/*
* @brief Structure for a data packet
* @param type: Type of the packet
* @param meta: Meta data of the packet
* @param length: Length of the data
* @param data: Data of the packet
*/
struct DAT_T{
    uint8_t type = 0x0D;
    META_T meta;
    uint8_t length;
    uint8_t* data;
};
const int DAT_SIZE = sizeof(DAT_T);

/*
* @brief Structure for an ACK packet
* @param type: Type of the packet
* @param meta: Meta data of the packet
*/
struct ACK_T{
    uint8_t type = 0x0A;
    META_T meta;
};
const int ACK_SIZE = sizeof(ACK_T);

/*
* @brief Create a stream from a DAT_T struct
* @param data: DAT_T struct
* @return u8_t* stream
*/
uint8_t* dataToStream(DAT_T data);

/*
* @brief Create a DAT_T struct from a stream
* @param stream: u8_t* stream
* @return DAT_T struct
*/
ACK_T streamToAck(uint8_t* stream);

/*
* @brief Calculate the CRC16 checksum of a data
* @param data_p: u8_t* data
* @param length: u16_t length
* @return u16_t crc16
*/
uint16_t crc16(uint8_t* data_p, uint8_t length);

/*
* @brief Get the ID of a packet
* @param source_mac: u8_t* source_mac
* @param dest_mac: u8_t* dest_mac
* @param data: u8_t* data
* @param length: u8_t length
* @param crc16: u16_t crc16
* @return u16_t id
*/
uint16_t getId(uint8_t* source_mac, uint8_t* dest_mac, uint8_t* data, uint8_t length, uint16_t crc16);

#endif
