# RF-controller
## Description
This was apart of a project course during my freshman year. My teams project was a RC-car, and my area of responsibility was the controller. The controller had include a joystick and programmable buttons. Furthermore, it had to display the speed of the car, be handheld, and battery-powered. 

## Working principle

### Power
The controller is powered by a ???? Li-Po battery. The voltage is regulated by a ?????-reulator to 5V. This 5V powers the microcontroller and the display. The voltage is further regulated to 3.3V for the radio module by a LD33CV-regulator.

### Controls
The readings from the joystick gets processed by the microcontroller and sent to the car via the radio module. There are also 4 programmable buttons which can be used based on needs. We used one of the buttons to light a LED on the car.<br />
The display displayed the speed of the car, mesured by a sensor on the car. It also displayed an informative message ("Obstacle ahead/behind") if any of the cars ultrasonic sensors detected obstacles.

#### PCB
PCB was designed in EasyEDA and ordered from JLCPCB. Desing files in project repository.

#### Case
The case was 3D-printed.
