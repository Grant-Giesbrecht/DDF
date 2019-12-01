#ifndef CPPKV_HPP
#define CPPKV_HPP

#include <cctype>
#include <cstdio>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#define CURRENT_VERSION "2.0"

std::string to_gstring(double x, size_t buf_size=30, size_t precision=6);
std::string bool_to_string(bool b);

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
	std::string description;
}KVFlatItem;

typedef struct{
	std::vector<double> md;
	std::vector<std::string> ms;
	std::vector<bool> mb;

	std::string name;
	char type;
	std::string description;
}KV1DItem;

typedef struct{
	std::vector<std::vector<double> > md2;
	std::vector<std::vector<std::string> > ms2;
	std::vector<std::vector<bool> > mb2;

	std::string name;
	char type;
	std::string description;
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
	void add(double newVar, std::string varName, std::string desc="");
	void add(std::string newVar, std::string varName, std::string desc="");
	void add(bool newVar, std::string varName, std::string desc="");

	//1D Matrices
	void add(std::vector<double> newVar, std::string varName, std::string desc="");
	void add(std::vector<std::string> newVar, std::string varName, std::string desc="");
	void add(std::vector<bool> newVar, std::string varName, std::string desc="");

	//2D Matrices
	void add(std::vector<std::vector<double> > newVar, std::string varName, std::string desc="");
	void add(std::vector<std::vector<std::string> > newVar, std::string varName, std::string desc="");
	void add(std::vector<std::vector<bool> > newVar, std::string varName, std::string desc="");

	//TODO: Add way to edit variables

	//*********** READ VARIABLES

	KVItem operator()(std::string varName);


	//********** FILE I/O

	std::string swrite(std::string fileOut, std::string options="");
	bool write(std::string fileOut, std::string options="");
	bool read(std::string fileIn);
	bool clread(std::string fileIn);


	//*************** VARIABLE MANAGEMENT

	bool checkContains(std::vector<std::string> names);
	void clear();
	size_t numVar();

	//*************** HEADER

	void setHeader(std::string h);
	std::string getHeader();

	//************** PRINTING

	std::vector<std::string> names(std::string options="");


private:

	std::vector<KVFlatItem> variablesFlat;
	std::vector<KV1DItem> variables1D;
	std::vector<KV2DItem> variables2D;

	void initialize();
	bool isValidName(std::string name);
	bool nameInUse(std::string name);

	std::string header;

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

/*
Adds a variable to the KVFile object. Retuns without adding variable if name is
invalid or already in use.
*/


void KVFile::add(double newVar, std::string varName, std::string desc){

	//Check that name is valid and unclaimed
	if ( (!isValidName(varName)) || nameInUse(varName)) return;

	KVFlatItem newItem; 	//Create new item
	newItem.d = newVar;		//Give new item correct value
	newItem.name = varName; //Give new item
	newItem.type = 'd';		//Specify type
	newItem.description = desc; //Add description

	variablesFlat.push_back(newItem); //Add to vector of varaiables

}

void KVFile::add(std::string newVar, std::string varName, std::string desc){

	//Check that name is valid and unclaimed
	if ( (!isValidName(varName)) || nameInUse(varName)) return;

	KVFlatItem newItem; 	//Create new item
	newItem.s = newVar;		//Give new item correct value
	newItem.name = varName; //Give new item
	newItem.type = 's';		//Specify type
	newItem.description = desc; //Add description

	variablesFlat.push_back(newItem); //Add to vector of varaiables

}

void KVFile::add(bool newVar, std::string varName, std::string desc){

	//Check that name is valid and unclaimed
	if ( (!isValidName(varName)) || nameInUse(varName)) return;

	KVFlatItem newItem; 	//Create new item
	newItem.b = newVar;		//Give new item correct value
	newItem.name = varName; //Give new item
	newItem.type = 'b';		//Specify type
	newItem.description = desc; //Add description

	variablesFlat.push_back(newItem); //Add to vector of varaiables

}

//******  1D matrix variables **********

void KVFile::add(std::vector<double> newVar, std::string varName, std::string desc){

	//Check that name is valid and unclaimed
	if ( (!isValidName(varName)) || nameInUse(varName)) return;

	KV1DItem newItem; 		//Create new item
	newItem.md = newVar;	//Give new item correct value
	newItem.name = varName; //Give new item
	newItem.type = 'd';		//Specify type
	newItem.description = desc; //Add description

	variables1D.push_back(newItem); //Add to vector of varaiables

}

void KVFile::add(std::vector<std::string> newVar, std::string varName, std::string desc){

	//Check that name is valid and unclaimed
	if ( (!isValidName(varName)) || nameInUse(varName)) return;

	KV1DItem newItem; 		//Create new item
	newItem.ms = newVar;	//Give new item correct value
	newItem.name = varName; //Give new item
	newItem.type = 's';		//Specify type
	newItem.description = desc; //Add description

	variables1D.push_back(newItem); //Add to vector of varaiables

}

void KVFile::add(std::vector<bool> newVar, std::string varName, std::string desc){

	//Check that name is valid and unclaimed
	if ( (!isValidName(varName)) || nameInUse(varName)) return;

	KV1DItem newItem; 		//Create new item
	newItem.mb = newVar;	//Give new item correct value
	newItem.name = varName; //Give new item
	newItem.type = 'b';		//Specify type
	newItem.description = desc; //Add description

	variables1D.push_back(newItem); //Add to vector of varaiables

}

//******  2D matrix variables **********

void KVFile::add(std::vector<std::vector<double> > newVar, std::string varName, std::string desc){

	//Check that name is valid and unclaimed
	if ( (!isValidName(varName)) || nameInUse(varName)) return;

	KV2DItem newItem; 		//Create new item
	newItem.md2 = newVar;	//Give new item correct value
	newItem.name = varName; //Give new item
	newItem.type = 'd';		//Specify type
	newItem.description = desc; //Add description

	variables2D.push_back(newItem); //Add to vector of varaiables

}

void KVFile::add(std::vector<std::vector<std::string> > newVar, std::string varName, std::string desc){

	//Check that name is valid and unclaimed
	if ( (!isValidName(varName)) || nameInUse(varName)) return;

	KV2DItem newItem; 		//Create new item
	newItem.ms2 = newVar;	//Give new item correct value
	newItem.name = varName; //Give new item
	newItem.type = 's';		//Specify type
	newItem.description = desc; //Add description

	variables2D.push_back(newItem); //Add to vector of varaiables

}

void KVFile::add(std::vector<std::vector<bool> > newVar, std::string varName, std::string desc){

	//Check that name is valid and unclaimed
	if ( (!isValidName(varName)) || nameInUse(varName)) return;

	KV2DItem newItem; 		//Create new item
	newItem.mb2 = newVar;	//Give new item correct value
	newItem.name = varName; //Give new item
	newItem.type = 'b';		//Specify type
	newItem.description = desc; //Add description

	variables2D.push_back(newItem); //Add to vector of varaiables

}

//***************************************************************************//
//*************** FILE I/O

/*
Writes the currently loaded variables to a KV file on disk.

Options:
	v: Save all matrices as vertical
	o: Optimize size by removing lines intended to improve human-readability.
	d: Decapitate - ie. omit header
	-: Do not print version statement. WARNING: This will make the file unreadable.
		Don't use it unless you know what you're doing.
	;: Terminate variable statements with the optional semicolon

In the event of an error, it returns a blank string.
*/
std::string KVFile::swrite(std::string fileOut, std::string options){ //TODO: Impliment write()

	std::string kvf; //This is the string which will contain the file

	bool vertical_mode = false;
	bool optimize = false;
	bool decapitate = false;
	bool skip_version = false;
	std::string term_char = "";

	//Read in options
	if (options.find("v", 0) != std::string::npos){
		vertical_mode = true;
	}
	if (options.find("o", 0) != std::string::npos){
		optimize = true;
	}
	if (options.find("d", 0) != std::string::npos){
		decapitate = true;
	}
	if (options.find("-", 0) != std::string::npos){
		skip_version = true;
	}
	if (options.find(";", 0) != std::string::npos){
		term_char = ";";
	}

	//********************** Write version statement *************************//
	kvf = kvf + "#VERSION " + std::string(CURRENT_VERSION) + "\n";
	if (!optimize) kvf = kvf + "\n";

	//*********************** Write header statement *************************//
	if (header.length() > 0 && !decapitate){
		kvf = kvf + "#HEADER\n" + header + "\n#HEADER\n";
		if (!optimize) kvf = kvf + "\n";
	}


	//********************** Write Flat Variables ****************************//

	//For each variable...
	for (size_t i = 0 ; i < variablesFlat.size() ; i++){

		//Write type
		switch(variablesFlat[i].type){
			case('d'):
				kvf = kvf + "d " + variablesFlat[i].name + " " + to_gstring(variablesFlat[i].d) + term_char + "\n";
				break;
			case('b'):
				kvf = kvf + "b " + variablesFlat[i].name + " " + bool_to_string(variablesFlat[i].b) + term_char + "\n";
				break;
			case('s'):
				kvf = kvf + "m " + variablesFlat[i].name + " " + variablesFlat[i].s + term_char + "\n";
				break;
			default:
				return ""; //An error occured
		}

	}

	return kvf;
}

bool KVFile::write(std::string fileOut, std::string options){

	//Generate string
	std::string fileString = swrite(fileOut, options);

	//Write to file
	std::ofstream out(fileOut);
	if (!out.is_open()){
		return false;
	}
    out << fileString;
    out.close();

	return false;
}

bool KVFile::read(std::string fileIn){ //TODO: Impliment read()
	std::cout << "Ooops! Not implimented" << std::endl;
	return false;
}

bool KVFile::clread(std::string fileIn){
	clear();
	return read(fileIn);
}



//***************************************************************************//
//*************** VARIABLE MANAGEMENT

bool KVFile::checkContains(std::vector<std::string> names){ //TODO: impliment checkContains()
	std::cout << "Oops! Need to impliment this :)" << std::endl;
	return false;
}

/*
Clears all variables from the KVFile object.
*/
void KVFile::clear(){
	variablesFlat.clear();
	variables1D.clear();
	variables2D.clear();
}

/*
Returns the number of variables of all types combined that are presently loaded
in this KVFile object.
*/
size_t KVFile::numVar(){
	return variablesFlat.size() + variables1D.size() + variables2D.size();
}

//***************************************************************************//
//*************** HEADER

/*
Sets the header
*/
void KVFile::setHeader(std::string h){
	header = h;
}

/*
Returns the header
*/
std::string KVFile::getHeader(){
	return header;
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

/*
Verifies that the proposed variable name is a valid variable name.

Returns true if the name is valid.
*/
bool KVFile::isValidName(std::string name){

	//Ensure name is at least 1 character
	if (name.length() < 1){
		return false;
	}

	//Make sure name starts with a letter
	if (!isalpha(name[0])){
		return false;
	}

	//For each character...
	for (size_t i = 0 ; i < name.length() ; i++){

		//Check if it is whitespace...
		if (isspace(name[i]) == 1){
			return false;
		}
	}

	return true;
}

/*
Checks if the variable name is already in use. Returns true if in use.
*/
bool KVFile::nameInUse(std::string name){

	//Check flat variables...
	for (size_t i = 0 ; i < variablesFlat.size() ; i++){
		if (variablesFlat[i].name == name) return true;
	}

	//Check 1D variables...
	for (size_t i = 0 ; i < variables1D.size() ; i++){
		if (variables1D[i].name == name) return true;
	}

	//Check 2D variables...
	for (size_t i = 0 ; i < variables2D.size() ; i++){
		if (variables2D[i].name == name) return true;
	}

	return false;
}

/*
Converts a double to a string using 'g' formatting.

precision is the number after the decimal in fprint strings. It
specifies the max number of digits after the decimal. Recommended value = 6.

buf_size is the size of the char buffer to use. Defaults to 30.

Returns a string of the variable 'x'.
*/
std::string to_gstring(double x, size_t buf_size, size_t precision){

	//Set upper limits on size
	if (precision > 100){
		precision = 100;
	}
	if (buf_size > 300){
		buf_size = 300;
	}

	if (buf_size < 10){
		buf_size = 10;
	}

	char buffer[buf_size];
	std::string format = "%." + std::to_string(precision) + "G";

	sprintf(buffer, format.c_str(), x);

	return std::string(buffer);
}

std::string bool_to_string(bool b){
	return b? "true" : "false";
}

#endif
