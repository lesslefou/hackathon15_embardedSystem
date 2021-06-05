GCC= gcc
CFLAGS = -Wall -Werror 
CLIBS = 
INCLUDES = -lm

all: gpio.out
	./gpio.out

gpio.out: main.c gpio.o common_dht_read.o pi_dht_read.o  
	$(GCC) $(CFLAGS) main.c gpio.o common_dht_read.o pi_dht_read.o -o gpio.out  $(INCLUDES) $(CLIBS)

gpio.o: gpio.c gpio.h
	$(GCC) $(CFLAGS) gpio.c -c  $(INCLUDES) $(CLIBS)
	
common_dht_read.o: common_dht_read.c common_dht_read.h
	$(GCC) $(CFLAGS) common_dht_read.c -c  $(INCLUDES) $(CLIBS)

pi_dht_read.o: pi_dht_read.c pi_dht_read.h
	$(GCC) $(CFLAGS) pi_dht_read.c -c  $(INCLUDES) $(CLIBS)
	
clean:
	rm -f *.out
	rm -f *.o