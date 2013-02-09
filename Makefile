#
# This Makefile has been added for convenience: Delete all object and executable files 
# in this directory and its subdirectories by issuing command "make clean".
#
all:
	make -C ArduinoSender all
clean:
	make -C ArduinoSender clean
