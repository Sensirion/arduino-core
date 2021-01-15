Changelog
=========

All notable changes to this project will be documented in this file.

The format is based on `Keep a Changelog <https://keepachangelog.com/en/1.0.0/>`_
and this project adheres to `Semantic Versioning <https://semver.org/spec/v2.0.0.html>`_.

`Unreleased`_
-------------

Breaking
........

- Change interface of ``errorToString()`` function to include length of the
  provided buffer.

Removed
.......

- Removed ``reset()`` function from ``SensirionI2CTxFrame`` since the
  functionality is not needed.


`0.3.0`_ 2021-01-13
-------------------

Added
.....

- Core implementation for I2C communication. This includes a RX and TX frame
  and a I2C communication class.

Changed
.......

- SHDLC and I2C RX frame inherit from a RX frame base class.
- ESP8266 test board from esp8266:esp8266:arduino to esp8266:esp8266:generic.
- Sorted errors into general, SHDLC and I2C errors.
- Replace C style casts with ``static_cast``.


`0.2.0`_ 2021-01-11
-------------------

Added
.....

- Explanation what SHDLC is in README.
- ``SensirionErrors.h`` to ``SensirionCoreArduinoLibrary.h``.
- ``sendAndReceiveFrame()`` function to ``SensirionShdlcCommunication``. This
  function combines ``sendFrame()`` and ``receiveFrame()`` into one function and
  adds additional error checking.

Changed
.......

- Rename DeviceError to ExecutionError.
- Move check for execution error after the whole frame is read and checksum is
  checked. This prevents that a wrong checksum can't be displayed as an
  execution error.

Removed
.......

- ``reset()`` function from ``SensirionShdlcTxFrame`` and ``SensirionShdlcRxFrame``,
  since one can just create a new frame object which has the same effect.

`0.1.0`_ 2021-01-07
-------------------

- Initial release


.. _Unreleased: https://github.com/Sensirion/Sensirion_Core_Arduino_Library/compare/0.3.0...main
.. _0.3.0: https://github.com/Sensirion/Sensirion_Core_Arduino_Library/compare/0.2.0...0.3.0
.. _0.2.0: https://github.com/Sensirion/Sensirion_Core_Arduino_Library/compare/0.1.0...0.2.0
.. _0.1.0: https://github.com/Sensirion/Sensirion_Core_Arduino_Library/releases/tag/0.1.0
