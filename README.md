LED-Coffee-table
================
This project will create a 2 dimensional LED board that can track and display patterns of hand motions
over the board within a certain proximity. The mbed development board receives signals from the IR receivers, stores it into an internal representation of the surface, then uses a concurrently running thread to update the NeoPixels appropriately. 

Uses:  
  a FRDM-KL25Z platform
  4 CD74HC4067 multiplexers (extendable)
  7 NeoPixel light strips (extendable)
  7x8 array of TSOP 32823 38 kHz IR Receivers (extendable)
  6x7 array of IR-204 940nm IR Emitters (extendable)
  A lot of wires

Schedule
• October 21: Project charter (this document)
• October 28: Choice of sensors finalized after discussion with GSIs.
• November 4: Statecharts simulation model with logic. Test devices chosen
• November 11: Installed software for development, more parts testing. construction of board
• November 18: Mini project update: Demonstrate sensor action
• November 25: Measure sensor accuracy and calibrate appropriately
• December 2: Continue debugging. If no bugs, add features
• December 9: System testing, measure false positives, assess sensitivity and calibrate for different test
environments
If time, add additional features (ideas in mind: pattern recognition, image file display, etc.)
• December 16: Demonstration video made, powerpoint prepared.
• December 17: Final presentation and demo.
• December 19: Project report and video turned in.

README will be updated as project progresses
