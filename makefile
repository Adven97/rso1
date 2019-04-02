CFLAGS=-ggdb -Wall -pedantic -D_REENTRANT

%: %.c
	gcc $(CFLAGS) $< -o $@ -lpthread -lm

EXECS = netclient server1 
		

all: $(EXECS)

netclient: netclient.c
server1: server1.c


.PHONY: clean all

clean:
	rm -f $(EXECS)
