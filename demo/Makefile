all: iirdemo

iirdemo: iirdemo.o
	gcc -o iirdemo iirdemo.o -liir

iirdemo.o: iirdemo.cpp
	gcc -c iirdemo.cpp

clean:
	rm -f *.dat *~ *.o iirdemo
