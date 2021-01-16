#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include "../C++/cppkv.hpp"

using namespace std;

int main(){

	vector<string> test_files;
	test_files.push_back("../examples/read_test.kv");
	test_files.push_back("../examples/read_test2.kv");
	test_files.push_back("../examples/read_test3.kv");

	KVFile kv;

	for (size_t i = 0 ; i < test_files.size() ; i++){

		cout << " ****************** Testing " << test_files[i] << " **************** " << endl;

		if (!kv.read(test_files[i])){
			cout << "Failed to read file '" << test_files[i] << "'." << endl;
			cout << kv.err() << endl;
		}else{
			cout << kv.show() << endl;
			kv.clear();
		}
	}

	return 0;
}
