CC = clang++ -std=c++11
OBJS = cppkv.o

test: ../examples/cppkv_example.cpp KTable.o
	$(CC) -o ../examples/cppkv_example ../examples/cppkv_example.cpp KTable.o
	../examples/cppkv_example

KTable.o: KTable.cpp
	$(CC) -c KTable.cpp
