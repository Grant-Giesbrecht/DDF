#ifndef CPPKV_HPP
#define CPPKV_HPP

#include <cctype>
#include <cstdio>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "KTable.hpp" //TODO: Change this to be separate library

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
	std::string description; //TODO: Add code to verify desc doesn't contain newline when added by user
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

	void sortMatrices();
	size_t matrixLength(KV1DItem m);
	size_t matrixLength(KV2DItem m);

	double linaccess(std::vector<std::vector<double> > m, size_t idx);
	bool linaccess(std::vector<std::vector<bool> > m, size_t idx);
	std::string linaccess(std::vector<std::vector<std::string> > m, size_t idx);

	bool isRowEnd(KV2DItem m, size_t idx);

	void init_ktable(KTable& kt);

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

Options: (Order does not matter. Case-sensitive)
	v: Save all matrices as vertical
	o: Optimize size by removing lines intended to improve human-readability.
	d: Decapitate - ie. omit header
	-: Do not print version statement. WARNING: This will make the file unreadable.
		Don't use it unless you know what you're doing.
	;: Terminate variable statements with the optional semicolon
	s: Sort vectors largest to smallest
	u: Undocumented - variable descriptions are not printed

In the event of an error, it returns a blank string.
*/
std::string KVFile::swrite(std::string fileOut, std::string options){ //TODO: Impliment write()

	std::string kvf; //This is the string which will contain the file

	bool vertical_mode = false;
	bool optimize = false;
	bool decapitate = false;
	bool skip_version = false;
	std::string term_char = "";
	bool sort_mats = false;
	bool show_descriptions = true;

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
	if (options.find("s", 0) != std::string::npos){
		sort_mats = true;
	}
	if (options.find("u", 0) != std::string::npos){
		show_descriptions = false;
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

		//Write type, name, value
		switch(variablesFlat[i].type){
			case('d'):
				kvf = kvf + "d " + variablesFlat[i].name + " " + to_gstring(variablesFlat[i].d) + term_char; //Add variable
				if (variablesFlat[i].description.length() > 0 && show_descriptions) kvf = kvf + " ?" + variablesFlat[i].description; //Add description if applicable
				kvf = kvf + "\n"; //Add newline
				break;
			case('b'):
				kvf = kvf + "b " + variablesFlat[i].name + " " + bool_to_string(variablesFlat[i].b) + term_char;  //Add variable
				if (variablesFlat[i].description.length() > 0 && show_descriptions) kvf = kvf + " ?" + variablesFlat[i].description; //Add description if applicable
				kvf = kvf + "\n"; //Add newline
				break;
			case('s'):
				kvf = kvf + "s " + variablesFlat[i].name + " \"" + variablesFlat[i].s + "\"" + term_char;  //Add variable
				if (variablesFlat[i].description.length() > 0 && show_descriptions) kvf = kvf + " ?" + variablesFlat[i].description; //Add description if applicable
				kvf = kvf + "\n"; //Add newline
				break;
			default:
				return ""; //An error occured
		}

	}

	//Sort matrices if requested or requried (required for vertical writing)
	if (sort_mats || vertical_mode) sortMatrices();

	if (vertical_mode){ //******************** Write matrices - Vertical Mode ***********//

		//TODO: Optimized veritcal mode not using KTable, instead using spaces

		{
			//************************ 1D ******************************

			//Start vertical block
			kvf = kvf + "#VERTICAL\n";
			if (!optimize) kvf = kvf + "\n";

			//To keep everything aligned, use KTable
			KTable kt;

			//Write types
			//
			std::vector<std::string> trow;
			for (size_t i = 0 ; i < variables1D.size() ; i++){
				switch(variables1D[i].type){
					case('d'):
						trow.push_back("m<d>");
						break;
					case('b'):
						trow.push_back("m<b>");
						break;
					case('s'):
						trow.push_back("m<s>");
						break;
					default:
						return ""; //An error occured
				}
			}
			//
			kt.row(trow);

			//Write names
			//
			trow.clear();
			for (size_t i = 0 ; i < variables1D.size() ; i++){
				trow.push_back(variables1D[i].name);
			}
			//
			kt.row(trow);



			//Write descriptions
			//
			if (show_descriptions){

				trow.clear();

				bool all_descr_blank = true; //Will let code skip description if all blank

				trow.clear();
				for (size_t i = 0 ; i < variables1D.size() ; i++){ //Get descr for 1D vars

					if (variables1D[i].description.find("?", 0) != std::string::npos){ //Skip desc if it contains key symbol '?''
						trow.push_back("?");
					}else{
						trow.push_back("?" + variables1D[i].description); //Else print desc
						all_descr_blank = false;
					}

				}

				if (!all_descr_blank) kt.row(trow);

			}

			//Write data lines
			//
			bool still_printing;
			size_t row = 0;
			do{ //While any matrix still has data to print...

				trow.clear();

				still_printing = false;

				for (size_t i = 0 ; i < variables1D.size() ; i++){ //For each 1D matrix...

					switch(variables1D[i].type){ //Find out its type...
						case('d'):
							if (row < variables1D[i].md.size()){ //See if it has data to print...
								still_printing  = true; //Set printing to true
								trow.push_back(to_gstring(variables1D[i].md[row])); //Add its data point
							}
							break;
						case('b'):
							if (row < variables1D[i].mb.size()){ //See if it has data to print...
								still_printing  = true; //Set printing to true
								trow.push_back(bool_to_string(variables1D[i].mb[row])); //Add its data point
							}
							break;
						case('s'):
							if (row < variables1D[i].ms.size()){ //See if it has data to print...
								still_printing  = true; //Set printing to true
								trow.push_back("\"" + variables1D[i].ms[row] + "\""); //Add its data point
							}
							break;
					}

				}

				kt.row(trow);

				row++;

			}while(still_printing);




			init_ktable(kt); //TODO: Alignment is wrong if this is NOT caleld at end. Put's left align columns right aligned.


			//Add table's string as vertical matrix statement
			kvf = kvf + kt.str();


			//End vertical block
			if (!optimize) kvf = kvf + "\n";
			kvf = kvf + "#VERTICAL\n";
		}

		{
			//******************************** 2D ******************************

			//Start vertical block
			if (!optimize) kvf = kvf + "\n";
			kvf = kvf + "#VERTICAL\n";
			if (!optimize) kvf = kvf + "\n";

			//To keep everything aligned, use KTable
			KTable kt;

			//Write types
			//
			std::vector<std::string> trow;
			for (size_t i = 0 ; i < variables2D.size() ; i++){
				switch(variables2D[i].type){
					case('d'):
						trow.push_back("m<d>");
						break;
					case('b'):
						trow.push_back("m<b>");
						break;
					case('s'):
						trow.push_back("m<s>");
						break;
					default:
						return ""; //An error occured
				}
			}
			//
			kt.row(trow);

			//Write names
			//
			trow.clear();
			for (size_t i = 0 ; i < variables2D.size() ; i++){
				trow.push_back(variables2D[i].name);
			}
			//
			kt.row(trow);

			//Write descriptions
			//
			if (show_descriptions){

				trow.clear();

				bool all_descr_blank = true; //Will let code skip description if all blank

				trow.clear();
				for (size_t i = 0 ; i < variables2D.size() ; i++){ //Get descrs for 2D vars

					if (variables2D[i].description.find("?", 0) != std::string::npos){ //Skip desc if it contains key symbol '?''
						trow.push_back("?");
					}else{
						trow.push_back("?" + variables2D[i].description); //Else print desc
						all_descr_blank = false;
					}

				}

				if (!all_descr_blank) kt.row(trow);

			}

			//Write data lines
			//
			bool still_printing;
			size_t row = 0;
			do{ //While any matrix still has data to print...

				trow.clear();

				still_printing = false;
				for (size_t i = 0 ; i < variables2D.size() ; i++){ //For each 2D variable

					switch(variables2D[i].type){ //Find out its type...
						case('d'):
							if (row < matrixLength(variables2D[i])){ //See if it has data to print...
								still_printing  = true; //Set printing to true
								std::string tstr = to_gstring(linaccess(variables2D[i].md2, row));
								if (isRowEnd(variables2D[i], row)) tstr = tstr + ";";
								trow.push_back(tstr); //Add its data point

							}
							break;
						case('b'):
							if (row < matrixLength(variables2D[i])){ //See if it has data to print...
								still_printing  = true; //Set printing to true
								std::string tstr = bool_to_string(linaccess(variables2D[i].mb2, row));
								if (isRowEnd(variables2D[i], row)) tstr = tstr + ";";
								trow.push_back(tstr); //Add its data point
							}
							break;
						case('s'):
							if (row < matrixLength(variables2D[i])){ //See if it has data to print...
								still_printing  = true; //Set printing to true
								std::string tstr = "\"" + linaccess(variables2D[i].ms2, row) + "\"";
								if (isRowEnd(variables2D[i], row)) tstr = tstr + ";";
								trow.push_back(tstr); //Add its data point
							}
							break;
					}
				}

				kt.row(trow);

				row++;

			}while(still_printing);




			init_ktable(kt); //TODO: Alignment is wrong if this is NOT caleld at end. Put's left align columns right aligned.


			//Add table's string as vertical matrix statement
			kvf = kvf + kt.str();


			//End vertical block
			if (!optimize) kvf = kvf + "\n";
			kvf = kvf + "#VERTICAL\n";
		}


	}else{ //******************** Write matrices - Horizontal Mode **********************//

		//********************** Write 1D Variables - Horizontal ****************************//

		//For each variable...
		for (size_t i = 0 ; i < variables1D.size() ; i++){

			switch(variables1D[i].type){
				case('d'):

					//print type, name, open brackets
					kvf = kvf + "m<d> " + variables1D[i].name + " [";

					//For each element...
					for (size_t k = 0 ; k < variables1D[i].md.size() ; k++){
						if (k != 0) kvf = kvf + ", "; //Add comma if not first element
						kvf = kvf + to_gstring(variables1D[i].md[k]); //Add variable string
					}

					kvf = kvf + "]" + term_char;  //Add termination
					if (variables1D[i].description.length() > 0 && show_descriptions) kvf = kvf + " ?" + variables1D[i].description; //Add description if applicable
					kvf = kvf + "\n"; //Add newline
					break;
				case('b'):

					//print type, name, open brackets
					kvf = kvf + "m<b> " + variables1D[i].name + " [";

					//For each element...
					for (size_t k = 0 ; k < variables1D[i].mb.size() ; k++){
						if (k != 0) kvf = kvf + ", "; //Add comma if not first element
						kvf = kvf + bool_to_string(variables1D[i].mb[k]); //Add variable string
					}

					kvf = kvf + "]" + term_char;  //Add termination
					if (variables1D[i].description.length() > 0 && show_descriptions) kvf = kvf + " ?" + variables1D[i].description; //Add description if applicable
					kvf = kvf + "\n"; //Add newline
					break;
				case('s'):

					//print type, name, open brackets
					kvf = kvf + "m<s> " + variables1D[i].name + " [";

					//For each element...
					for (size_t k = 0 ; k < variables1D[i].ms.size() ; k++){
						if (k != 0) kvf = kvf + ", "; //Add comma if not first element
						kvf = kvf + "\"" + variables1D[i].ms[k] + "\""; //Add variable string
					}

					kvf = kvf + "]" + term_char;  //Add termination
					if (variables1D[i].description.length() > 0 && show_descriptions) kvf = kvf + " ?" + variables1D[i].description; //Add description if applicable
					kvf = kvf + "\n"; //Add newline
					break;

				default:
					return ""; //An error occured
			}
		}

		//********************** Write 2D Variables - Horizontal ****************************//

		//For each variable...
		for (size_t i = 0 ; i < variables2D.size() ; i++){

			switch(variables2D[i].type){
				case('d'):

					//print type, name, open brackets
					kvf = kvf + "m<d> " + variables2D[i].name + " [";

					//For each row...
					for (size_t k = 0 ; k < variables2D[i].md2.size() ; k++){

						if (k != 0) kvf = kvf + "; "; //Add semicolon if not first row

						//Write row
						for (size_t j = 0 ; j < variables2D[i].md2[k].size() ; j++){ //For each element...
							if (j != 0) kvf = kvf + ", "; //Add comma if not first element
							kvf = kvf + to_gstring(variables2D[i].md2[k][j]); //Add variable string
						}
					}

					kvf = kvf + "]" + term_char;  //Add termination
					if (variables2D[i].description.length() > 0 && show_descriptions) kvf = kvf + " ?" + variables2D[i].description; //Add description if applicable
					kvf = kvf + "\n"; //Add newline
					break;
				case('b'):

					//print type, name, open brackets
					kvf = kvf + "m<b> " + variables2D[i].name + " [";

					//For each row...
					for (size_t k = 0 ; k < variables2D[i].mb2.size() ; k++){

						if (k != 0) kvf = kvf + "; "; //Add semicolon if not first row

						//Write row
						for (size_t j = 0 ; j < variables2D[i].mb2[k].size() ; j++){ //For each element...
							if (j != 0) kvf = kvf + ", "; //Add comma if not first element
							kvf = kvf + bool_to_string(variables2D[i].mb2[k][j]); //Add variable string
						}
					}

					kvf = kvf + "]" + term_char;  //Add termination
					if (variables2D[i].description.length() > 0 && show_descriptions) kvf = kvf + " ?" + variables2D[i].description; //Add description if applicable
					kvf = kvf + "\n"; //Add newline
					break;
				case('s'):

					//print type, name, open brackets
					kvf = kvf + "m<s> " + variables2D[i].name + " [";

					//For each row...
					for (size_t k = 0 ; k < variables2D[i].ms2.size() ; k++){

						if (k != 0) kvf = kvf + "; "; //Add semicolon if not first row

						//Write row
						for (size_t j = 0 ; j < variables2D[i].ms2[k].size() ; j++){ //For each element...
							if (j != 0) kvf = kvf + ", "; //Add comma if not first element
							kvf = kvf + "\"" + variables2D[i].ms2[k][j] + "\""; //Add variable string
						}
					}

					kvf = kvf + "]" + term_char;  //Add termination
					if (variables2D[i].description.length() > 0 && show_descriptions) kvf = kvf + " ?" + variables2D[i].description; //Add description if applicable
					kvf = kvf + "\n"; //Add newline
					break;
				default:
					return ""; //An error occured
			}
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

//***************************************************************************//
//**			PRIVATE FUNCTION DEFINITIONS							   **//
//***************************************************************************//

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
Sorts the 1D and 2D matrices from largest to smallest.
*/
void KVFile::sortMatrices(){

	//Sort 1D vector
	for (size_t i = 1 ; i < variables1D.size() ; i++){ //For each matrix (skipping first) ...

		//Get length
		size_t el = matrixLength(variables1D[i]);

		size_t j = i-1; //Get position to compare against
		while (el > matrixLength(variables1D[j])){ //Keep checking new indeces until you find a larger or equal size matrix

			//The current matrix is larger than the previous matrix...

			if (j > 0){ //If possible, move compare index closer to 0
				j--;
			}else{ //Else break - you're at the beginning
				break;
			}
		}

		//Move matrix if required
		if (j+1 == i){
			KV1DItem temp = variables1D[i]; //Make a copy of the current matrix

			variables1D.erase(variables1D.begin() + i); 			//Erase at old location
			variables1D.insert(variables1D.begin() + j+1, temp);	//Insert into new location
		}

	}

	//Sort 2D vector
	for (size_t i = 1 ; i < variables2D.size() ; i++){ //For each matrix (skipping first) ...

		//Get length
		size_t el = matrixLength(variables2D[i]);

		size_t j = i-1; //Get position to compare against
		while (el > matrixLength(variables2D[j])){ //Keep checking new indeces until you find a larger or equal size matrix

			//The current matrix is larger than the previous matrix...

			if (j > 0){ //If possible, move compare index closer to 0
				j--;
			}else{ //Else break - you're at the beginning
				break;
			}
		}

		//Move matrix if required
		if (j+1 == i){
			KV2DItem temp = variables2D[i]; //Make a copy of the current matrix

			variables2D.erase(variables2D.begin() + i); 			//Erase at old location
			variables2D.insert(variables2D.begin() + j+1, temp);	//Insert into new location
		}

	}

}

/*
Calculates the total number of elements in the KVitem and returns it.
*/
size_t KVFile::matrixLength(KV1DItem m){

	switch(m.type){
		case('d'):
			return m.md.size();
			break;
		case('s'):
			return m.ms.size();
			break;
		case('b'):
			return m.mb.size();
			break;
		default:
			return 0;
			break;
	}
}

/*
Calculates the total number of elements (in all rows and cols, combined) in the
KVitem and returns it.
*/
size_t KVFile::matrixLength(KV2DItem m){

	size_t l;

	switch(m.type){
		case('d'):

			l = 0;
			for (size_t j = 0 ; j < m.md2.size() ; j++){ //For each row...
				l += m.md2[j].size(); //Get size, increment sum
			}

			return l; //Return sum

			break;
		case('s'):

			l = 0;
			for (size_t j = 0 ; j < m.ms2.size() ; j++){ //For each row...
				l += m.ms2[j].size(); //Get size, increment sum
			}

			return l; //Return sum

			break;
		case('b'):

			l = 0;
			for (size_t j = 0 ; j < m.mb2.size() ; j++){ //For each row...
				l += m.mb2[j].size(); //Get size, increment sum
			}

			return l; //Return sum

			break;
		default:
			return 0;
			break;
	}

}

/*
Finds the 'idx'th variable in m, acting as if each lower row is appended to the
end of the preceeding row. Makes 2D vector look 1D in terms of indexing.

Returns 'idx'-th value in 'm'. Returns -1 if idx out of range.
*/
double KVFile::linaccess(std::vector<std::vector<double> > m, size_t idx){

	size_t count = 0;

	for (size_t r = 0 ; r < m.size() ; r++){
		if (m[r].size() + count > idx){
			return m[r][idx - count];
		}else{
			count += m[r].size();
		}
	}

	return -1; //Default return if out of bounds
}

/*
Finds the 'idx'th variable in m, acting as if each lower row is appended to the
end of the preceeding row. Makes 2D vector look 1D in terms of indexing.

Returns 'idx'-th value in 'm'. Returns false if idx out of range.
*/
bool KVFile::linaccess(std::vector<std::vector<bool> > m, size_t idx){

	size_t count = 0;

	for (size_t r = 0 ; r < m.size() ; r++){
		if (m[r].size() + count > idx){
			return m[r][idx - count];
		}else{
			count += m[r].size();
		}
	}

	return ""; //Default false if out of bounds
}

/*
Finds the 'idx'th variable in m, acting as if each lower row is appended to the
end of the preceeding row. Makes 2D vector look 1D in terms of indexing.

Returns 'idx'-th value in 'm'. Returns blank if idx out of range.
*/
std::string KVFile::linaccess(std::vector<std::vector<std::string> > m, size_t idx){

	size_t count = 0;

	for (size_t r = 0 ; r < m.size() ; r++){
		if (m[r].size() + count > idx){
			return m[r][idx - count];
		}else{
			count += m[r].size();
		}
	}

	return ""; //Default return if out of bounds
}

/*
Treats a 2D matrix as 1D, pretending each row is appended to the preceeding row.
Returns true if the element at index 'idx' is actually the end of a row in the
real 2D matrix.

Also returns false if idx is out of bounds, or if any other error occurs.
*/
bool KVFile::isRowEnd(KV2DItem m, size_t idx){

	size_t count = 0;

	switch(m.type){
		case('d'):
			for (size_t r = 0 ; r < m.md2.size() ; r++){
				if (m.md2[r].size() + count > idx){
					return (idx == (count + m.md2[r].size()-1));
				}else{
					count += m.md2[r].size();
				}
			}
			break;
		case('b'):
			for (size_t r = 0 ; r < m.mb2.size() ; r++){
				if (m.mb2[r].size() + count > idx){
					return (idx == (count + m.mb2[r].size()-1));
				}else{
					count += m.mb2[r].size();
				}
			}
			break;
		case('s'):
			for (size_t r = 0 ; r < m.ms2.size() ; r++){
				if (m.ms2[r].size() + count > idx){
					return (idx == (count + m.ms2[r].size()-1));
				}else{
					count += m.ms2[r].size();
				}
			}
			break;
		default:
			return false;
	}

	return false; //Default return if out of bounds

}

/*
Initializes the KTable for use in generating vertical KV matrix statements.
*/
void KVFile::init_ktable(KTable& kt){

	kt.table_title("");
	kt.set(KTABLE_INTERWALLS, false);
	kt.set(KTABLE_SIDEWALLS, false);
	kt.set(KTABLE_TOPBOTTOMHBAR, false);
	kt.set(KTABLE_HEADERINTERWALLS, false);
	kt.set(KTABLE_HEADERHBAR, false);
	kt.set(KTABLE_TITLEHBAR, false);
	// kt.set(KTABLE_TITLEBAR, false); //TODO: KTable bug - if I turn title off, can't surpress preceeding ----s and on same line :\
	// kt.set(KTABLE_INTERHBARS, false);
	kt.alignh('l');
	kt.alignc('l');
	kt.alignt('l');

}

//***************************************************************************//
//**			NON-CLASS FUNCTION DEFINITIONS							   **//
//***************************************************************************//

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
