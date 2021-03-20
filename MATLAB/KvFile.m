classdef KvFile < handle
	
	properties   %************************ PROPERTIES *********************
		varsFlat
		vars1D
		vars2D
		header
		fileVersion
		error_messages
	end   %********************** END PROPERTIES **************************
	
	methods   %************************ METHODS ***************************
		
		function obj=KvFile()	%*************** INITIALIZER **************
			obj.varsFlat = [];
			obj.vars1D = [];
			obj.vars2D = [];
			obj.header = "";
			obj.fileVersion = -1;
			obj.error_messages = [];
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
				obj.varsFlat
				if length(obj.varsFlat) == 0
					obj.varsFlat = newItem
					obj.varsFlat
				else
					obj.varsFlat(end+1) = newItem;
					obj.varsFlat
				end
			elseif newItem.dimension == 2
				obj.vars1D(end+1) = newItem;
			elseif newItem.dimension == 3
				obj.vars2D(end+1) = newItem;
			else
				obj.logErr(strcat("Dimension exceeded 2D - failed to add '", varName, "'"));
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
				obj.error_messages = msg;
				return;
			end
			obj.error_messages(end+1) = msg;
		end %****************************** END logErr() ******************
		
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
		
        function redaKV1_V2(obj, fileIn, options)
           
            line_num = 0;

            %Open file
            fid = fopen(fileIn);

            %Read file line by line
            while(~feof(fid)) %- - - - - - - Loop Through File - - - - - - 
                
                sline = fgetl(fid); %Read line
                line_num = line_num+1; %Increment Line Number

                %Note: char(9) is the tab character
                words = strsplit(sline, {' ', ']', '[', ';', ',', char(9)}, 'CollapseDelimiters', true); 
                
            end %- - - - - - - - - - - - END Loop Through File - - - - - - 
        end
        
        
        
        
        
        
        
        
	end   %************************ END METHODS ***************************
end









