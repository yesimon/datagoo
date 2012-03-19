.. _ref-overview:

========
Overview
========

.. _ref-overview_mission:

Mission
=======

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

Intended Purpose
================
* Easy to use power measurement device
* Cheap to make
* Ability to monitor current and voltage on 1 and 3 phase system
* Logs the data on an SD card
* GSM connectivity provides two functionality:
* Sends an SMS if the voltage drops for more than 5 minutes
* Sends a daily "power collected" SMS with the power generated during that day
* Flexible design allows for reprogramming and hacking

Intended Users
==============
* Engineering graduates and other workers with basic technology fluency.
* Designed so that it can be manufactured *relatively* easily.

Target Capabilities
===================
"The generator produces and transmits 220V AC (which eventually either
gets dropped down to 110 for household use, or used at 220 for certain
tools/machines -- but the monitor should expect around 220V). There is
also a controller circuit which maintains the frequency around 60Hz
(+/- a few hertz) by keeping the load on the system at a constant
level (i.e. using a "dump load" when necessary to keep the turbine
spinning at a constant speed when the system is underloaded). Our
current system is single phase, but there was a question/suggestion to
support multiple input channels, so that power calculations for
multi-phase systems could be done in software. " -Stephen, Alterna
Foundation

Power Measurement Specifications
================================
* Nominal voltage is 220V. Maximum we've seen was 225, minimum 219. There may be brief moments (around half a second or less) when it is much lower, down to 150V, when an extra heavy load is switched on.
* Nominal frequency is 60 Hz. Minimum 59, maximum 62.5.
* Single Phase AC.
* Current varies significantly depending on the season (water flow). Max in the rainy season is approximately 55A. Right now it's about 30A. Dry season production may drop down as low as 15A, but we aren't sure on that yet.
