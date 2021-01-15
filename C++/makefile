OS := $(shell uname)
ifeq ($(OS),Darwin)
	CC = clang++ -std=c++11
else
	CC = g++ -std=c++11
endif

INCLUDES = -I/Users/grantgiesbrecht/Documents/GitHub
LIBS = -L/Users/grantgiesbrecht/Documents/GitHub/gstd -lgstd -L/Users/grantgiesbrecht/Documents/GitHub/KTable -lktable
OBJS = cppkv.o

simpletest: ../examples/cppkv_example.cpp
	$(CC) -o ../examples/cppkv_example ../examples/cppkv_example.cpp $(INCLUDES) $(LIBS)
	../examples/cppkv_example

testall: ../examples/cppkv_read_all_tests.cpp
	$(CC) -o ../examples/cppkv_read_all_tests ../examples/cppkv_read_all_tests.cpp $(INCLUDES) $(LIBS)
	../examples/cppkv_read_all_tests
