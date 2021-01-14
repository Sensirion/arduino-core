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
#ifndef SENSIRION_RX_FRAME_H_
#define SENSIRION_RX_FRAME_H_

#include <stdint.h>
#include <stdlib.h>

class SensirionRxFrame {

    friend class SensirionI2CCommunication;
    friend class SensirionShdlcCommunication;

  public:
    SensirionRxFrame(uint8_t buffer[], size_t bufferSize);
    uint16_t getUInt32(uint32_t& data);
    uint16_t getInt32(int32_t& data);
    uint16_t getUInt16(uint16_t& data);
    uint16_t getInt16(int16_t& data);
    uint16_t getUInt8(uint8_t& data);
    uint16_t getInt8(int8_t& data);
    uint16_t getBool(bool& data);
    uint16_t getFloat(float& data);
    uint16_t getBytes(uint8_t data[], size_t amount);

  private:
    uint8_t* _buffer = 0;
    size_t _bufferSize = 0;
    size_t _index = 0;
    size_t _numBytes = 0;
};

#endif /* SENSIRION_RX_FRAME_H_ */