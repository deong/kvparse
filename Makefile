run_tests : parsimony.h parsimony.cpp test_parsimony.cpp
	g++ -Wall -O2 -std=c++11 -o run_tests parsimony.cpp test_parsimony.cpp -lgtest -lgtest_main -lpthread

.PHONY : clean
clean :
	rm -f *.o

.PHONY : distclean
distclean :
	make clean
	rm -f run_tests
