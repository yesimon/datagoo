=======
Datagoo
=======

Project Description
===================

DataGoo is an open-source device designed to measure power generation
or consumption from small-scale electrical installations--cheaply. We
designed it to be initially deployed in `microhydro plants
<http://www.alterna-la.org/en/projects/microhydro>`_ run by `Alterna
<http://www.alterna-la.org/en>`_ in Guatemala.

DataGoo builds heavily on a similar, open-source device called the
`OpenEnergyMonitor <http://www.openenergymonitor.org>`_ (emon). The emon system
involves a transmitting device, a receiving device, and
display software and is targeted for in-home deployment. Our device is
designed for deployment in the field, so it uses a simpler output
interface: text messages over GSM and logging to an SD card.

Software Development
====================

Building with Arduino IDE
-------------------------

Download the required external libraries (see http://datagoo.readthedocs.org)
and place them in the Arduino ``libraries/`` directory. Open
``datagoo.ino``, compile and upload using the GUI.


Building With CMake
-------------------

**OS X/Linux**

To build on *nix systems (tested on OS X 10.7)::

   ./configure
   cd build
   make
