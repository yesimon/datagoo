.. _ref-electrical:

==========
Electrical
==========


.. warning::

   Before trying to have the PCB manufactured, please contact
   nhallsny@stanford.edu for the correct settings. For components and
   designators, check out the Bill of Materials

.. image:: images/electrical_layout.png

Schematic Errors
================
There are no known errors in the below schematic.

Layout Errors
=============

There are several known errors in the layout:

1. The LDO_EN is not connected from the GSM Module
2. The GSM_TXD is not connected to the GSM Module
3. The MOSI pin on the AVR programming header is RST, and should be MOSI
4. The Zener diode footprint does not have a direction indicator
5. The LED footprint does not have a clear direction indicator
6. The footprint for SW1 is incorrect, but hackable
7. The large 100uF capacitor doesn't have a polarity marking
8. The same net is connected to two of the seven segment display pins

3D Layout
=========

.. image:: images/electrical_3d_layout.png

Schematic
=========

.. image:: images/electrical_schematic.png
   :width: 100 %
   :target: _downloads/electrical_schematic.pdf

:download:`Electrical Schematic <images/electrical_schematic.pdf>`.
