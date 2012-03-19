.. _ref-libraries:

==================
Software Libraries
==================

EmonLib
=======
A significant portion of our code is based on the OpenEnergyMonitor project,
which is building an open source energy monitoring solution targeted at homes
in the UK/Europe (and to a lesser extent the US). Their library does much of
the heavy lifting for us in converting the raw voltage and current inputs coming
into the DataGoo device into power measurements.

We forked their code base primarily because we needed to output our readings
over GSM and to an SD logger, rather than over a custom (low-power) RF system
which OpenEMon uses to transmit their data.

`Open Energy Monitor website <http://openenergymonitor.org/emon/>`_
`GitHub (up-to-date code) for the library <https://github.com/openenergymonitor/EmonLib/>`_

SD Library
===========
The library to write text to SD cards is part of the Arduino standard library. It lets us write
to the SD card just by creating a file and then calling file.print().

`SD Library <http://arduino.cc/en/Reference/SD/>`_

SoftwareSerial Library
======================
This library is also part of the Arduino standard library and emulates a Serial port over
software, which is especially critical for interfacing with the GSM module (which takes input
and provides output over Serial).

`SoftwareSerial Library <http://arduino.cc/en/Reference/SoftwareSerial/>`_
