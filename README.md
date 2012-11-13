OptimalControlRcProject
=======================

Collection of files required for the optimal control rc project at www.rus.rub.de.

Use of ArduinoSender
====================
ArduinoSender uses an arduino uno board to read bytes from the serial port the arduino is connected to, 
and to send two channels to a mini-z or dnano toy rc car. Used it as follows: 
- setup arduino circuit described in ArduinoSender/Fritzing/ArduinoSenderCircuitBoard.fzz
- connect arduino to PC, start arduino ide, open ArduinoSender/arduinoSender/arduinoSender.ino
- compile&upload arduinoSender.ino to arduino
- switch on 12V power supply to circuit ArduinoSenderCircuitBoard.fzz
- switch on mini-z porsche (other models will require to adjust binding signal data in arduinoSender.ino) 
- verify red LED on rc car is on constantly, flashing LED indicates that binding was not successful
- Run tests/loop_steering_signal. You should see the steering wheels move periodically.  

Vocab
=====
arduino - easy-to-use, low-cost microcontroller; used to generate rc signals in this project
arduino ide - simple development environment for arduino programming (start it with "arduino" on linux, look for executable on windows)
arduino uno - particular type and generation of arduino board
binding - process that connects 2.4GHz sender to receiver, necessary because several senders and receivers may be used at the same time
binding signal data - data that uniquely identifies 2.4GHz receiver
dnano - type of toy rc car, particularly small, scale is 1:43
kyosho - vendor of mini-z and dnano toy rc cars etc.
mini-z - type of toy rc car, scale is about 1:25  
pairing - see binding
rc - short for remote-control, remote-controlled etc.
