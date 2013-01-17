CXX=clang++
CPPFLAGS=-I.
CXXFLAGS=-g -Wall -std=c++11

csim: csim.o
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@

csim.o: csim.cc cache.h

clean:
	rm -f csim *.o

.PHONY: clean
