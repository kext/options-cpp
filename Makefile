CFLAGS = -std=c++11 -Wall -pedantic -O3

example: example.cc liboptions.a options.h Makefile
	$(CXX) -o example example.cc liboptions.a $(CFLAGS)

liboptions.a: options.cc options.h Makefile
	$(CXX) -c options.cc $(CFLAGS)
	ar rcs liboptions.a options.o
	@rm options.o

clean:
	rm -rf example liboptions.a
