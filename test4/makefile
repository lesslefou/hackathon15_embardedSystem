GCC= gcc
CFLAGS = -Wall -Werror 
CLIBS = 
INCLUDES = -lm

default:MAIN
	./main

MAIN: main.c log_personnal_api.o onem2m_personnal_api.o
	$(GCC) $(CFLAGS) $^ -o main  $(INCLUDES) $(CLIBS)

%.o: %.c %.h
	$(GCC) $(CFLAGS) $< -c $(INCLUDES) $(CLIBS)

clean:
	rm -f main
	rm -f *.o
	rm -f *.sav
