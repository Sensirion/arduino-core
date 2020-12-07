/*
 * Copyright (c) 2020, Sensirion AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <SensirionShdlcRxFrame.h>

SensirionShdlcRxFrame::SensirionShdlcRxFrame(uint8_t *buffer, size_t bufferSize)
    : buffer(buffer), bufferSize(bufferSize) {
  checksum = 0;
  dataLength = 0;
  isFilled = 0;
  index = 0;
}

uint16_t SensirionShdlcRxFrame::getUInt32(uint32_t *data) {
  dataLength -= 4;
  if (dataLength < 0) {
    return TODO_ERROR;
  }
  *data = (uint32_t)unstuffByte() << 24;
  *data |= (uint32_t)unstuffByte() << 16;
  *data |= (uint32_t)unstuffByte() << 8;
  *data |= (uint32_t)unstuffByte();
  return NO_ERROR;
}

uint16_t SensirionShdlcRxFrame::getInt32(int32_t *data) {
  return getUInt32((uint32_t *)data);
}

uint16_t SensirionShdlcRxFrame::getUInt16(uint16_t *data) {
  dataLength -= 2;
  if (dataLength < 0) {
    return TODO_ERROR;
  }
  *data = (uint16_t)unstuffByte() << 8;
  *data |= (uint16_t)unstuffByte();
  return NO_ERROR;
}

uint16_t SensirionShdlcRxFrame::getInt16(int16_t *data) {
  return getUInt16((uint16_t *)data);
}

uint16_t SensirionShdlcRxFrame::getUInt8(uint8_t *data) {
  dataLength -= 1;
  if (dataLength < 0) {
    return TODO_ERROR;
  }
  *data = unstuffByte();
  return NO_ERROR;
}

uint16_t SensirionShdlcRxFrame::getInt8(int8_t *data) {
  dataLength -= 1;
  if (dataLength < 0) {
    return TODO_ERROR;
  }
  *data = (int8_t)unstuffByte();
  return NO_ERROR;
}

uint16_t SensirionShdlcRxFrame::getBool(bool *data) {
  dataLength -= 1;
  if (dataLength < 0) {
    return TODO_ERROR;
  }
  *data = (bool)unstuffByte();
  return NO_ERROR;
}

uint16_t SensirionShdlcRxFrame::getFloat(float *data) {
  union {
    uint32_t uInt32Data;
    float floatData;
  } convert;
  uint16_t error = getUInt32(&convert.uInt32Data);
  *data = convert.floatData;
  return error;
}

uint16_t SensirionShdlcRxFrame::getBytes(uint8_t *data, size_t amount) {
  for (size_t i = 0; i < amount; i++) {
    dataLength -= 1;
    if (dataLength < 0) {
      return TODO_ERROR;
    }
    data[i] = unstuffByte();
  }
  return NO_ERROR;
}

uint16_t SensirionShdlcRxFrame::processHeader(void) {
  uint8_t start = buffer[index++];
  if (start != 0x7e) {
    return TODO_ERROR;
  }
  uint8_t address = unstuffByte();
  uint8_t command = unstuffByte();
  uint8_t state = unstuffByte();
  if (state) {
    return TODO_ERROR;
  }
  dataLength = unstuffByte();
  return NO_ERROR;
}

uint16_t SensirionShdlcRxFrame::processTail(void) {
  if (dataLength != 0) {
    return TODO_ERROR;
  }
  uint8_t expectedChecksum = ~checksum;
  uint8_t actualChecksum = unstuffByte();
  if (expectedChecksum != actualChecksum) {
    return TODO_ERROR;
  }
  uint8_t stop = buffer[index];

  if (stop != 0x7e) {
    return TODO_ERROR;
  }
  return NO_ERROR;
}

void SensirionShdlcRxFrame::reset(void) {
  dataLength = 0;
  index = 0;
  checksum = 0;
  isFilled = 0;
}

uint8_t SensirionShdlcRxFrame::unstuffByte(void) {
  uint8_t data = buffer[index++];

  if (data == 0x7d) {
    data = buffer[index++];
    data = data ^ (1 << 5);
  }
  checksum += data;
  return data;
}
