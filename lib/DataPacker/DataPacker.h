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
* Data                          >> data w src MAC (encripted)   >> n bytes (between 100 and 255 bytes we'll see)
* CRC                           >> crc16                        >> 2 bytes
* Destination                   >> MAC address                  >> 6 bytes
* Packet ID                     >> internal counter             >> 2 bytes
* Packet index                  >> packet counter               >> 1 byte
* Total packets                 >> total packets                >> 1 byte
* End byte                      >> 0x7F                         >> 1 byte
*/

#endif
