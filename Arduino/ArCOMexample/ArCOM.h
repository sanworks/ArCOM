/*
----------------------------------------------------------------------------

This file is part of the Sanworks ArCOM repository
Copyright (C) 2016 Sanworks LLC, Sound Beach, New York, USA
Copyright (C) 2022 Florian Rau
Copyright (C) 2026 International Brain Laboratory

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
#pragma once
#include <Arduino.h>

// Macro for generating wrapper functions. Arguments are taken by value, like
// the functions they replace, so that bit-fields and members of packed
// structs can be passed.
// cf. https://www.fluentcpp.com/2017/10/27/function-aliases-cpp/
#define ARCOM_ALIAS(alias, target)                \
template<typename... Args>                        \
inline auto alias(Args... args) ->                \
decltype(target(args...)) {                       \
  return target(args...);                         \
}

namespace ArCOMdetail {
  template<typename T> struct isDouble { static const bool value = false; };
  template<> struct isDouble<double> { static const bool value = true; };
  template<typename T> struct isPointer { static const bool value = false; };
  template<typename T> struct isPointer<T *> { static const bool value = true; };
  template<typename T> struct alwaysFalse { static const bool value = false; };
}

class ArCOM
{
  // Used for restricting the templated constructor to classes derived from
  // Stream (needs to be declared ahead of the constructor)
  static void isStream(Stream *);

public:

  // Constructor: sets the interface (Serial, Serial1, SerialUSB, etc.)
  ArCOM(Stream &s) : ArCOMstream(&s), readSomeFn(&readSome<Stream>) {}

  // Constructor for classes derived from Stream: binds to the readBytes() of
  // that class, which may be faster than the one of Stream. Classes without an
  // accessible readBytes(char *, size_t) use the constructor above instead.
  template<typename S, typename = decltype(isStream((S *) nullptr)),
           typename = decltype(((S *) nullptr)->readBytes((char *) nullptr, (size_t) 0))>
  ArCOM(S &s) : ArCOMstream(&s), readSomeFn(&readSome<S>) {}

  // Serial functions
  unsigned int available();
  void flush();
  Stream &stream() { return *ArCOMstream; }

  // Template: Write scalar
  template<typename T> void write(T data) {
    checkType<T>();
    ArCOMstream->write(reinterpret_cast<const uint8_t *>(&data), sizeof(T));
  }

  // A pointer passed without a number of values would send the address
  template<typename T> void write(T *) {
    static_assert(ArCOMdetail::alwaysFalse<T>::value,
                  "ArCOM: pass the number of values, e.g. write(data, nValues)");
  }

  // Template: Write array (raw bytes, does not make use of write<T>())
  // Array functions accept volatile arrays
  template<typename T> void write(const volatile T *data, size_t nValues) {
    checkType<T>();
    writeChunked(reinterpret_cast<const uint8_t *>(const_cast<const T *>(data)),
                 nValues * sizeof(T));
  }

  // Template: Read scalar
  template<typename T> T read() {
    checkType<T>();
    T data;
    readBlocking(reinterpret_cast<uint8_t *>(&data), sizeof(T));
    return data;
  }

  // Template: Read array (raw bytes, does not make use of read<T>())
  template<typename T> void read(volatile T *data, size_t nValues) {
    checkType<T>();
    readBlocking(reinterpret_cast<uint8_t *>(const_cast<T *>(data)),
                 nValues * sizeof(T));
  }

  // Wrapper functions for backwards-compatibility (read scalar)
  ARCOM_ALIAS(readByte,         read<uint8_t>)
  ARCOM_ALIAS(readUint8,        read<uint8_t>)
  ARCOM_ALIAS(readUint16,       read<uint16_t>)
  ARCOM_ALIAS(readUint32,       read<uint32_t>)
  ARCOM_ALIAS(readUint64,       read<uint64_t>)
  ARCOM_ALIAS(readChar,         read<char>)
  ARCOM_ALIAS(readInt8,         read<int8_t>)
  ARCOM_ALIAS(readInt16,        read<int16_t>)
  ARCOM_ALIAS(readInt32,        read<int32_t>)
  ARCOM_ALIAS(readInt64,        read<int64_t>)
  ARCOM_ALIAS(readFloat,        read<float>)
  ARCOM_ALIAS(readDouble,       read<double>)

  // Wrapper functions for backwards-compatibility (read array)
  ARCOM_ALIAS(readByteArray,    read<uint8_t>)
  ARCOM_ALIAS(readUint8Array,   read<uint8_t>)
  ARCOM_ALIAS(readUint16Array,  read<uint16_t>)
  ARCOM_ALIAS(readUint32Array,  read<uint32_t>)
  ARCOM_ALIAS(readUint64Array,  read<uint64_t>)
  ARCOM_ALIAS(readCharArray,    read<char>)
  ARCOM_ALIAS(readInt8Array,    read<int8_t>)
  ARCOM_ALIAS(readInt16Array,   read<int16_t>)
  ARCOM_ALIAS(readInt32Array,   read<int32_t>)
  ARCOM_ALIAS(readInt64Array,   read<int64_t>)
  ARCOM_ALIAS(readFloatArray,   read<float>)
  ARCOM_ALIAS(readDoubleArray,  read<double>)

  // Wrapper functions for backwards-compatibility (write scalar)
  ARCOM_ALIAS(writeByte,        write<uint8_t>)
  ARCOM_ALIAS(writeUint8,       write<uint8_t>)
  ARCOM_ALIAS(writeUint16,      write<uint16_t>)
  ARCOM_ALIAS(writeUint32,      write<uint32_t>)
  ARCOM_ALIAS(writeUint64,      write<uint64_t>)
  ARCOM_ALIAS(writeChar,        write<char>)
  ARCOM_ALIAS(writeInt8,        write<int8_t>)
  ARCOM_ALIAS(writeInt16,       write<int16_t>)
  ARCOM_ALIAS(writeInt32,       write<int32_t>)
  ARCOM_ALIAS(writeInt64,       write<int64_t>)
  ARCOM_ALIAS(writeFloat,       write<float>)
  ARCOM_ALIAS(writeDouble,      write<double>)

  // Wrapper functions for backwards-compatibility (write array)
  ARCOM_ALIAS(writeByteArray,   write<uint8_t>)
  ARCOM_ALIAS(writeUint8Array,  write<uint8_t>)
  ARCOM_ALIAS(writeUint16Array, write<uint16_t>)
  ARCOM_ALIAS(writeUint32Array, write<uint32_t>)
  ARCOM_ALIAS(writeUint64Array, write<uint64_t>)
  ARCOM_ALIAS(writeCharArray,   write<char>)
  ARCOM_ALIAS(writeInt8Array,   write<int8_t>)
  ARCOM_ALIAS(writeInt16Array,  write<int16_t>)
  ARCOM_ALIAS(writeInt32Array,  write<int32_t>)
  ARCOM_ALIAS(writeInt64Array,  write<int64_t>)
  ARCOM_ALIAS(writeFloatArray,  write<float>)
  ARCOM_ALIAS(writeDoubleArray, write<double>)

private:
  // Rejects types that cannot be exchanged: pointers (they would send an
  // address), and double on boards where it has only 4 bytes (it would not
  // match the 8-byte doubles used by MATLAB and Python)
  template<typename T> static void checkType() {
    static_assert(!ArCOMdetail::isPointer<T>::value,
                  "ArCOM: pointers cannot be sent or received, pass data and number of values");
    static_assert(sizeof(double) == 8 || !ArCOMdetail::isDouble<T>::value,
                  "ArCOM: double has only 4 bytes on this board, use float instead");
  }

  Stream *ArCOMstream;
  size_t (*readSomeFn)(Stream *, uint8_t *, size_t);

  // Reads what has arrived so far, up to nBytes, without waiting:
  // - The first byte is obtained by means of read(), the only function that
  //   also returns a byte held back by peek() (SAMD).
  // - The rest is read in bulk, but only as far as available, so readBytes()
  //   never waits (on Teensy, it would wait without calling yield() and set
  //   the stream's read-error flag on timeout).
  template<typename S> static size_t readSome(Stream *s, uint8_t *data, size_t nBytes) {
    int c = s->read();
    if (c < 0)
      return 0;
    *data = c;
    if (nBytes == 1)
      return 1;
    int nAvailable = s->available();
    if (nAvailable <= 0)
      return 1;
    size_t nWant = min((size_t) nAvailable, nBytes - 1);
    size_t nRead = static_cast<S *>(s)->readBytes((char *) data + 1, nWant);
    return nRead <= nWant ? 1 + nRead : 1;  // else invalid, e.g., -1 cast to size_t
  }

  // The two helpers below are defined here, not in ArCOM.cpp, so that they
  // can be inlined: out of line, single-byte reads are ~14% slower (Teensy 4).

  // Writes in chunks, as some cores cannot send large buffers in a single
  // call of write() (e.g., SAMD: 16384 bytes). The chunk size is kept well
  // below that; it has no measurable effect on throughput.
  void writeChunked(const uint8_t *data, size_t nBytes) {
    const size_t chunkSize = 1024;
    while (nBytes > 0) {
      size_t nWrite = min(nBytes, chunkSize);
      ArCOMstream->write(data, nWrite);
      data += nWrite;
      nBytes -= nWrite;
    }
  }

  // Blocks until all bytes have been read. While waiting, yield() is called
  // explicitly, as available() does not do so on all cores (e.g., SAMD, Due).
  void readBlocking(uint8_t *data, size_t nBytes) {
    while (nBytes > 0) {
      size_t nRead;
      if (nBytes == 1) {  // single byte: read() directly, saves a call
        int c = ArCOMstream->read();
        nRead = c >= 0;
        if (nRead)
          *data = c;
      } else
        nRead = readSomeFn(ArCOMstream, data, nBytes);
      if (nRead == 0) {
        ArCOMstream->available();  // services USB on some cores (TinyUSB)
        yield();
        continue;
      }
      data += nRead;
      nBytes -= nRead;
    }
  }
};

#undef ARCOM_ALIAS

// USB serial ports of Teensy 3.x, LC and 4.x: peek() does not hold bytes back
// and usb_serial*_read() never waits, so it can be used directly (Teensy 4:
// multi-byte values ~2.5x faster; Teensy 3.5: uint16 values ~1.8x faster).
// - Below 20 MHz, the Teensy 3 core replaces USB serial with a stub.
// - USB_SERIAL_DTR is only defined if the USB type includes serial.
// - SerialUSB1/2 only exist in the dual/triple serial USB types. These are
//   tested directly, as the Teensy 3 core hides its CDC2/CDC3 macros.
#if defined(TEENSYDUINO) && defined(__arm__) && F_CPU >= 20000000
#if defined(USB_SERIAL_DTR)
template<> inline size_t ArCOM::readSome<usb_serial_class>(Stream *, uint8_t *data, size_t nBytes) {
  return usb_serial_read(data, nBytes);
}
#endif
#if defined(USB_DUAL_SERIAL) || defined(USB_TRIPLE_SERIAL)
template<> inline size_t ArCOM::readSome<usb_serial2_class>(Stream *, uint8_t *data, size_t nBytes) {
  return usb_serial2_read(data, nBytes);
}
#endif
#if defined(USB_TRIPLE_SERIAL)
template<> inline size_t ArCOM::readSome<usb_serial3_class>(Stream *, uint8_t *data, size_t nBytes) {
  return usb_serial3_read(data, nBytes);
}
#endif
#endif
