#ifndef CPPKV_HPP
#define CPPKV_HPP

#include <string>
#include <vector>

//***************************************************************************//
//**			TYPE DEFINITIONS										   **//
//***************************************************************************//

/*
Describes a single data point in a KVar.
*/
typedef struct{
	double d;
	std::string s;
	bool b;
	char type;
}KVDatum;


/*
Contains every available type of data. Makes operator() able to return
any variable and a '.<type>' after the operator() call allows quick
initialization of values.

ex:
vector<double> Vin = kvf("Vin").md;
*/
typedef struct{
	//Non-matrix variables
	double d;
	std::string s;
	bool b;

	//1D Matrices
	std::vector<double> md;
	std::vector<std::string> ms;
	std::vector<bool> mb;

	//2D Matrices
	std::vector<std::vector<double> > md2;
	std::vector<std::vector<std::string> > ms2;
	std::vector<std::vector<bool> > mb2;
}KVItem;

/*
The KVFlatItem, ..1DITem, and ..2DItem are used to make a simple database of
variables inside the KVFile object. Three vectors, once of each type of struct
contain all of the object's variables. One vector of KVItems could contain them
all, but would waste more memory as 1/9 variables in ea. struct would be used
instead of 1/3. This especially helps save memory when few matrices or 2D
matrices are used.
*/

typedef struct{
	double d;
	std::string s;
	bool b;

	std::string name;
	char type;
}KVFlatItem;

typedef struct{
	std::vector<double> md;
	std::vector<std::string> ms;
	std::vector<bool> mb;

	std::string name;
	char type;
}KV1DItem;

typedef struct{
	std::vector<std::vector<double> > md2;
	std::vector<std::vector<std::string> > ms2;
	std::vector<std::vector<bool> > mb2;

	std::string name;
	char type;
}KV2DItem;

//***************************************************************************//
//**			CLASS DEFINITIONS									   **//
//***************************************************************************//

class KVFile{
public:

	//************ INITIALIZERS

	KVFile();
	KVFile(std::string fileIn);

	//************ ADD VARIABLES

	//Non-matrix variables
	void add(double newVar, std::string varName);
	void add(std::string newVar, std::string varName);
	void add(bool newVar, std::string varName);

	//Add variables
	//
	//1D Matrices
	void add(std::vector<double> newVar, std::string varName);
	void add(std::vector<std::string> newVar, std::string varName);
	void add(std::vector<bool> newVar, std::string varName);

	//Add variables
	//
	//2D Matrices
	void add(std::vector<std::vector<double> > newVar, std::string varName);
	void add(std::vector<std::vector<std::string> > newVar, std::string varName);
	void add(std::vector<std::vector<bool> > newVar, std::string varName);

	//*********** READ VARIABLES

	KVItem operator()(std::string varName);


	//********** FILE I/O

	bool write(std::string fileOut, std::string options);
	bool read(std::string fileIn);
	bool clread(std::string fileIn);


	//*************** VARIABLE MANAGEMENT

	bool checkContains(std::vector<std::string> names);
	void clear();
	size_t numVar();

	//************** PRINTING

	std::vector<std::string> names(std::string options="");


private:

	std::vector<KVFlatItem> variablesFlat;
	std::vector<KV1DItem> variables1D;
	std::vector<KV2DItem> variables2D;

	void initialize();

};

//***************************************************************************//
//**			FUNCTION DEFINITIONS									   **//
//***************************************************************************//


//***************************************************************************//
//***************** INITIALIZERS

KVFile::KVFile(){
	initialize();
}

/*
Reads the input file. Same as calling blank initializer combined with read()
*/
KVFile::KVFile(std::string fileIn){

	initialize();

	read(fileIn);
}

/*
Initialize the object
*/
void KVFile::initialize(){
	//Nothing to do here at the moment...
}

//***************************************************************************//
//*************** ADD VARIABLES

//****** Non-matrix variables **********

void KVFile::add(double newVar, std::string varName){

	KVFlatItem newItem; 	//Create new item
	newItem.d = newVar;		//Give new item correct value
	newItem.name = varName; //Give new item
	newItem.type = 'd';		//Specify type

	variablesFlat.push_back(newItem); //Add to vector of varaiables

}

void KVFile::add(std::string newVar, std::string varName){

	KVFlatItem newItem; 	//Create new item
	newItem.s = newVar;		//Give new item correct value
	newItem.name = varName; //Give new item
	newItem.type = 's';		//Specify type

	variablesFlat.push_back(newItem); //Add to vector of varaiables

}

void KVFile::add(bool newVar, std::string varName){

	KVFlatItem newItem; 	//Create new item
	newItem.b = newVar;		//Give new item correct value
	newItem.name = varName; //Give new item
	newItem.type = 'b';		//Specify type

	variablesFlat.push_back(newItem); //Add to vector of varaiables

}

//******  1D matrix variables **********

void KVFile::add(std::vector<double> newVar, std::string varName){

	KV1DItem newItem; 		//Create new item
	newItem.md = newVar;	//Give new item correct value
	newItem.name = varName; //Give new item
	newItem.type = 'd';		//Specify type

	variables1D.push_back(newItem); //Add to vector of varaiables

}

void KVFile::add(std::vector<std::string> newVar, std::string varName){

	KV1DItem newItem; 		//Create new item
	newItem.ms = newVar;	//Give new item correct value
	newItem.name = varName; //Give new item
	newItem.type = 's';		//Specify type

	variables1D.push_back(newItem); //Add to vector of varaiables

}

void KVFile::add(std::vector<bool> newVar, std::string varName){

	KV1DItem newItem; 		//Create new item
	newItem.mb = newVar;	//Give new item correct value
	newItem.name = varName; //Give new item
	newItem.type = 'b';		//Specify type

	variables1D.push_back(newItem); //Add to vector of varaiables

}

//******  2D matrix variables **********

void KVFile::add(std::vector<std::vector<double> > newVar, std::string varName){

	KV2DItem newItem; 		//Create new item
	newItem.md2 = newVar;	//Give new item correct value
	newItem.name = varName; //Give new item
	newItem.type = 'd';		//Specify type

	variables2D.push_back(newItem); //Add to vector of varaiables

}

void KVFile::add(std::vector<std::vector<std::string> > newVar, std::string varName){

	KV2DItem newItem; 		//Create new item
	newItem.ms2 = newVar;	//Give new item correct value
	newItem.name = varName; //Give new item
	newItem.type = 's';		//Specify type

	variables2D.push_back(newItem); //Add to vector of varaiables

}

void KVFile::add(std::vector<std::vector<bool> > newVar, std::string varName){

	KV2DItem newItem; 		//Create new item
	newItem.mb2 = newVar;	//Give new item correct value
	newItem.name = varName; //Give new item
	newItem.type = 'b';		//Specify type

	variables2D.push_back(newItem); //Add to vector of varaiables

}

bool KVFile::read(std::string fileIn){
	return false;
}

//*************** VARIABLE MANAGEMENT


size_t KVFile::numVar(){
	return variablesFlat.size() + variables1D.size() + variables2D.size();
}

//***************************************************************************//
//*************** PRINTING

/*
Returns a vector of strings of the variables' names.

Options:
	- t: include variable type with name
	- m: merge all names into one string, save in returned vector's index 0.
*/
std::vector<std::string> KVFile::names(std::string options){

	//Options
	bool include_type = false;
	bool merge = false;

	//Read in options
	if (options.find("t", 0) != std::string::npos){
		include_type = true;
	}
	if (options.find("m", 0) != std::string::npos){
		merge = true;
	}

	std::vector<std::string> names_out;

	//Add flat names
	for (size_t i = 0 ; i < variablesFlat.size() ; i++){
		if (include_type){
			names_out.push_back(variablesFlat[i].name + " (" + variablesFlat[i].type + ")");
		}else{
			names_out.push_back(variablesFlat[i].name);
		}
	}

	//Add 1D names
	for (size_t i = 0 ; i < variables1D.size() ; i++){
		if (include_type){
			names_out.push_back(variables1D[i].name + " (m<" + variables1D[i].type + ">)");
		}else{
			names_out.push_back(variables1D[i].name);
		}
	}

	//Add 2D names
	for (size_t i = 0 ; i < variables2D.size() ; i++){
		if (include_type){
			names_out.push_back(variables2D[i].name + " (m<" + variables2D[i].type + ">(2D))");
		}else{
			names_out.push_back(variables2D[i].name);
		}
	}

	//Merge (if requested)
	if (merge){

		std::string merged_str = ""; //Create blank string

		//For each name...
		for (size_t i = 0 ; i+1 < names_out.size() ; i++){
			merged_str = merged_str + names_out[i] + ", "; //Add to merged name...
		}

		//Add last element without comma
		if (names_out.size() > 0){
			merged_str = merged_str + names_out[names_out.size()-1];
		}

		names_out.clear(); //Clear output vector
		names_out.push_back(merged_str); //push back merged string
	}

	//Return names vector
	return names_out;

}

#endif
