.. _ref-software:

========
Software
========

Building with Arduino IDE
=========================

Download the required external libraries in
:ref:`ref-software_libraries` and place them in the Arduino
``libraries/`` directory.
Open ``datagoo.ino``, compile and upload using the GUI.

Building with CMake
===================

**OS X/Linux**

To build on \*nix systems (tested on OS X 10.7):

.. code-block:: sh

   ./configure
   cd build
   make
