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
#ifndef _SENSIRION_CORE_ARDUINO_LIBRARY_H_
#define _SENSIRION_CORE_ARDUINO_LIBRARY_H_

static const uint16_t NO_ERROR = 0x0000;

static const uint16_t TODO_ERROR = 0xf000; // Error code for not yet defined errors.

// USER LEVEL ERRORS:
static const uint16_t WRITE_ERROR = 0x1000;
static const uint16_t READ_ERROR = 0x2000;

// LOWER LEVEL ERRORS:
static const uint16_t SERIAL_ERROR = 0x0100;
static const uint16_t FRAME_ERROR = 0x0200;
static const uint16_t DEVICE_ERROR = 0x0300;
static const uint16_t DRIVER_ERROR = 0x0400;

// WRITE_ERROR->SERIAL_ERROR:
static const uint16_t NOT_ALL_BYTES_WRITTEN = 0x0001;

// READ_ERROR->DRIVER_ERROR:
static const uint16_t FRAME_NOT_EMPTY = 0x0001;
static const uint16_t BUFFER_OVERFLOW = 0x0002;

// READ_ERROR->FRAME_ERROR:
static const uint16_t START_BYTE = 0x0001;

// READ_ERROR->SERIAL_ERROR:
static const uint16_t NO_DATA_AVAILIBLE = 0x0001;
static const uint16_t NO_DATA_READ = 0x0002;
static const uint16_t TOO_MUCH_DATA_READ = 0x0004;

#include "SensirionShdlcCommunication.h"
#include "SensirionShdlcRxFrame.h"
#include "SensirionShdlcTxFrame.h"

#endif /* _SENSIRION_CORE_ARDUION_LIBRARY_H_ */
