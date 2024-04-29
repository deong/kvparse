CXX=clang++
CXXFLAGS=-Wall -Werror -std=c++23 -O2 -pipe 

libkvparse.so.1.0.0 : kvparse.h kvparse.cpp
	${CXX} ${CXXFLAGS} -c -fpic kvparse.cpp
	${CXX} -shared -o libkvparse.so.1.0.0 kvparse.o

run_tests : kvparse.h kvparse.cpp test_kvparse.cpp
	${CXX} ${CXXFLAGS} -o run_tests kvparse.cpp test_kvparse.cpp -lgtest -lgtest_main -lpthread -lboost_regex

install : libkvparse.so.1.0.0
	cp kvparse.h /usr/local/include
	cp libkvparse.so.1.0.0 /usr/local/lib
	ln -s /usr/local/lib/libkvparse.so.1.0.0 /usr/local/lib/libkvparse.so.1.0
	ln -s /usr/local/lib/libkvparse.so.1.0.0 /usr/local/lib/libkvparse.so.1
	ln -s /usr/local/lib/libkvparse.so.1.0.0 /usr/local/lib/libkvparse.so

.PHONY : clean
clean :
	rm -f *.o libkvparse.so.1.0.0

.PHONY : distclean
distclean :
	make clean
	rm -f run_tests

.PHONY : uninstall
uninstall :
	rm -f /usr/local/lib/libkvparse.so*
	rm -f /usr/local/include/kvparse.h
