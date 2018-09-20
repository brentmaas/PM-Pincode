all:
	@g++ -o Assignment2 src/maas2.cc -std=c++17 -Wall -Wextra

clean:
	@rm Assignment2.exe

run:
	@./Assignment2.exe