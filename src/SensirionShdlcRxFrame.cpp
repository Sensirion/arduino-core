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

#include "SensirionShdlcRxFrame.h"

#include <stdint.h>
#include <stdlib.h>

#include "SensirionErrors.h"

SensirionShdlcRxFrame::SensirionShdlcRxFrame(uint8_t buffer[],
                                             size_t bufferSize)
    : _buffer(buffer), _bufferSize(bufferSize), _index(0), _dataLength(0),
      _isFilled(false) {
}

uint16_t SensirionShdlcRxFrame::getUInt32(uint32_t& data) {
    if (_dataLength < 4) {
        return RxFrameError | NoDataError;
    }
    data = (uint32_t)_buffer[_index++] << 24;
    data |= (uint32_t)_buffer[_index++] << 16;
    data |= (uint32_t)_buffer[_index++] << 8;
    data |= (uint32_t)_buffer[_index++];
    _dataLength -= 4;
    return NoError;
}

uint16_t SensirionShdlcRxFrame::getInt32(int32_t& data) {
    return getUInt32((uint32_t&)data);
}

uint16_t SensirionShdlcRxFrame::getUInt16(uint16_t& data) {
    if (_dataLength < 2) {
        return RxFrameError | NoDataError;
    }
    data = (uint16_t)_buffer[_index++] << 8;
    data |= (uint16_t)_buffer[_index++];
    _dataLength -= 2;
    return NoError;
}

uint16_t SensirionShdlcRxFrame::getInt16(int16_t& data) {
    return getUInt16((uint16_t&)data);
}

uint16_t SensirionShdlcRxFrame::getUInt8(uint8_t& data) {
    if (_dataLength < 1) {
        return RxFrameError | NoDataError;
    }
    data = _buffer[_index++];
    _dataLength -= 1;
    return NoError;
}

uint16_t SensirionShdlcRxFrame::getInt8(int8_t& data) {
    if (_dataLength < 1) {
        return RxFrameError | NoDataError;
    }
    data = (int8_t)_buffer[_index++];
    _dataLength -= 1;
    return NoError;
}

uint16_t SensirionShdlcRxFrame::getBool(bool& data) {
    if (_dataLength < 1) {
        return RxFrameError | NoDataError;
    }
    data = (bool)_buffer[_index++];
    _dataLength -= 1;
    return NoError;
}

uint16_t SensirionShdlcRxFrame::getFloat(float& data) {
    union {
        uint32_t uInt32Data;
        float floatData;
    } convert;
    uint16_t error = getUInt32(convert.uInt32Data);
    data = convert.floatData;
    return error;
}

uint16_t SensirionShdlcRxFrame::getBytes(uint8_t data[], size_t maxAmount) {
    if (_dataLength < 1) {
        return RxFrameError | NoDataError;
    }
    size_t readAmount = maxAmount;
    if (_dataLength < maxAmount) {
        readAmount = _dataLength;
    }
    for (size_t i = 0; i < readAmount; i++) {
        data[i] = _buffer[_index++];
    }
    _dataLength -= readAmount;
    return NoError;
}

void SensirionShdlcRxFrame::reset(void) {
    _dataLength = 0;
    _index = 0;
    _isFilled = 0;
}
