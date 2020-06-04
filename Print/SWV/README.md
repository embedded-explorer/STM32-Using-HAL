<h2 align="center">Debug Using Serial Wire Viewer</h2>

It is very simple to debug applications for STM32  using printf like statements with the help of serial wire debugger (SWV). This is a simple application which prints text onto console. 

#### Follow these simple steps to enable the SWV.
1. Enable SWV in Debug configurations

![](1.png)

2. Modify write function in syscalls.c and implement function to write data to ITM

3. Start debug session and choose SWV ITM Data Console under Window/ Show View/ SWV

4. Select Port 0 as stimulus port under Configure Trace and Start Trace

![](2.png)

