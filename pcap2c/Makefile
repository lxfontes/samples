CC = g++
CFLAGS =-Wall -O2 -g
LDFLAGS =-rdynamic -lpcap -ldl
PROG=pcap2c
PCAP2C_OBJ=pcap2c.o

COMPILE = $(CC) $(CFLAGS) -c
COMPILE_SO = $(CC) $(CFLAGS) -fPIC -shared
all: $(PROG)


pcap2c: $(PCAP2C_OBJ)
	$(CC) -o pcap2c $(PCAP2C_OBJ) $(LDFLAGS)

%.o: %.cc
	$(COMPILE) -o $@ $<

%.so: %.cc
	$(COMPILE_SO) -o $@ $<

clean:
	/bin/rm -f $(PROG) $(PCAP2C_OBJ)
