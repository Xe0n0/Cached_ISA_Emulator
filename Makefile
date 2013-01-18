CC=clang
CXX=clang++
CPPFLAGS=-I.
CFLAGS=-g -Wall -std=c11
CXXFLAGS=-g -Wall -std=c++11 -stdlib=libc++

all: csim validate

csim: csim.o cache.o cache_replace.o
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@

csim.o: csim.cc cache.h

cache.o: cache.c cache.h cache_replace.h

cache_replace.o: cache_replace.c cache_replace.h cache.h

validate: validate.cc

clean:
	rm -f csim validate *.o

.PHONY: clean
