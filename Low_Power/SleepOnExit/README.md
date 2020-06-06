<h2 align="center"> Tesing Sleep on Exit</h2>

Write an application in which Timer2 triggers an update interrupt every 10ms and sends some data using UART2. Putting unused IOs to analog mode reduces power consumption.

#### Comparison of power consumption with different clock frequencies

| Clock Frequency      | Run Mode | Sleep on Exit |
| ----------- | ----------- | ------------------ |
| HSI_PLL 50MHz     | 12.3mA | 9.4mA |
| HSE_PLL 50MHz   | 12.0mA | 9.1mA |
| HSI 16MHz | 6.7mA | 5.9mA |
| HSE 8MHz | 5.3mA | 5mA |

#### Do You Know Running UART at higher baud rate saves power
HSE - 8MHz, Sleep on Exit Mode enabled

| Baud Rate | Current Consumed |
| ----------- | ----------- |
| 9600 | 6.4mA |
| 115200 | 5mA |
| 460800 | 4.6mA |

#### Clock gating
* Disable clocks for peripherals not used
* Disable clock for UART2 while entering sleep mode
* Baud Rate = 460800, HSE = 8MHz
* Power Consumption = 4.5mA

