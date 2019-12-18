CC = clang++ -std=c++11
INCLUDES = -I/Users/grantgiesbrecht/Documents/GitHub
LIBS = -L/Users/grantgiesbrecht/Documents/GitHub/gstd -lgstd -L/Users/grantgiesbrecht/Documents/GitHub/KTable -lktable
OBJS = cppkv.o

test: ../examples/cppkv_example.cpp
	$(CC) -o ../examples/cppkv_example ../examples/cppkv_example.cpp $(INCLUDES) $(LIBS)
	../examples/cppkv_example
