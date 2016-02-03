Assign01: IntSet.o Assign01.o
	g++ IntSet.o Assign01.o -o Assign01
IntSet.o: IntSet.cpp IntSet.h
	g++ -Wall -ansi -pedantic -c IntSet.cpp
Assign01.o: Assign01.cpp IntSet.h
	g++ -Wall -ansi -pedantic -c Assign01.cpp

cleanall:
	@rm Assign01 *.o
test:
	./Assign01 auto < Assign01test.in > Assign01test.out
