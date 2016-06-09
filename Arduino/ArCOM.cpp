/*
----------------------------------------------------------------------------

This file is part of the Sanworks ArCOM repository
Copyright (C) 2016 Sanworks LLC, Sound Beach, New York, USA

----------------------------------------------------------------------------

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, version 3.

This program is distributed  WITHOUT ANY WARRANTY and without even the 
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
#include <Arduino.h>
#include "ArCOM.h"

void ArCOM::writeByte(byte byte2Write) {
  SerialUSB.write(byte2Write);
}
void ArCOM::writeUint8(byte byte2Write) {
  SerialUSB.write(byte2Write);
}
void ArCOM::writeUint16(unsigned short int2Write) {
   SerialUSB.write((byte)int2Write);
   SerialUSB.write((byte)(int2Write >> 8));
}

void ArCOM::writeUint32(unsigned long int2Write) {
    SerialUSB.write((byte)int2Write);
    SerialUSB.write((byte)(int2Write >> 8));
    SerialUSB.write((byte)(int2Write >> 16));
    SerialUSB.write((byte)(int2Write >> 24));
}
byte ArCOM::readByte(){
  while (SerialUSB.available() == 0) {}
  return SerialUSB.read();
}
byte ArCOM::readUint8(){
  while (SerialUSB.available() == 0) {}
  return SerialUSB.read();
}
unsigned short ArCOM::readUint16() {
  while (SerialUSB.available() == 0) {}
  typeBuffer.byteArray[0] = SerialUSB.read();
  while (SerialUSB.available() == 0) {}
  typeBuffer.byteArray[1] = SerialUSB.read();
  return typeBuffer.uint16;
}

unsigned long ArCOM::readUint32() {
  while (SerialUSB.available() == 0) {}
  typeBuffer.byteArray[0] = SerialUSB.read();
  while (SerialUSB.available() == 0) {}
  typeBuffer.byteArray[1] = SerialUSB.read();
  while (SerialUSB.available() == 0) {}
  typeBuffer.byteArray[2] = SerialUSB.read();
  while (SerialUSB.available() == 0) {}
  typeBuffer.byteArray[3] = SerialUSB.read();
  return typeBuffer.uint32;
}

void ArCOM::writeInt8(int8_t int2Write) {
  typeBuffer.int8 = int2Write;
  SerialUSB.write(typeBuffer.byteArray[0]);
}

void ArCOM::writeInt16(int16_t int2Write) {
  typeBuffer.int16 = int2Write;
  SerialUSB.write(typeBuffer.byteArray[0]);
  SerialUSB.write(typeBuffer.byteArray[1]);
}

void ArCOM::writeInt32(int32_t int2Write) {
  typeBuffer.int32 = int2Write;
  SerialUSB.write(typeBuffer.byteArray[0]);
  SerialUSB.write(typeBuffer.byteArray[1]);
  SerialUSB.write(typeBuffer.byteArray[2]);
  SerialUSB.write(typeBuffer.byteArray[3]);
}

int8_t ArCOM::readInt8() {
  while (SerialUSB.available() == 0) {}
  typeBuffer.byteArray[0] = SerialUSB.read();
  return typeBuffer.int8;
}
int16_t ArCOM::readInt16() {
  while (SerialUSB.available() == 0) {}
  typeBuffer.byteArray[0] = SerialUSB.read();
  while (SerialUSB.available() == 0) {}
  typeBuffer.byteArray[1] = SerialUSB.read();
  return typeBuffer.int16;
}
int32_t ArCOM::readInt32() {
  while (SerialUSB.available() == 0) {}
  typeBuffer.byteArray[0] = SerialUSB.read();
  while (SerialUSB.available() == 0) {}
  typeBuffer.byteArray[1] = SerialUSB.read();
  while (SerialUSB.available() == 0) {}
  typeBuffer.byteArray[2] = SerialUSB.read();
  while (SerialUSB.available() == 0) {}
  typeBuffer.byteArray[3] = SerialUSB.read();
  return typeBuffer.int32;
}
void ArCOM::writeByteArray(byte numArray[], unsigned int nValues) {
  for (int i = 0; i < nValues; i++) {
    SerialUSB.write(numArray[i]);
  }
}
void ArCOM::writeUint8Array(byte numArray[], unsigned int nValues) {
  for (int i = 0; i < nValues; i++) {
    SerialUSB.write(numArray[i]);
  }
}
void ArCOM::writeInt8Array(int8_t numArray[], unsigned int nValues) {
  for (int i = 0; i < nValues; i++) {
    typeBuffer.int8 = numArray[i];
    SerialUSB.write(typeBuffer.byteArray[0]);
  }
}
void ArCOM::writeUint16Array(unsigned short numArray[], unsigned int nValues) {
  for (unsigned int i = 0; i < nValues; i++) {
    typeBuffer.uint16 = numArray[i];
    SerialUSB.write(typeBuffer.byteArray[0]);
    SerialUSB.write(typeBuffer.byteArray[1]);
  }
}
void ArCOM::writeInt16Array(int16_t numArray[], unsigned int nValues) {
  for (int i = 0; i < nValues; i++) {
    typeBuffer.int16 = numArray[i];
    SerialUSB.write(typeBuffer.byteArray[0]);
    SerialUSB.write(typeBuffer.byteArray[1]);
  }
}
void ArCOM::writeUint32Array(unsigned long numArray[], unsigned int nValues) {
  for (unsigned int i = 0; i < nValues; i++) {
    typeBuffer.uint32 = numArray[i];
    SerialUSB.write(typeBuffer.byteArray[0]);
    SerialUSB.write(typeBuffer.byteArray[1]);
    SerialUSB.write(typeBuffer.byteArray[2]);
    SerialUSB.write(typeBuffer.byteArray[3]);
  }
}
void ArCOM::writeInt32Array(long numArray[], unsigned int nValues) {
  for (unsigned int i = 0; i < nValues; i++) {
    typeBuffer.int32 = numArray[i];
    SerialUSB.write(typeBuffer.byteArray[0]);
    SerialUSB.write(typeBuffer.byteArray[1]);
    SerialUSB.write(typeBuffer.byteArray[2]);
    SerialUSB.write(typeBuffer.byteArray[3]);
  }
}
void ArCOM::readByteArray(byte numArray[], unsigned int nValues) {
  for (unsigned int i = 0; i < nValues; i++) {
    while (SerialUSB.available() == 0) {}
    numArray[i] = SerialUSB.read();
  }
}
void ArCOM::readUint8Array(byte numArray[], unsigned int nValues) {
  for (unsigned int i = 0; i < nValues; i++) {
    while (SerialUSB.available() == 0) {}
    numArray[i] = SerialUSB.read();
  }
}
void ArCOM::readInt8Array(int8_t numArray[], unsigned int nValues) {
  for (unsigned int i = 0; i < nValues; i++) {
    while (SerialUSB.available() == 0) {}
    typeBuffer.byteArray[0] = SerialUSB.read();
    numArray[i] = typeBuffer.int8;
  }
}
void ArCOM::readUint16Array(unsigned short numArray[], unsigned int nValues) {
  for (unsigned int i = 0; i < nValues; i++) {
    while (SerialUSB.available() == 0) {}
    typeBuffer.byteArray[0] = SerialUSB.read();
    while (SerialUSB.available() == 0) {}
    typeBuffer.byteArray[1] = SerialUSB.read();
    numArray[i] = typeBuffer.uint16;
  }
}
void ArCOM::readInt16Array(short numArray[], unsigned int nValues) {
  for (unsigned int i = 0; i < nValues; i++) {
    while (SerialUSB.available() == 0) {}
    typeBuffer.byteArray[0] = SerialUSB.read();
    while (SerialUSB.available() == 0) {}
    typeBuffer.byteArray[1] = SerialUSB.read();
    numArray[i] = typeBuffer.int16;
  }
}
void ArCOM::readUint32Array(unsigned long numArray[], unsigned int nValues) {
  for (unsigned int i = 0; i < nValues; i++) {
    while (SerialUSB.available() == 0) {}
    typeBuffer.byteArray[0] = SerialUSB.read();
    while (SerialUSB.available() == 0) {}
    typeBuffer.byteArray[1] = SerialUSB.read();
    while (SerialUSB.available() == 0) {}
    typeBuffer.byteArray[2] = SerialUSB.read();
    while (SerialUSB.available() == 0) {}
    typeBuffer.byteArray[3] = SerialUSB.read();
    numArray[i] = typeBuffer.uint32;
  }
}
void ArCOM::readInt32Array(long numArray[], unsigned int nValues) {
  for (unsigned int i = 0; i < nValues; i++) {
    while (SerialUSB.available() == 0) {}
    typeBuffer.byteArray[0] = SerialUSB.read();
    while (SerialUSB.available() == 0) {}
    typeBuffer.byteArray[1] = SerialUSB.read();
    while (SerialUSB.available() == 0) {}
    typeBuffer.byteArray[2] = SerialUSB.read();
    while (SerialUSB.available() == 0) {}
    typeBuffer.byteArray[3] = SerialUSB.read();
    numArray[i] = typeBuffer.int32;
  }
}
