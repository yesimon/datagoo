.. _ref-ac_math:

==============
AC Math Theory
==============

This page covers the mathematics behind calculating real power,
apparent power, power factor, RMS voltage and RMS current from
instantaneous Voltage and Current measurements of single phase AC
electricity. Discreet time equations are detailed since the
calculations are carried out in the Arduino in the digital
domain. There are also code snippets of the equations included below:

**Real Power**

Real power (also known as active power) is defined as the
power used by a device to produce useful work.

Mathematically it is the definite integral of voltage, :math:`u(t)`,
times current, :math:`i(t)`, as follows:

.. math::

   P = \frac{1}{T} \int_ \! u(n)\times i(n) \, \mathrm{d}t

**Equation 1. Real Power Definition.**

:math:`U` - Root-Mean-Square (RMS) voltage.

:math:`I` - Root-Mean-Square (RMS) current.

:math:`cos(\varphi)` - Power factor.

The discrete time equivalent is:

.. math::

   P = \frac{1}{T} \sum_{n=0}^{N-1} u(n)\times i(n)


**Equation 2. Real Power Definition in Discrete Time.**

:math:`u(n)` - sampled instance of :math:`u(t)`

:math:`i(n)` - sampled instance of :math:`i(t)`

:math:`N` - number of samples.

Real power is calculated simply as the average of N voltage-current
products. It can be shown that this method is valid for both
sinusoidal and distorted waveforms.

Code example of equation 2::

   for (n=0; n < numberOfSamples; n++)
   {
       //instV and instI calculation from raw ADC input goes here.
       instP = instV * instI;
       sumP += instP;
   }
   realPower = sumP / numberOfSamples;

**RMS Voltage and Current Measurement**

An RMS value is defined as the square root of the mean value of the
squares of the instantaneous values of a periodically varying
quantity, averaged over one complete cycle. The discrete time equation
for calculating voltage RMS is as follows:

.. math::

   U_{rms} = \sqrt{\frac{\sum_{n=0}^{N-1} u^2(n)}{N}}

Equation 3. Voltage RMS Calculation in Discrete Time Domain.

Current RMS is calculated using the same equation, only substituting
voltage samples, :math:`u(n)`, for current samples, :math:`i(n)`.

Code example of equation 3::

   for (n=0; n < numberOfSamples; n++)
   {
       //instV calculation fsqV = instV * instV;
       sumV += sqV;
   }
   Vrms = sqrt(sumV / numberOfSamples);

Substitute V for I for current.

**Apparent Power and Power Factor**

Apparent power is calculated, as follows:

:math:`Apparent power = RMS Voltage x RMS current`

and the power factor:

:math:`Power Factor = Real Power / Apparent Power`

**Bringing it all together**

The following snippet carries out all the measurements above::

   for (n=0; n < numberOfSamples; n++)
   {
       //instV and instI calculation from raw ADC input goes here.
       sqV = instV * instV;
       sumV += sqV;
       sqI = instI * instI;
       sumI += sqI;
       instP = instV * instI;
       sumP +=instP;
   }
   Vrms = sqrt(sumV / numberOfSamples);
   Irms = sqrt(sumI / numberOfSamples);
   realPower = sumP / numberOfSamples;
   apparentPower = Vrms * Irms;
   powerFactor = realPower / apparentPower;

Thats it, thats the core of single phase AC power measurment
calculation.

This page is based on Atmel's AVR465 appnote page 12-15 which can be
found `here <http://www.atmel.com/dyn/resources/prod_documents/doc2566.pdf>`_.
