#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include "../C++/cppkv.hpp"

#include "../C++/KTable.hpp"

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
	kv.add(Vout, "Vout", "Output");
	kv.add(Vin, "Vin", "eingang");
	kv.add(V2d, "Vec_2D", "2D Matrix :)");
	kv.add(V2d, "Vec_2D2", "I'm a copycat :P");

	cout << "\nNo. Variables: " << to_string(kv.numVar()) << endl;
	cout << "Variables:\n\t" << kv.names("mt")[0] << endl;

	kv.write("../examples/cppkv_out.kv", ";ov");

	KTable kt;
	// kt.table_title("");
	// kt.row({"m<d>", "m<s>", "m<b>"});
	// kt.row({"test", "test2", "tests"});
	// kt.row({"?what", "?where", "?when"});
	// kt.row({"19", "\"yasss\"", "TRUE"});
	// kt.row({"20", "\"mmmm\"", "TRUE"});
	// kt.set(KTABLE_INTERWALLS, false);
	// kt.set(KTABLE_SIDEWALLS, false);
	// kt.set(KTABLE_TOPBOTTOMHBAR, false);
	// kt.set(KTABLE_HEADERINTERWALLS, false);
	// kt.set(KTABLE_HEADERHBAR, false);
	// kt.set(KTABLE_TITLEHBAR, false);
	// kt.alignh('l');
	// kt.alignc('l');
	// kt.alignt('l');

	// cout << "\n\n" << kt.str() << endl;


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
