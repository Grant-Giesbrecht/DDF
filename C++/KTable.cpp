//
//  KTable.cpp
//  
//
//  Created by Grant Giesbrecht on 27/2/19.
//

#include "KTable.hpp"
#include <iostream>
#include <cmath>

using namespace std;

/*
 Constuctor - initializes the class
 */
KTable::KTable(){
	
	title = "Title";
	title_alignment = 'c';
	
	ncols = 0;
	
	print_sidewalls = true; //Walls at ends of table
	print_headerinterwalls = true; //Walls between columns in header bar
	print_interwalls = true; //Walls between columns in data matrix
	print_titlebar = false; //Bar at very top of table w/ a table title
	print_topbottomhbar = true; //horiz. bar at top and bottom
	print_titlehbar = true; //horiz. bar between title and col. headers
	print_headerhbar = true; //horiz. bar between col. headers and data mtrx
	print_interhbars = false; //horiz. bar between data mtrx rows
	col_padding = 2; //Number of extra spaces to include per column
	
	wall_char = '|';
	hbar_char = '-';
	joint_char = '+';
	
	nxtstr_line = 0; //The next line of the tab
	next_is_good = true;
	
	table_up_to_date = false; //Tells the class when the 'table' vector needs to be recalculated
	
	default_alignment = 'r';
	default_header_alignment = 'c';
	
}

/*
 Adds a row to the table. If the row isn't the same size as the previous rows, the
 size of all rows will be adjusted to match the largest row by adding empty cells to
 the shorter rows.
 
 nrow - new row to add
 
 Void return
 
 Example usage: KTable kt(); kt.row({"element 1", "element 2", "element 3"});
 */
void KTable::row(std::vector<std::string> nrow){
	
	//Make sure all rows are the same size...
	if (nrow.size() > ncols){ //If the new row is too big...
		for (size_t row = 0 ; row < contents.size() ; row++){ //For each old row...
			do{
				contents[row].push_back(""); //Add blank cells until its the same size
			}while(nrow.size() > ncols);
		}
		ncols = nrow.size(); //Update the size of 'ncols'

		//Update the alignment
		while (alignment.size() < ncols){
			alignment.push_back(default_alignment);
			header_alignment.push_back(default_header_alignment);
		}
		
	}else if(nrow.size() < ncols){ //If the new row is too small...
		do{
			nrow.push_back(""); //Add blank cells to the new row until its the same size
		}while(nrow.size() < ncols);
	}
	
	contents.push_back(nrow); //Add the row
	
	table_up_to_date = false; //Specify that the table will need to be regenerated
	
}

/*
 Returns a string containing the entire table.
 
 Example usage: cout << kt.str() << endl;
 */
std::string KTable::str(){
	
	string table_string = "";
	
	size_t nr = num_rows();
	for (size_t r = 0 ; r < num_rows() ; r++){
		table_string = table_string + str(r) + "\n";
	}
	
	return table_string;
}

/*
 Returns the line of the table designated by 'line'.
 
 Example usage: cout << kt1.str(0) << "\t\t" << kt2.str(0) << endl;
 */
std::string KTable::str(size_t line){
	
	//Generate the table
	if (!table_up_to_date){
		generate_table();
	}
	
	string out = "";
	if (line <= table.size() -1 ){
		out = table[line];
	}else{
		out = " -- LINE " + to_string(line) + " OUT OF BOUNDS -- ";
	}

	return out;
}

/*
 Returns a single line of the table, starting at line 0, then moving to
 the next line down with each call of the function until the entire table
 is returned (at which point it returns to line 0).
 
 Example usage: cout << kt1.nxtstr() << "\t\t" << kt2.nxtstr() << endl;
 */
std::string KTable::nxtstr(){
	next_is_good = true;
	string table_line_str = str(nxtstr_line++); //Get next line...
	if (nxtstr_line > num_rows()-1){
		nxtstr_line = 0; //Set next next line to 0 if whole table given
		next_is_good = false; //Set to false (this makes it easy to use nxtstr in a loop)
	}
	return table_line_str; //Return the string
}

/*
 Calculates and returns the number of rows in the table.
 */
size_t KTable::num_rows(){
	
	size_t nr = 0;
	nr = contents.size();
	if (print_titlebar) nr++;
	if (print_topbottomhbar) nr += 2;
	if (print_titlehbar && print_titlebar) nr++;
	if (print_headerhbar) nr++;
	if (print_interhbars && contents.size() > 1) nr += contents.size()-2;
	
	return nr;
	
}

/*
 Generates the table and saves it internally to the 'vector<string> table'.
 */
void KTable::generate_table(){
	
	//Erase any outdated table
	table.clear();
	
	//------------------------- Calculate column widths -----------------
	//
	
	size_t title_space = 0;
	column_widths.clear();
	for (size_t cc = 0 ; cc < ncols ; cc++){ //For each column...
		column_widths.push_back(0); //Set width to zero
		for (size_t rr = 0 ; rr < contents.size() ; rr++){ //Then check each row...
			if (contents[rr][cc].length() > column_widths[cc]){ //And if the element is bigger than the recorded max...
				column_widths[cc] = contents[rr][cc].length(); //Record the new max
			}
		}
		column_widths[cc] += col_padding; //Then add the padding to the column
		title_space += column_widths[cc];
	}
	
	//Calculate width of table and modify if title doesn't fit
	if (print_interwalls || print_headerinterwalls) title_space += ncols-1; //Add to space available to title if verticals expand table
	if (title.length() > title_space){ //If there isn't enough room for the title in the table...
		size_t extra = title.length() - title_space; //Calculate how much additional room is needed
		size_t cc = 0;
		for (; extra > 0 ; extra--){ //Then, until the additional room needed is zero...
			column_widths[cc]++; //Add one character to each column...
			if (cc >= column_widths.size()) cc = 0; //and cycle back to the first column when all have been incremented
		}
	}
	
	//---------------------------- Generate Table -------------------------
	//
	
	string table_line = "";
	
	//Top bar
	if (print_sidewalls) table_line = table_line + joint_char; //Add joint char for side-wall
	for(size_t c = 0; c < ncols ; c++){
		table_line = table_line + string(column_widths[c], hbar_char); //Create hbar over column
		if (c+1 < ncols){ // Add hbar_char inbetween columns
			if (print_titlebar || !print_headerinterwalls){ //... No mid-joints required
				table_line = table_line + hbar_char;
			}else{ //... Mid-joints required
				table_line = table_line + joint_char;
			}
			
		}
	}
	if (print_sidewalls) table_line = table_line + joint_char; //Add joint char for side-wall
	size_t table_width = table_line.length(); //Save table width
	if (print_topbottomhbar){ //Add to table if required
		table.push_back(table_line);
		table_line = "";
	}
	
	//Title bar
	if (print_titlebar){
		size_t title_length = table_width; //Size of title is size of table...
		if (print_sidewalls) title_length -= 2; //... unless side walls are present, in which case subtract 2
		if (title_alignment == 'c'){ //Generate title bar
			table_line = center(title, title_length);
		}else if (title_alignment == 'r'){
			table_line = right(title, title_length);
		}else{
			table_line = left(title, title_length);
		}
		if (print_sidewalls) table_line = wall_char + table_line + wall_char; //Add wall chars if side walls present
		table.push_back(table_line);
		table_line = "";
	}
	
	//Title horiz. bar
	if (print_titlehbar && print_titlebar){ //Print title horiz. bar
		if (print_sidewalls) table_line = table_line + joint_char; //Add joint char for side-wall
		for(size_t c = 0; c < ncols ; c++){
			table_line = table_line + string(column_widths[c], hbar_char); //Create hbar over column
			if (c+1 < ncols){ // Add hbar_char inbetween columns
				if (!print_headerinterwalls){ //... No mid-joints required
					table_line = table_line + hbar_char;
				}else{ //... Mid-joints required
					table_line = table_line + joint_char;
				}
				
			}
		}
		if (print_sidewalls) table_line = table_line + joint_char; //Add joint char for side-wall
		table.push_back(table_line);
		table_line = "";
	}
	
	//Header bar
	if (print_sidewalls) table_line = table_line + wall_char; //Add joint char for side-wall
	for(size_t c = 0; c < ncols ; c++){
		
		if (header_alignment[c] == 'l'){ //left
			if (col_padding >= 2){
				table_line = table_line + ' ' + left(contents[0][c], column_widths[c]-2) + ' ';
			}else if (col_padding == 1){
				table_line = table_line + ' ' + left(contents[0][c], column_widths[c]-1);
			}else{
				table_line = table_line + left(contents[0][c], column_widths[c]);
			}
		}else if(header_alignment[c] == 'r'){ //right
			if (col_padding >= 2){
				table_line = table_line + ' ' + right(contents[0][c], column_widths[c]-2) + ' ';
			}else if (col_padding == 1){
				table_line = table_line + ' ' + right(contents[0][c], column_widths[c]-1);
			}else{
				table_line = table_line + right(contents[0][c], column_widths[c]);
			}
		}else{ //'c', center
			if (col_padding >= 2){
				table_line = table_line + ' ' + center(contents[0][c], column_widths[c]-2) + ' ';
			}else if (col_padding == 1){
				table_line = table_line + ' ' + center(contents[0][c], column_widths[c]-1);
			}else{
				table_line = table_line + center(contents[0][c], column_widths[c]);
			}
		}
		
		if (c+1 < ncols){ // Add wall or space inbetween columns
			if (!print_headerinterwalls){ //... No wall required
				table_line = table_line + ' ';
			}else{ //... wall required
				table_line = table_line + wall_char;
			}
			
		}
	}
	if (print_sidewalls) table_line = table_line + wall_char; //Add joint char for side-wall
	table.push_back(table_line);
	table_line = "";
	
	//Header horiz. bar
	if (print_headerhbar){ //Print header horiz. bar
		if (print_sidewalls) table_line = table_line + joint_char; //Add joint char for side-wall
		for(size_t c = 0; c < ncols ; c++){
			table_line = table_line + string(column_widths[c], hbar_char); //Create hbar over column
			if (c+1 < ncols){ // Add hbar_char inbetween columns
				if (!print_interwalls && !print_headerinterwalls){ //... No mid-joints required
					table_line = table_line + hbar_char;
				}else{ //... Mid-joints required
					table_line = table_line + joint_char;
				}
				
			}
		}
		if (print_sidewalls) table_line = table_line + joint_char; //Add joint char for side-wall
		table.push_back(table_line);
		table_line = "";
	}
	
	for (size_t rr = 1 ; rr < contents.size() ; rr++){
		
		//Content bar
		if (print_sidewalls) table_line = table_line + wall_char; //Add joint char for side-wall
		for(size_t c = 0; c < ncols ; c++){
			
			if (alignment[c] == 'l'){ //left
				if (col_padding >= 2){
					table_line = table_line + ' ' + left(contents[rr][c], column_widths[c]-2) + ' ';
				}else if (col_padding == 1){
					table_line = table_line + ' ' + left(contents[rr][c], column_widths[c]-1);
				}else{
					table_line = table_line + left(contents[rr][c], column_widths[c]);
				}
			}else if(alignment[c] =='r'){ //right
				if (col_padding >= 2){
					table_line = table_line + ' ' + right(contents[rr][c], column_widths[c]-2) + ' ';
				}else if (col_padding == 1){
					table_line = table_line + ' ' + right(contents[rr][c], column_widths[c]-1);
				}else{
					table_line = table_line + right(contents[rr][c], column_widths[c]);
				}
			}else{ //'c', center
				if (col_padding >= 2){
					table_line = table_line + ' ' + center(contents[rr][c], column_widths[c]-2) + ' ';
				}else if (col_padding == 1){
					table_line = table_line + ' ' + center(contents[rr][c], column_widths[c]-1);
				}else{
					table_line = table_line + center(contents[rr][c], column_widths[c]);
				}
			}
			
			if (c+1 < ncols){ // Add wall or space inbetween columns
				if (!print_interwalls){ //... No wall required
					table_line = table_line + ' ';
				}else{ //... wall required
					table_line = table_line + wall_char;
				}
				
			}
		}
		if (print_sidewalls) table_line = table_line + wall_char; //Add joint char for side-wall
		table.push_back(table_line);
		table_line = "";
		
		//inter horiz. bar
		if (print_interhbars && rr+1 < contents.size()){ //Print header horiz. bar
			if (print_sidewalls) table_line = table_line + joint_char; //Add joint char for side-wall
			for(size_t c = 0; c < ncols ; c++){
				table_line = table_line + string(column_widths[c], hbar_char); //Create hbar over column
				if (c+1 < ncols){ // Add hbar_char inbetween columns
					if (!print_interwalls){ //... No mid-joints required
						table_line = table_line + hbar_char;
					}else{ //... Mid-joints required
						table_line = table_line + joint_char;
					}
				}
			}
			if (print_sidewalls) table_line = table_line + joint_char; //Add joint char for side-wall
			table.push_back(table_line);
			table_line = "";
		}
	}
	
	//Bottom bar
	if (print_topbottomhbar){ //Print bottom bar
		if (print_sidewalls) table_line = table_line + joint_char; //Add joint char for side-wall
		for(size_t c = 0; c < ncols ; c++){
			table_line = table_line + string(column_widths[c], hbar_char); //Create hbar over column
			if (c+1 < ncols){ // Add hbar_char inbetween columns
				if (!print_interwalls){ //... No mid-joints required
					table_line = table_line + hbar_char;
				}else{ //... Mid-joints required
					table_line = table_line + joint_char;
				}
				
			}
		}
		if (print_sidewalls) table_line = table_line + joint_char; //Add joint char for side-wall
		table.push_back(table_line);
		table_line = "";
	}
	
	
	table_up_to_date = true;
	
}

/*
 Set parameters of the table, such as how to format the table
 
 parameter - item to change
 value - new value for the item
 
 Void return
 */
void KTable::set(short parameter, bool value){
	switch(parameter){
		case(KTABLE_SIDEWALLS):
			print_sidewalls = value;
			break;
		case(KTABLE_HEADERINTERWALLS):
			print_headerinterwalls = value;
			break;
		case(KTABLE_INTERWALLS):
			print_interwalls = value;
			break;
		case(KTABLE_TITLEBAR):
			print_titlebar = value;
			break;
		case(KTABLE_TOPBOTTOMHBAR):
			print_topbottomhbar = value;
			break;
		case(KTABLE_TITLEHBAR):
			print_titlehbar = value;
			break;
		case(KTABLE_INTERHBARS):
			print_interhbars = value;
			break;
		case(KTABLE_HEADERHBAR):
			print_headerhbar = value;
			break;
	}
}

/*
 Set parameters of the table, such as how to format the table
 
 parameter - item to change
 value - new value for the item
 
 Void return
 */
void KTable::set(short parameter, char value){
	switch(parameter){
		case(KTABLE_JOINTCHAR):
			joint_char = value;
			break;
		case(KTABLE_WALLCHAR):
			wall_char = value;
			break;
		case(KTABLE_HBARCHAR):
			hbar_char = value;
			break;
	}
}

/*
 Set parameters of the table, such as how to format the table
 
 parameter - item to change
 value - new value for the item
 
 Void return
 */
void KTable::set(short parameter, double value){
	switch(parameter){
		case(KTABLE_COLPADDING):
			col_padding = int(abs(value));
			break;
	}
}

/*
 Set the table's title
 
 nt - New title
 
 Void return
 */
void KTable::table_title(std::string nt){
	title = nt;
	
	print_titlebar = true;
	
	table_up_to_date = false;
	
}

/*
 Set the alignment for the specified data column
 
 col_number - index of column to set
 value - new value for the alignment. Options: 'l'=left, 'c'=center, 'r'=right
 
 Void return
 */
void KTable::alignc(size_t col_number, char value){
	if (value != 'c' && value != 'l' && value != 'r'){ //Ensure 'value' is valid
		value = default_alignment;
	}
	if (col_number > alignment.size()) return; //Return if col_number out of bounds
	alignment[col_number] = value; //Set parameter
}

/*
 Set the alignment of all data columns
 
 value - new value for the alignments. Options: 'l'=left, 'c'=center, 'r'=right
 
 Void return
 */
void KTable::alignc(char value){
	for (size_t col = 0 ; col < ncols ; col++){
		alignc(col, value);
	}
}

/*
 Set the alignment for the specified header column
 
 col_number - index of column to set
 value - new value for the alignment. Options: 'l'=left, 'c'=center, 'r'=right
 
 Void return
 */
void KTable::alignh(size_t col_number, char value){
	if (value != 'c' && value != 'l' && value != 'r'){ //Ensure 'value' is valid
		value = default_header_alignment;
	}
	if (col_number > header_alignment.size()) return; //Return if col_number out of bounds
	header_alignment[col_number] = value; //Set parameter
}

/*
 Set the alignment of all header columns
 
 value - new value for the alignments. Options: 'l'=left, 'c'=center, 'r'=right
 
 Void return
 */
void KTable::alignh(char value){
	for (size_t col = 0 ; col < ncols ; col++){
		alignh(col, value);
	}
}

/*
 Set the alignment of the title.
 
 value - new value for the alignments. Options: 'l'=left, 'c'=center, 'r'=right
 
 Void return
 */
void KTable::alignt(char value){
	if (value != 'c' && value != 'l' && value != 'r'){ //Ensure 'value' is valid
		value = default_header_alignment;
	}
	title_alignment = value; //Set parameter
}

/*
 This function is used to determine when nxtstr() reaches the end of the table. It returns true until nxtstr() reaches
 the end of the table and resets nxtstr_line to zero (at which point it becomes false). Once nxtstr() is called again or
 nxtreset() is called, this function returns true again.
 */
bool KTable::nxtgood(){
	return next_is_good;
}

/*
 This function is used to print two or more KTables next to eachother. Put all ktables you wish to plot parallel to
 eachother in the vector 'tables'.
 
 tables - vector of all the tables you wish to print on one line
 spacers - vector of the characters you wish to place inbetween the tables to space them apart. Spacers must have one
 	fewer elements than 'tables'. spacers[0] goes bewteen tables[0] and tables[1].
 offset - vector of the vertical offset for each column in terms of number of rows. Only positive integers are valid.
 
 Returns a string containing all 2+ tables printed parallel to eachother.
 
 Example usage: cout << KTable::parallel({ktable1, ktable2, ktable3}, {"\t\t", "   "}, {0, 1, 0}) << endl;
 */
std::string KTable::parallel(std::vector<KTable> tables, std::vector<std::string> spacers, std::vector<size_t> offset){
	
	string ptout;
	
	//Check sizes
	if ((tables.size() != (spacers.size()+1)) || (offset.size() != tables.size()) ){
		return "";
	}
	
	//Ensure tables aren't already 'not good' because of previous nxtstr() scans
	for (size_t tc = 0 ; tc < tables.size() ; tc++){
		if (!tables[tc].nxtgood()){
			tables[tc].nxtreset();
		}
	}
	
	//Print each line
	size_t count = 0;
	bool quit;
	while(true){
		
		//Print a line from each table...
		quit = true;
		for (size_t tc = 0 ; tc < tables.size() ; tc++){
			
			//Print spacer between tables if not the first table...
			if (tc > 0){
				ptout = ptout + spacers[tc-1];
			}
			
			//Print line from table if required
			if (offset[tc] <= count && tables[tc].nxtgood()){
				ptout = ptout + tables[tc].nxtstr();
			}else{ //Otherwise print spaces to fill up the void
				ptout = ptout + string(tables[tc].getWidth(), ' ');
			}
			
			//Determine when all tables are entirely printed:
			if (tables[tc].nxtgood()){ //If one or more tables has more lines...
				quit = false; //Do not quit
			}
		}
		
		//Add a newline character
		ptout = ptout + "\n";
		count++;
		
		//Exit the loop when all tables are done
		if (quit) break;
	}
	
	return ptout;
	
}

/*
 Returns the width (in characters, not data elements) of the table (at the thickest part)
 */
size_t KTable::getWidth(){
	
	//Generate the table
	if (!table_up_to_date){
		generate_table();
	}
	
	size_t width = 0;
	for (size_t tr = 0 ; tr < table.size() ; tr++){
		if (table[tr].length() > width ) width = table[tr].length();
	}
	
	return width;
}

/*
 Resets the nxtstr()'s line index counter so calls to nxtstr() restart at the top of the table.
 */
void KTable::nxtreset(){
	
	next_is_good = true;
	nxtstr_line = 0;
	
}
