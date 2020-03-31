# FlightGear Bridge

The FlightGear alternative to the current PX4's mainstream simulator Gazebo.

This stand-alone application adding the possibility of the use of FlightGear simulator and was tested on the Rascal airplane and TF-G1 autogyro simulation models.

It connects to FG (over UDP generic protocol) and transform the data to PX4 over TCP Mavlink packets.

### How to run the development version:

1) Install FlightGear and . You should be able to run Flightgear by ```fgfs``` from command line.
2) Install required aircraft models ([Rascal_110](http://wiki.flightgear.org/Rascal_110), [ThunderFly TF-G1](https://github.com/ThunderFly-aerospace/FlightGear-TF-G1))
3) Set write permissions to the Protocol folder of FG (in ubuntu /usr/share/games/flightgear/Protocols)
4) If flightgear data folder is different than ```/usr/share/games/flightgear/```, set that folder to ```FGDATA``` enviroment variable.
5) Open [QgroundControl](http://qgroundcontrol.com/)
6) In PX4Firmware folder run: ```make px4_sitl flightgear_plane``` for plane or ```make px4_sitl flightgear_TF-G1``` for autogyro


### Credits

 FlightGear bridge was initially developed at ThunderFly s.r.o. by Vít Hanousek <info@thunderfly.cz>
