#ifndef DATAPACKER_H
#define DATAPACKER_H

/*
* Pack the data into a byte array
*
* Structure:
*
* Start byte                    >> 0x7E                         >> 1 byte
* Mode                          >> 0x00 for SYN, 0x01 for ACK   >> 1 byte
* Data length                   >> n                            >> 1 byte
* Data                          >> encripted data               >> n bytes (max 128 bytes)
* CRC                           >> crc16                        >> 2 bytes
* Origin                        >> MAC address                  >> 6 bytes
* Destination                   >> MAC address                  >> 6 bytes
* Packet ID                     >> id + total                   >> 2 bytes
* End byte                      >> 0x7F                         >> 1 byte
*/

#endif
