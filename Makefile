libkvp.a : kvparse.h kvparse_except.h kvparse.o 
	ar rcs libkvp.a kvparse.o

kvparse.o : kvparse.h kvparse_except.h kvparse.cpp
	g++ -Wall -O3 -c -o kvparse.o kvparse.cpp -lboost_regex

run_tests : kvparse.h kvparse.cpp test_kvparse.cpp
	g++ -Wall -O2 -o run_tests kvparse.cpp test_kvparse.cpp -lgtest -lgtest_main -lpthread -lboost_regex

.PHONY : clean
clean :
	rm -f *.o

.PHONY : distclean
distclean :
	make clean
	rm -f libkvp.a run_tests
