MAKE=gmake
CC=g++
LDFLAGS= -L../lib
CFLAGS= -Wall -O3 -I../include
LIBS= -lopenalpp

all:	teststream testsend testmic testlib
	@echo "Use the following to run: "
	@echo "LD_LIBRARY_PATH=../lib/:$LD_LIBRARY_PATH ./testlib"

%.o:	%.cpp %.h ../include/alpp.h
	$(CC) $(CFLAGS) -c $< -o $@

testlib: test.o Makefile
	$(CC) $(LDFLAGS) $(LIBS) -o testlib test.o
teststream: teststream.o Makefile
	$(CC) $(LDFLAGS) $(LIBS) -o teststream teststream.o
testsend: testsend.o Makefile
	$(CC) $(LDFLAGS) $(LIBS) -o testsend testsend.o
testmic: testmic.o Makefile
	$(CC) $(LDFLAGS) $(LIBS) -o testmic testmic.o

clean:
	rm -f *.o core core.* *~
