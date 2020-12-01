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
#include "SensirionShdlcCommunication.h"

uint16_t SensirionShdlcCommunication::begin(class Stream *serial) {
  this->serial = serial;
  return NO_ERROR;
}

uint16_t
SensirionShdlcCommunication::sendFrame(class SensirionShdlcTxFrame &frame) {
  size_t writtenBytes = serial->write(&frame.buffer[0], frame.index);
  if (writtenBytes != frame.index) {
    return WRITE_ERROR;
  }
  return NO_ERROR;
}

uint16_t
SensirionShdlcCommunication::receiveFrame(class SensirionShdlcRxFrame &frame) {
  if (frame.isFilled) {
    return NONEMPTY_FRAME_ERROR;
  }
  if (!serial->available()) {
    return NO_DATA_ERROR;
  }
  if (!frame.bufferSize) {
    return TODO_ERROR;
  }
  frame.buffer[0] = serial->read();
  if (frame.buffer[0] != 0x7e) {
    return FRAME_START_BYTE_ERROR;
  }
  size_t readBytes =
      serial->readBytesUntil(0x7e, &frame.buffer[1], frame.bufferSize - 1);
  if (!readBytes) {
    return READ_ERROR;
  }
  if (readBytes + 1 >= frame.bufferSize) {
    return BUFFER_SIZE_ERROR;
  }
  frame.buffer[readBytes + 1] = 0x7e;
  frame.isFilled = true;
  return NO_ERROR;
}
