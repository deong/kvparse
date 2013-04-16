run_tests : kvparse.h kvparse.cpp test_kvparse.cpp
	g++ -Wall -O2 -o run_tests kvparse.cpp test_kvparse.cpp -lgtest -lgtest_main -lpthread -lboost_regex

.PHONY : clean
clean :
	rm -f *.o

.PHONY : distclean
distclean :
	make clean
	rm -f run_tests
