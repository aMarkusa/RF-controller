# RF-controller
<img src="https://user-images.githubusercontent.com/83133831/192849107-02060bf8-b10f-4f93-b036-a338e187cc33.jpg" width="225" height="300"> <img src="https://user-images.githubusercontent.com/83133831/195907887-b2a06cc6-76e8-4d22-ac62-96b00d4f3e8c.jpg" width="225" height="300"> <img src="https://user-images.githubusercontent.com/83133831/195907612-2778f2e7-a8cb-42d4-889b-f2e9149f961c.jpg" width="225" height="300">



## Description
This was a part of a project course during my freshman year. My team's project was an RC car, and my area of responsibility was the controller. The controller included a joystick and programmable buttons. Furthermore, it had to display the speed of the car, be handheld, and be battery-powered.<br />
The controller is powered by a 7.4 Li-Po battery. The voltage is regulated by a LM2940T low-dropout regulator to 5V. This 5V powers the microcontroller and the display. The voltage is further regulated to 3.3V for the radio module by an LD33CV regulator. The case for the controller was 3D-printed by one of the team members.


### Controls
The readings from the joystick get processed by the microcontroller and sent to the car via the radio module. 4 programmable buttons can be used based on needs. We used one of the buttons to light a LED on the car.<br />
The display displayed the speed of the car, measured by a sensor on the car. It also displayed an informative message ("Obstacle ahead/behind") if any of the car ultrasonic sensors detected obstacles.

### PCB
The PCB was designed in EasyEDA and ordered from JLCPCB. Desing files in project repository.

### BOM
BOM is found in the [design files](https://github.com/aMarkusa/RF-controller/tree/main/Design%20files) repository. 

## Reflections
As this was a part of a course project it developed my ability to work in a team. Even though a lot of the development was done individually, the controller had to be compatible with the car, and therefore, the team had to actively discuss the design choices.<br />
This was my first time using a radio module, but thanks to good libraries, it was surprisingly easy to integrate. We did have some problems with the radio modules "freezing". I improved the code, resulting in better performance, but the modules still broke occasionally.


