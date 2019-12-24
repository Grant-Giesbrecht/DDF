#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include "../C++/cppkv.hpp"

using namespace std;

int main(){

	double x = 5;

	vector<double> Vout;
	Vout.push_back(2); Vout.push_back(9); Vout.push_back(12); Vout.push_back(20);

	vector<double> Vin;
	Vin.push_back(3); Vin.push_back(5); Vin.push_back(7); Vin.push_back(19);

	vector<vector<double> > V2d;
	V2d.push_back(Vout); V2d.push_back(Vin);
	KVFile kv;
	kv.setHeader("This is a test file.\n\tI was written by a test program :)");
	kv.add(x, "X", "I am a description");
	kv.add(Vout, "Vout", "Output");
	kv.add(Vin, "Vin", "eingang");
	kv.add(V2d, "Vec_2D", "2D Matrix :)");
	kv.add(V2d, "Vec_2D2", "I'm a copycat :P");

	cout << "\nNo. Variables: " << to_string(kv.numVar()) << endl;
	cout << "Variables:\n\t" << kv.names("mt")[0] << endl;

	kv.write("../examples/cppkv_out.kv", ";ov");

	kv.clear();

	cout << "\nCleared...\n" << endl;

	cout << "\nNo. Variables: " << to_string(kv.numVar()) << endl;
	cout << "Variables:\n\t" << kv.names("mt")[0] << endl;

	if (!kv.read("../C++/read_test.kv")){
		cout << "Failed to read file." << endl;
		cout << kv.err() << endl;
	}else{
		cout << "Read file..." << endl;

		cout << "\nVersion: " << kv.getVersion() << endl;
		cout << "Header: " << kv.getHeader() << endl;

		cout << "\nNo. Variables: " << to_string(kv.numVar()) << endl;
		cout << "Variables:\n\t" << kv.names("mt")[0] << endl;
	}




	return 0;
}
