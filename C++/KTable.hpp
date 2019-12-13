//
//  KTable.hpp
//  
//
//  Created by Grant Giesbrecht on 27/2/19.
//

#ifndef KTable_hpp
#define KTable_hpp

//Settings options
#define KTABLE_SIDEWALLS 2
#define KTABLE_HEADERINTERWALLS 3
#define KTABLE_INTERWALLS 4
#define KTABLE_TITLEBAR 5
#define KTABLE_TOPBOTTOMHBAR 6
#define KTABLE_TITLEHBAR 7
#define KTABLE_INTERHBARS 8
#define KTABLE_COLPADDING 9
#define KTABLE_WALLCHAR 10
#define KTABLE_HBARCHAR 11
#define KTABLE_JOINTCHAR 12
#define KTABLE_HEADERHBAR 13

#include <stdio.h>
#include <string>
#include <vector>
#include <sstream>

class KTable{
	
public:
	
	KTable(); //Initialize the class
	
	void row(std::vector<std::string> nrow); //Add a row to the table

	std::string str(); //Generate a string containing the entire table
	std::string str(size_t line); //Generate a string for the specified line of the table
	std::string nxtstr(); //Generate a string for the next line of the table
	
	size_t num_rows(); //Get the number of rows that will be used to print the entire table (not just rows of data)
	
	void table_title(std::string nt); //Set the table's title
	
	void alignc(size_t col_number, char alignment); //Set the alignment of the specified data column
	void alignc(char alignment); //Set the alignment of all data columns
	void alignh(size_t col_number, char alignment); //Set the alignment of the specified header column
	void alignh(char alignment); //Set the alignment of all header columns
	void alignt(char alignment); //Set the alignment of the table title
	
	void set(short parameter, double value); //Set a parameter
	void set(short parameter, bool value); //Set a parameter
	void set(short parameter, char value); //Set a parameter
	
	bool nxtgood(); //Returns false when nxtstr_line is reset to zero
	
	static std::string parallel(std::vector<KTable> tables, std::vector<std::string> spacers, std::vector<size_t> offset);
	
	size_t getWidth(); //Get the width of the table (ie. the total number of characters across at the thickest row).
	
	void nxtreset(); //Reset nxtstr() and nxtgood() so they feed the entire table back again
	
private:
	
	size_t ncols; //Constantly updated variable containing the number of data columns in the table (not the number of characters across)
	
	std::vector<std::vector<std::string> > contents; //Matrix containing the data elements
	std::string title; //Title of the table
	
	std::vector<std::string> table; //Vector of each line of the table. This is updated when str(size_t) is called & 'table_up_to_date' is false and when getWidth() is called and 'table_up_to_date' is false.
	
	std::vector<size_t> column_widths; //Contains the number of spaces allocated for each column. Based on data element length, padding, and title length.
	std::vector<char> alignment; //Alignment of data columns. (Options: l, c, r)
	std::vector<char> header_alignment; //Alignment of header columns. (Options: l, c, r)
	char title_alignment; //Alignment of title. (Options: l, c, r)
	
	bool print_sidewalls; //Walls at ends of table
	bool print_headerinterwalls; //Walls between columns in header bar
	bool print_interwalls; //Walls between columns
	bool print_titlebar; //Bar at very top of table w/ a table title
	bool print_topbottomhbar; //horiz. bar at top and bottom
	bool print_titlehbar; //horiz. bar between title and col. headers
	bool print_headerhbar; //horiz. bar between col. headers and data mtrx
	bool print_interhbars; //horiz. bar between data mtrx rows
	size_t col_padding; //Number of extra spaces to include per column (default: 2)
	
	char wall_char; //Character to use on verticles of table
	char hbar_char; //Character to use on horizontals of table
	char joint_char; //Characters to use at verticle/horizontal intersections of table
	
	size_t nxtstr_line; //The next line of the table for nxtstr() to return
	bool next_is_good; //Always 'true' unless nxtstr_line was set to zero after running through the entire table and 'nxtstr()' hasn't been called since
	
	bool table_up_to_date; //Tells the class when the 'table' vector needs to be recalculated
	
	void generate_table(); //Generates the table and saves it in 'table'
	
	char default_alignment; //Default alignment to assign to new data columns
	char default_header_alignment; //Default alignment to assign to new header columns
	
	
	
	//From Synaptik ( https://stackoverflow.com/questions/14765155/how-can-i-easily-format-my-data-table-in-c )
	/* Convert double to string with specified number of places after the decimal
	 and left padding. */
	std::string prd(const double x, const int decDigits, const int width) {
		std::stringstream ss;
		ss << std::fixed << std::right;
		ss.fill(' ');        // fill space around displayed #
		ss.width(width);     // set  width around displayed #
		ss.precision(decDigits); // set # places after decimal
		ss << x;
		return ss.str();
	}
	
	//Modified from Synaptik by G Giesbrecht
	/* Convert double to string with specified number of places after the decimal
	 but without padding. */
	std::string prd(const double x, const int decDigits){
		std::stringstream ss;
		ss << std::fixed << std::right;
		ss.precision(decDigits); // set # places after decimal
		ss << x;
		return ss.str();
	}
	
	//From Synaptik ( https://stackoverflow.com/questions/14765155/how-can-i-easily-format-my-data-table-in-c )
	/*! Center-aligns string within a field of width w. Pads with blank spaces
	 to enforce alignment. */
	std::string center(const std::string s, const int w) {
		std::stringstream ss, spaces;
		int padding = w - s.size();                 // count excess room to pad
		for(int i=0; i<padding/2; ++i)
			spaces << " ";
		ss << spaces.str() << s << spaces.str();    // format with padding
		if(padding>0 && padding%2!=0)               // if odd #, add 1 space
			ss << " ";
		return ss.str();
	}
	
	//From Synaptik ( https://stackoverflow.com/questions/14765155/how-can-i-easily-format-my-data-table-in-c )
	/* Right-aligns string within a field of width w. Pads with blank spaces
	 to enforce alignment. */
	std::string right(const std::string s, const int w) {
		std::stringstream ss, spaces;
		int padding = w - s.size();                 // count excess room to pad
		for(int i=0; i<padding; ++i)
			spaces << " ";
		ss << spaces.str() << s;                    // format with padding
		return ss.str();
	}
	
	//From Synaptik ( https://stackoverflow.com/questions/14765155/how-can-i-easily-format-my-data-table-in-c )
	/*! Left-aligns string within a field of width w. Pads with blank spaces
	 to enforce alignment. */
	std::string left(const std::string s, const int w) {
		std::stringstream ss, spaces;
		int padding = w - s.size();                 // count excess room to pad
		for(int i=0; i<padding; ++i)
			spaces << " ";
		ss << s << spaces.str();                    // format with padding
		return ss.str();
	}
};



#endif /* KTable_hpp */
