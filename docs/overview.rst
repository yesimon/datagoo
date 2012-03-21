.. _ref-overview:

================
Project Overview
================

.. _ref-overview_mission:

Alterna's Mission
==================

Alterna on their mission:

	Alterna is a Center for Innovation and Entrepreneurship based in 
	Quetzaltenango, Guatemala. Our mission is to develop technologies and build 
	local businesses that satisfy basic needs and provide economic growth 
	opportunities for the Guatemalan people in an environmentally sustainable 
	way.


One of their major projects are micro-hydro plants in rural areas:

	Two million Guatemalans live without access to electricity. The vast 
	majority of these people live in rural areas. Expansion of the national grid 
	to the areas that are left unserved is increasingly difficult due to 
	geographic constraints.

	Guatemala is also a country with great potential for utilization of 
	hydroelectric power. The extensive rainfall and mountainous terrain of the 
	country provide optimal conditions for generating electricity with water 
	power.
| http://www.alterna-la.org/en
| http://www.alterna-la.org/en/projects/microhydro

	
Our Mission
============

Alterna currently has no good ways to monitor how much power these
micro-hydro plants produce, because off-the-shelf systems designed for
ultra-high current environments are expensive. Measuring power generation is
useful both to understand the efficacy of these installations and because it
is prerequisite to applying for carbon grants which can offset the costs of
development. Therefore, we prototyped a circuit based on the open-source
Arduino platform which measures single-phase AC power coming out of the
installation, logs it to an SD card, and transmits daily SMS status updates
over a GSM network. The circuit can handle household voltages (~240V),
currents up to 100A, and costs around $200 (and as little as $100 when 
produced at scale).

Carbon Offsets
---------------
Becoming accredited to sell carbon offsets is not a trivial process and having
information on kWh generated is only one part of it. Nevertheless, organizations
like `ClimateTrust <http://www.climatetrust.org/>`_ exist to help organizations
through the bureaucratic steps of the process, but they can't do anything
without data, so we feel confident that this device will be an important part
of offsetting micro-hydro costs with carbon offsets.

The U.S. Department of Energy Clean Energy Application Center authored a report
on selling carbon offsets, which you can read
`here <http://www.chpcenternw.org/NwChpDocs/SellingCarbonOffsets.pdf>`_.

Specs In More Detail
---------------------
**Intended Purpose**

* Easy to use power measurement device
* Cheap to make
* Able to monitor current and voltage on 1 phase system
* Logs data to an SD card
* GSM connectivity provides two functionality:
* Sends an SMS if the voltage drops for an extended period
* Sends a daily "power collected" SMS with the power generated during that day
* Flexible design allows for reprogramming and hacking

**Intended Users**

* Engineering graduates and other workers with basic technology fluency.
* Designed so that it can be manufactured *relatively* easily.

**Power Measurement Specifications**

* Nominal voltage is 220V. Maximum we've seen was 225, minimum 219. There may be brief moments (around half a second or less) when it is much lower, down to 150V, when an extra heavy load is switched on.
* Nominal frequency is 60 Hz. Minimum 59, maximum 62.5.
* Single Phase AC.
* Current varies significantly depending on the season (water flow). Max in the rainy season is approximately 55A. Right now it's about 30A. Dry season production may drop down as low as 15A, but we aren't sure on that yet.
