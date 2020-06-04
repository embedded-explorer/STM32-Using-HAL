<h2 align="center">Testing CAN Peripheral</h2>

CAN Normal Mode Testing, This example makes use of 2 STM32 devices with CAN.<br>

* Node 1 sends a message containing an LED number every 1 second.
* Node 2 receives this message and glows the corresponding LED. 
* Node 1 requests Temperature data from Node 2 every 5 second.
* Node 2 sends temperature data upon request.

## Note

This example is incomplete
