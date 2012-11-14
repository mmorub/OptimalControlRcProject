#
# This Makefile has been added for convenience: Delete all object and executable files 
# in this directory and its subdirectories by issuing command "make clean".
#
all:
	make -C ArduinoSender/Makefile all
clean:
	make -C ArduinoSender/Makefile clean
