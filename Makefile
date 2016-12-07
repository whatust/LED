all: build
	
build:
	g++ -Wall -std=c++11 -o util.o -c util.cpp
	g++ -Wall -std=c++11 -o led.o -c led.cpp
	g++ -Wall -std=c++11 main.cpp util.o led.o -o led

test:
	echo "e 0 0\n" | ./led
	echo "e 0123456789abcdef 0123456789abcdef\n" | ./led

test_comp: build
	echo "t\n" | ./led

test_key: build
	echo "e 0 1\n" | ./led

valgrind: build
	valgrind --leak-check=yes ./led < test.txt

clean:
	rm -rf util.o led.o led