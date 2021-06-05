GCC= gcc
CFLAGS = -Wall -Werror 
CLIBS = 
INCLUDES = -lm

all: gpio.out
	./gpio.out

gpio.out: main.c gpio.o  
	$(GCC) $(CFLAGS) main.c gpio.o -o gpio.out  $(INCLUDES) $(CLIBS)

gpio.o: gpio.c gpio.h
	$(GCC) $(CFLAGS) gpio.c -c  $(INCLUDES) $(CLIBS)
	
clean:
	rm -f *.out
	rm -f *.o