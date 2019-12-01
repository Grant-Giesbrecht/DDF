	#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include "../C++/cppkv.hpp"

using namespace std;

int main(){

	string fn = "./experiment_data.kv";

	double x = 5;

	vector<double> Vout;
	Vout.push_back(2); Vout.push_back(9); Vout.push_back(12); Vout.push_back(20);

	vector<double> Vin;
	Vin.push_back(3); Vin.push_back(5); Vin.push_back(7); Vin.push_back(19);

	vector<vector<double> > V2d;
	V2d.push_back(Vout); V2d.push_back(Vin);

	KVFile kv;
	kv.setHeader("This is a test file.\n\tI was written by a test program :)");
	kv.add(x, "X");
	kv.add(Vout, "Vout");
	kv.add(Vin, "Vin");
	kv.add(V2d, "Vec_2D");

	cout << "\nNo. Variables: " << to_string(kv.numVar()) << endl;
	cout << "Variables:\n\t" << kv.names("mt")[0] << endl;

	kv.write("../examples/cppkv_out.kv", ";o");

	//
	//
	// kv.clread()
	// if (!kv.checkContains(["Vin", "freq", "Vout"])){
	// 	cout << "ERROR: " << fn << " does not contain required variables. Exiting." << endl;
	// 	return -1;
	// }
	// vector<double> Vin = kv("Vin").md;
	// vector<double> Vout = kv("Vout").md;

	return 0;
}
