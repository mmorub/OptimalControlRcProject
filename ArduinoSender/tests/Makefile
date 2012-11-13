all: loop_steering_angle

loop_steering_angle: loop_steering_angle.c ../C/serialport_init.o ../C/serialport_writebyte.o
	gcc -I../C/ ../C/serialport_init.o ../C/serialport_writebyte.o -o loop_steering_angle loop_steering_angle.c 

../C/serialport_init.o:
	make -C../C all

../C/serialport_writebyte.o:
	make -C../C all

clean:
	rm -f loop_steering_angle *~


