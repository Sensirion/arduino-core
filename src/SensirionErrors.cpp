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
#include "SensirionErrors.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

void errorToString(uint16_t error, char errorMessage[256]) {
    uint16_t highLevelError = error & 0xFF00;
    uint16_t lowLevelError = error & 0x00FF;
    switch (highLevelError) {
        case HighLevelError::NoError:
            if (!error) {
                strcpy(errorMessage, "No error");
                return;
            }
            break;
        case HighLevelError::WriteError:
            switch (lowLevelError) {
                case LowLevelError::SerialWriteError:
                    strcpy(errorMessage, "Error writing to serial");
                    return;
            }
        case HighLevelError::ReadError:
            switch (lowLevelError) {
                case LowLevelError::NonemptyFrameError:
                    strcpy(errorMessage, "Frame already contains data");
                    return;
                case LowLevelError::NoDataAvailableError:
                    strcpy(errorMessage, "No data received");
                    return;
                case LowLevelError::BufferSizeError:
                    strcpy(errorMessage, "Not enough space in buffer");
                    return;
                case LowLevelError::StartByteError:
                    strcpy(errorMessage, "First byte doesn't match start byte");
                    return;
                case LowLevelError::NoDataError:
                    strcpy(errorMessage, "Only start byte received");
                    return;
            }
        case HighLevelError::DeviceError: {
            char format[] = "Device error, status register: %x";
            sprintf(errorMessage, format, lowLevelError);
            return;
        }
        case HighLevelError::TxFrameError:
            switch (lowLevelError) {
                case LowLevelError::BufferSizeError:
                    strcpy(errorMessage, "Not enough space in buffer");
                    return;
            }
        case HighLevelError::RxFrameError:
            switch (lowLevelError) {
                case LowLevelError::BufferSizeError:
                    strcpy(errorMessage, "Not enough space in buffer");
                    return;
                case LowLevelError::StartByteError:
                    strcpy(errorMessage, "First byte doesn't match start byte");
                    return;
                case LowLevelError::StopByteError:
                    strcpy(errorMessage, "Last byte doesn't match stop byte");
                    return;
                case LowLevelError::NoDataError:
                    strcpy(errorMessage, "No more data in frame");
                    return;
                case LowLevelError::ChecksumError:
                    strcpy(errorMessage, "Checksum is wrong");
                    return;
            }
    }
    strcpy(errorMessage, "Error processing error");
    return;
}
