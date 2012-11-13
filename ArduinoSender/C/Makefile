all: serialport_init.o serialport_writebyte.o 

clean: 
	rm -f *.o *~

serialport_init.o: serialport_init.h serialport_init.c
	gcc -c serialport_init.c -o serialport_init.o

serialport_writebyte.o: serialport_writebyte.h serialport_writebyte.c
	gcc -c serialport_writebyte.c -o serialport_writebyte.o 

