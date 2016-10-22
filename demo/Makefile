all: iirdemo

iirdemo: iirdemo.o
	g++ -L../.libs -o iirdemo iirdemo.o -liir

iirdemo.o: iirdemo.cpp
	g++ -c iirdemo.cpp

clean:
	rm -f *.dat *~ *.o iirdemo
