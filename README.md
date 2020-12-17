# Sensirion_Core_Arduino_Library

This library provides SHDLC and I2C(*coming soon*) protocol implementations for
Sensirion sensors. There shouldn't be a reason to use it directly, but is
required by the sensor driver libraries provided here:

- [SVM40]() - *coming soon*
- [SFA3x]() - *coming soon*

# More Drivers

Not looking for Ardunio drivers? Check out our other drivers here:

- [Embedded](https://github.com/Sensirion/info#repositories)
- [Python](https://github.com/Sensirion/info#python-drivers)

# Usage

## SHDLC

This library provides the following classes for communication with Sensirion
Sensors using the SHDLC protocol.
- `SensirionShdlcTxFrame`
- `SensirionShdlcRxFrame`
- `SensirionShdlcCommunication`

### Example Usage
First initialize an instance of `SensirionShdlcTxFrame` and
`SensirionShdlcRxFrame` with a properly sized buffer. A good worst case
estimation for the buffer size is `2 * (n+6)` where `n` is the number of bytes
you want to send. After that you can build your frame by first calling
`begin()`. Information about the correct COMMAND and ADDRESS can be found on
the data sheet of your sensor. Then you can add data to the frame by using
different add member functions. See the code below for examples. After adding
your data finish the frame by calling `finish()`.

To send this frame to the sensor you first need to initialize the correct
Stream object (Serial,UART,...) to talk to your sensor. Don't forget to also
call the `.begin()` function with the right configuration. Then call the static
function `sendFrame()` form `SensirionShdlcCommunication` as shown below. You
need to replace `STREAMOBJECT` with the initialized Stream object of your
choice. To receive the reply from the sensor call the following with the same
Stream object.

You can then decode the received frame by first calling `processHeader()` and
then decoding the frame by using the different get members to decode your data.
Lastly call `processTail()` to check the checksum of the data. If a non zero
error code is returned the data is corrupted and must be discarded.

```cpp
uint8_t txBuffer[256];
uint8_t rxBuffer[256];

SensirionShdlcTxFrame txFrame(txBuffer, 256);
SensirionShdlcRxFrame rxFrame(rxBuffer, 256);

txFrame.begin(COMMAND, ADDRESS, DATALENGTH);

txFrame.addUInt8(UINT8);
txFrame.addUInt32(UINT32);

txFrame.finish();

SensirionShdlcCommunication::sendFrame(txFrame, STREAMOBJECT);

SensirionShdlcCommunication::receiveFrame(rxFrame, STREAMOBJECT);

rxFrame.processHeader();

rxFrame.getUInt16(UINT16);
rxFrame.getFloat(FLOAT);

error = rxFrame.processTail();
```
