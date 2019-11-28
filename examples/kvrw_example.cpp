#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <KVrw>

using namespace std;

int main(){
	
	string fn = "./experiment_data.kv";
	
	double x = 5;
	
	vector<double> Vout = ;lkashdf;lksjdaf;lashf;
	vector<double> Vin = ;lkashdf;lksjdaf;lashf;
	
	kv = KVrw();
	kv.add(x, "X");
	kv.add(Vout, "Vout");
	kv.add(Vin, "Vin");
	kv.write("./calc_out.kv")
	
	//...
	
	//...
	
	kv.clread()
	if (!kv.checkContains("Vin", "freq", "Vout")){
		cout << "ERROR: " << fn << " does not contain required variables. Exiting." << endl;
		return -1;
	}
	vector<double> Vin = kv("Vin")
	vector<double> Vout = kv("Vout")
	
	return 0;
}
