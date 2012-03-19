.. _ref-quickstart:

==========
Quickstart
==========

Installation
============

This guide is for installing a datagoo with hardware and firmware
preinstalled. It details the steps for configuring and attaching the
datagoo to a power supply, so that it is ready to send text
notifications.

#. Format an SD card to FAT16. (This is by default for SD cards, but
   please double check).

#. Create file ``CELL.TXT`` containing the mobile number to notify of
   power usage statistics and alerts. An example ``CELL.TXT`` file::

      5731921102

#. Unscrew the four screws attaching the case to the board.

   .. image:: images/case_exploded_sim.png
      :height: 350 px

#. Place an activated SIM card in the SIM card holder. (Test that the
   SIM card is capable of sending texts via a normal mobile phone)

#. Flip the ON/OFF switch to ON.

   .. image:: images/case_exploded_switch.png
      :height: 350 px

#. Screw the case back together.

#. Plug in USB charger.

   .. warning::

      The USB charger should not be a computer. Plugging in the
      computer USB charger and line voltage to the voltage terminals
      at the same time will destroy both the datagoo and your
      computer.

#. Connect current sensor. The current sensor should be clamped around
   only one wire.

   .. image:: images/attached_ct.jpg
      :height: 400 px

#. Strip a wire with the generated voltage. (Keep **UNPLUGGED**)

   .. warning::

      Stripping and connecting a live (plugged in) wire will cause
      electrocution to the user.

#. Connect voltage terminals to the stripped wire.

#. Plug the stripped wire to the power source.

#. Check the LED display for kW generated and you are ready.
