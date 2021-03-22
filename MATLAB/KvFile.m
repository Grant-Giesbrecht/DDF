classdef KvFile < handle
	
	properties   %************************ PROPERTIES *********************
		varsFlat
		vars1D
		vars2D
		header
		fileVersion
		error_messages
		current_version
		current_version_str
	end   %********************** END PROPERTIES **************************
	
	methods   %************************ METHODS ***************************
		
		function obj=KvFile()	%*************** INITIALIZER **************
			obj.varsFlat = [];
			obj.vars1D = [];
			obj.vars2D = [];
			obj.header = "";
			obj.fileVersion = -1;
			obj.error_messages = [];
			obj.current_version = 2;
			obj.current_version_str = "2.0"
		end %******************************* END INITIALIZER **************
		
		function add(obj, newVar, varName, desc) %******** add() **********
			
			%Create new KV Item
			newItem = KvItem(newVar, varName, desc);
			
			%Verify variable not already in object
			if obj.checkContains(varName)
				obj.logErr(strcat("Failed to add variable '", varName, "' because a variable with it's name already exists"));
				return;
			end
			
			%Find correct group to add to
			if newItem.dimension == 1
				if length(obj.varsFlat) == 0
					obj.varsFlat = newItem
				else
					obj.varsFlat(end+1) = newItem;
				end
			elseif newItem.dimension == 2
				if length(obj.vars1D) == 0
					obj.vars1D = newItem
				else
					obj.vars1D(end+1) = newItem;
				end
			elseif newItem.dimension == 3
				if length(obj.vars2D) == 0
					obj.vars2D = newItem
				else
					obj.vars2D(end+1) = newItem;
				end
			else
				obj.logErr(strcat("Dimension exceeded 2D (d=", num2str(newItem.dimension) , ") - failed to add '", varName, "'"));
				return;
			end
			
		end %*********************************** END add() ****************
		
		function isHere=checkContains(obj, name) %***** checkContains() ***
			for kvi=obj.varsFlat
				if kvi.name == name
					isHere = true;
					return;
				end
			end
			for kvi=obj.vars1D
				if kvi.name == name
					isHere = true;
					return;
				end
			end
			for kvi=obj.vars2D
				if kvi.name == name
					isHere = true;
					return;
				end
			end
			
			isHere = false;
		end %********************* END checkContains() ********************
		
		function logErr(obj, msg) %************** logErr() ****************
			if length(obj.error_messages) == 0
				obj.error_messages = strcat('ERROR: ', msg);
				return;
			end
			obj.error_messages(end+1) = strcat('ERROR: ', msg);
		end %****************************** END logErr() ******************
		
		function logErrLn(obj, msg, lnum) %************** logErrLn() ******
			
			err_msg = strcat('Failed on line ', string(lnum), '. ', msg);
			obj.logErr(err_msg);
			
		end %**************************** END logErrLn() ******************
		
		function strout=err(obj)
			if length(obj.error_messages) == 0
				strout = "No errors";
				return;
			end
			strout=obj.error_messages(end);
		end
		
		function n=numVar(obj) %**************** numVar() *****************
			
			n = length(obj.varsFlat) + length(obj.vars1D) + length(obj.vars2D);
			
		end %************************ END numVar() ************************
		
		function clear(obj) %******************* clear() ******************
			obj.varsFlat = [];
			obj.vars1D = [];
			obj.vars2D = [];
		end %*************************** END clear() **********************
		
		function hdr=getHeader(obj) %************* getHeader() ************
			hdr = obj.header;
		end %*********************************** END getHeader() **********
		
		function setHeader(obj, hstr) %********** setHeader() *************
			obj.header = hstr;
		end %*************************** END setHeader() ******************
		
		function valid=isValidName(obj, name) %********** isValidName() *********
			
			%Convert to character array
			name = char(name);
			
			%Ensure name is at least 1 character
			if name == ""
				valid=false;
				return;
			end
			
			%Check that first character is letter
			if ~isletter(name(1))
				valid=false;
				return;
			end
			
			%For each character
			for c=name
				if isspace(c)
					valid=false;
					return;
				end
			end
			
			valid=true;
			return;
			
		end %*************************** END isValidName() ****************
		
		function sortMatrices(obj) %********* sortMatrices() **************
			
			%For 1D vectors
			[~, ind] = sort([obj.vars1D.count]);
			obj.vars1D = obj.vars1D(ind);
			
			%For 2D vectors
			[~, ind] = sort([obj.vars2D.count]);
			obj.vars1D = obj.vars2D(ind);
			
		end %************************** sortMatrices() ********************
		
		% Writes the currently loaded variables to a KV file on disk.
		%
		% Options: (Order does not matter. Case-sensitive)
		% 	v: Save all matrices as vertical
		%	o: Optimize size by removing lines intended to improve human-readability.
		%	d: Decapitate - ie. omit header
		%	-: Do not print version statement. WARNING: This will make the file unreadable.
		%		Don't use it unless you know what you're doing.
		%	;: Terminate variable statements with the optional semicolon
		%	s: Sort vectors largest to smallest
		%	u: Undocumented - variable descriptions are not printed
		%
		% In the event of an error, it returns a blank string
		function fstr=swrite(obj, options) %******* swrite() **************
			
			nl = string(newline);
			
			out = "";
			options = string(options);
			
			%Initialize options
			vertical_mode = false;
			optimize = false;
			decapitate = false;
			skip_version = false;
			term_char = '';
			sort_mats = false;
			show_descriptions = true;
			
			%Read options
			if ~isempty(find(char(options)=='v',1))
				vertical_mode = true;
			end
			if ~isempty(find(char(options)=='o',1))
				optimize = true;
			end
			if ~isempty(find(char(options)=='d',1))
				decapitate = true;
			end
			if ~isempty(find(char(options)=='-',1))
				skip_version = true;
			end
			if ~isempty(find(char(options)==';',1))
				term_char = ';';
			end
			if ~isempty(find(char(options)=='s',1))
				sort_mats = true;
			end
			if ~isempty(find(char(options)=='u',1))
				show_descriptions = true;
			end
			
			%Write version statement
			out = strcat("#VERSION ", obj.current_version_str, nl);
			if ~optimize
				out = strcat(out, nl);
			end
			
			%Write header statement
			if ~decapitate && obj.header ~= ""
				out = strcat(out, "#HEADER", nl, obj.header, nl, "#HEADER", nl);
				if ~optimize
					out = strcat(out, nl);
				end
			end
			
			%Write flat variables
			for v=obj.varsFlat
				
				%Get string for value
				valstr = v.getValueStr();
				if valstr == "ERROR"
					obj.logErr("Invalid type in flat variable!");
					return;
				end
				
				%Create variable definition
				out = strcat(out, v.getTypeStr(), " ", v.name, " ", valstr, term_char);
				
				%Add description
				if v.desc ~= "" && show_descriptions
					disp(strcat(">", v.desc, "<", string(class(v.desc))));
					out = strcat(out, " ?", v.desc);
				end
				
				%Carriage return
				out = strcat(out, nl);
				
			end
			
			%Sort matrices if required
			if sort_mats || vertical_mode
				obj.sortMatrices();
			end
			
			if ~vertical_mode %------------- Horizontal mode matrices -----
				
				%For each 1D matrix...
				for v = obj.vars1D
					
					%Get string for value
					valstr = v.getValueStr();
					if valstr == "ERROR"
						obj.logErr("Invalid type in flat variable!");
						return;
					end

					%Create variable definition
					out = strcat(out, v.getTypeStr(), " ", v.name, " ", valstr, term_char);

					%Add description
					if v.desc ~= "" && show_descriptions
						disp(strcat(">", v.desc, "<", string(class(v.desc))));
						out = strcat(out, " ?", v.desc);
					end

					%Carriage return
					out = strcat(out, nl);
					
				end
				
				%For each 2D matrix...
				for v = obj.vars2D
					
					%Get string for value
					valstr = v.getValueStr();
					if valstr == "ERROR"
						obj.logErr("Invalid type in flat variable!");
						return;
					end

					%Create variable definition
					out = strcat(out, v.getTypeStr(), " ", v.name, " ", valstr, term_char);

					%Add description
					if v.desc ~= "" && show_descriptions
						disp(strcat(">", v.desc, "<", string(class(v.desc))));
						out = strcat(out, " ?", v.desc);
					end

					%Carriage return
					out = strcat(out, nl);
					
				end
				
				
				
			else %------------------ END horiz, start vert. mode matrix ---
				out = out;
			end %--------------------- END matrix print
			
			fstr = out;
		end %*********************** END swrite() *************************
		
        function readKV1_V2(obj, fileIn, options) %***** readKV1_V2() *****
           
            lnum = 0;
			foundHeader = 0;
			
            %Open file
            fid = fopen(fileIn);
			if fid == -1
				obj.logErr(strcat('Failed to open file "', fileIn, '"'));
				return;
			end

            %Read file line by line
            while(~feof(fid)) %- - - - - - - Loop Through File - - - - - - 
                
                sline = fgetl(fid); %Read line
                lnum = lnum+1; %Increment Line Number

                %Note: char(9) is the tab character
				sline = ensureWhitespace(sline, ';');
                words = parseIdx(sline, strcat(" ", char(9)));
				
				%Skip blank lines
				if isempty(words)
					continue
				end
								
				% Check for each type of file elements
				if words(1).str == "#VERSION"
					
					%Ensure 2 words present
					if length(words) ~= 2
						obj.logErrLn('Version statement accepts exactly 2 words', lnum);
						return;
					end
					
					%Read version statement
					obj.fileVersion = str2double(words(1).str);
					if isnan(obj.fileVersion)
						obj.fileVersion = -1;
						obj.logErrLn('Failed to convert version number to string', lnum);
					end
				elseif words(1).str == "#HEADER"
					
					obj.header = "";
					openedOnLine = lnum;
					
					 while(~feof(fid))
						 sline = fgetl(fid); %Read line
						 lnum = lnum + 1;
						 words = parseIdx(sline, strcat(" ", char(9)));
						 if (~isempty(words)) && (words(1).str == "#HEADER")
							 foundHeader = 1;
							 break;
						 else
							 if obj.header ~= ""
								 obj.header = strcat(obj.header, string(newline), sline);
							 else
								 obj.header = sline;
							 end
						 end
					 end
					
					 if ~foundHeader
						 obj.logErrLn('Failed to find closing #HEADER statement', openedOnLine);
						 return;
					 end
				elseif words(1).str == "//" %TODO: Do I need to change this to check only the first two characters?
					
					%Is a comment
					continue;
				elseif words(1).str == "d" || words(1).str == "b" || words(1).str == "s" || words(1).str == "m<d>" || words(1).str == "m<s>" || words(1).str == "m<b>"
					
					%Check at least 3 words present
					if length(words) < 3
						obj.logErrLn('Insufficient number of tokens for inline variable statement', lnum);
						return;
					end
					
					%Check that name is valid
					if ~obj.isValidName(words(2).str)
						obj.logErrLn(strcat('Invalid variable name "', words(2).str, '"'));
						return;
					end
					
					%Read value
					if words(1).str == "d" || words(1).str == "b"
						if words(1).str == "d"
							newVal = str2double(words(3).str);
						elseif words(1).str == "b"
							newVal = str2logical(words(3).str);
						end
						temp = KvItem(newVal, words(2).str, "");
						if isnan(temp.val)
							obj.logErrLn('Invlaid variable value', lnum);
							return;
						end
						
						allowSemi = true;
						
						%Scan through optional features
						for w=words(4:end)
							cstr = char(w.str);
							if w.str == ";"
								if ~allowSemi
									obj.logErrLn('Duplicate semicolons', lnum);
									return
								end
								allowSemi = false;
							elseif w.str == "?" || cstr(1) == '?'
								temp.desc = sline(w.idx:end); %TODO: This will include inline comments. Go through document at beginning and purge all comments
							elseif w.str == "//"
								break; %Remainder is comment
							end
						end
						
						if isempty(obj.varsFlat)
							obj.varsFlat = temp;
						else
							obj.varsFlat(end+1) = temp;
						end
					elseif words(1).str == "s"
						
						%Create new KvItem
						temp = KvItem("", words(2).str, "");
						
						%Get string contents
						val = getString(sline);
						if isempty(val)
							obj.logErrLn("Failed to find string value", lnum);
						end
						valchar = char(val.str());
						temp.val = string(valchar(2:end-1));
						
						%Scan through optional features
						remainingwords = parseIdx(sline(val.idx+1:end), strcat(" ", char(9)));
						for w=remainingwords
							cstr = char(w.str);
							if w.str == ";"
								if ~allowSemi
									obj.logErrLn('Duplicate semicolons', lnum);
									return
								end
								allowSemi = false;
							elseif w.str == "?" || cstr(1) == '?'
								temp.desc = sline(val.idx+1+w.idx:end); %TODO: This will include inline comments. Go through document at beginning and purge all comments
							elseif w.str == "//"
								break; %Remainder is comment
							end
						end
						
						if isempty(obj.varsFlat)
							obj.varsFlat = temp;
						else
							obj.varsFlat(end+1) = temp;
						end
						
					end
					
% 					disp(strcat(words(1).str, "<"))
% 					disp(class(words(1).str))
				end %-------------------- END check match file element ----
                
            end %- - - - - - - - - - - - END Loop Through File - - - - - - 
		end %************************ END readKV1_V2() ********************
        
        
        
        
        
        
        
        
	end   %************************ END METHODS ***************************
end









