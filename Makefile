CC=g++
CFLAGS=-std=c++11 -g

all: TarArchive.o TarContents.o
	$(CC) $(CFLAGS) TarArchive.o TarContents.o
	$(CC) -shared -Wl,-soname=libfcf.so.1 -o libtarpp.so.1.0.0
	ar rcs libtarpp.a TarArchive.o TarContents.o
TarArchive.o:TarArchive.cpp TarArchive.hpp
	$(CC) $(CFLAGS) -c TarArchive.cpp
TarContents.o:TarContents.cpp TarContents.hpp
	$(CC) $(CFLAGS) -c TarContents.cpp

clean:
	rm -rf *.o *.a libtarpp.so* libtarpp.a a.out
