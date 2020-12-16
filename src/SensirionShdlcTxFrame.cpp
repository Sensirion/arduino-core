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
 *   list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   software without specific prior written permission.
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

#include "SensirionShdlcTxFrame.h"

#include <stdint.h>

#include "SensirionErrors.h"

SensirionShdlcTxFrame::SensirionShdlcTxFrame(uint8_t* buffer, size_t bufferSize)
    : buffer(buffer), bufferSize(bufferSize) {
    checksum = 0;
    index = 0;
    isFinished = false;
}

uint16_t SensirionShdlcTxFrame::addUInt32(uint32_t data) {
    int16_t error = addUInt8((uint8_t)((data & 0xFF000000) >> 24));
    error |= addUInt8((uint8_t)((data & 0x00FF0000) >> 16));
    error |= addUInt8((uint8_t)((data & 0x0000FF00) >> 8));
    error |= addUInt8((uint8_t)((data & 0x000000FF) >> 0));
    return error;
}

uint16_t SensirionShdlcTxFrame::addInt32(int32_t data) {
    return addUInt32((uint32_t)data);
}

uint16_t SensirionShdlcTxFrame::addUInt16(uint16_t data) {
    uint16_t error = addUInt8((uint8_t)((data & 0xFF00) >> 8));
    error |= addUInt8((uint8_t)((data & 0x00FF) >> 0));
    return error;
}

uint16_t SensirionShdlcTxFrame::addInt16(int16_t data) {
    return addUInt16((uint16_t)data);
}

uint16_t SensirionShdlcTxFrame::addUInt8(uint8_t data) {
    if (index + 2 > bufferSize) {
        return TODO_ERROR;
    }
    switch (data) {
        case 0x11:
        case 0x13:
        case 0x7d:
        case 0x7e:
            // byte stuffing is done by inserting 0x7d and inverting bit 5
            buffer[index++] = 0x7d;
            buffer[index++] = data ^ (1 << 5);
            break;
        default:
            buffer[index++] = data;
    }
    checksum += data;
    return NO_ERROR;
}

uint16_t SensirionShdlcTxFrame::addInt8(int8_t data) {
    return addUInt8((uint8_t)data);
}

uint16_t SensirionShdlcTxFrame::addBool(bool data) {
    return addUInt8((uint8_t)data);
}

uint16_t SensirionShdlcTxFrame::addFloat(float data) {
    union {
        uint32_t uInt32Data;
        float floatData;
    } convert;

    convert.floatData = data;
    return addUInt32(convert.uInt32Data);
}

uint16_t SensirionShdlcTxFrame::addBytes(uint8_t* data, size_t dataLength) {
    uint16_t error = 0;
    for (size_t i = 0; i < dataLength; i++) {
        error |= addUInt8(data[i]);
    }
    return error;
}

uint16_t SensirionShdlcTxFrame::begin(uint8_t command, uint8_t address,
                                      uint8_t dataLength) {
    buffer[index++] = 0x7e;
    uint16_t error = addUInt8(address);
    error |= addUInt8(command);
    error |= addUInt8(dataLength);
    return error;
}

uint16_t SensirionShdlcTxFrame::finish(void) {
    uint16_t error = addUInt8(~checksum);
    if (index + 1 > bufferSize) {
        return TODO_ERROR | error;
    }
    buffer[index++] = 0x7e;
    isFinished = true;
    return error;
}

uint16_t SensirionShdlcTxFrame::reset(void) {
    isFinished = false;
    index = 0;
    checksum = 0;
    return NO_ERROR;
}
