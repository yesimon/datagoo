.. DataGoo documentation master file, created by
   sphinx-quickstart on Sun Mar  4 19:27:08 2012.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

DataGoo Documentation
=====================

Contents:

.. toctree::
   :maxdepth: 1

   quickstart
   overview
   ac_math
   systems_design
   hardware_parts
   components
   cost
   electrical
   mechanical
   software
   software_libraries


Project Description
===================
Alterna installs micro-hydroelectric systems in off-grid locations in
Guatemala. They currently have no good ways to monitor how much power these
micro-hydro-plants produce, because off-the-shelf systems designed for
ultra-high current environments are expensive. Measuring power generation is
useful both to understand the efficacy of these installations and because it is
prerequisite to applying for carbon grants which can offset the costs of
development. Therefore, we are developing a circuit based on the open-source
Arduino platform which measures the current and voltage coming out of the
installation, logs it to an SD card, and transmits daily SMS status updates over
a GSM network. The circuit will be able to handle household voltages (~240V),
currents up to 100A, and will cost around $200.


Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`
