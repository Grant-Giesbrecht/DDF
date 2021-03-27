classdef KvItem < handle
	properties
		type
		dimension
		val
		name
		desc
		count
		isnotfound %When is a returned value that does not exist, is set true, else false
	end		 % ************* END PROPERTIES *****************
	
	methods  % ************* METHODS ************************
		
		function obj=KvItem(value, name, desc)
			
			%Get value, name, and description
			obj.val = value;
			obj.name = string(name);
			obj.desc = string(desc);
			obj.count = numel(obj.val);
			obj.isnotfound = false;
			if isa(value,'double')
				obj.type = "d";
			elseif isa(value,'string')
				obj.type = "s";
			elseif isa(value,'logical')
				obj.type = "b";
			else
				disp('ERROR: Unrecognized type!');
			end
			
			%Get number of dimensions
			
			%ndims gives 2 dimensions even for numeric types (because
			%everything is a matrix)
			if ndims(value) > 2
				obj.dimension = ndims(value);
			elseif ndims(value) == 2
				
				% To see if it's not a matrix, we have to check if it's a 
				% 1x1 matrix.
				if size(value, 1) == 1 && size(value, 2) == 1
					obj.dimension = 1;
				else
					s = size(value);
					if s(1) == 1
						obj.dimension = 2;
					else
						obj.dimension = 3;
					end
				end
			end
			
		end % ************* END CONSTRUCTOR ********************
		
		function nf=isnf(obj)
			nf=obj.isnotfound;
		end
		
		function valstr=getValueStr(obj)
			
			obj.dimension
			
			if obj.dimension == 2 %If a 1D matrix
				disp("1D");
				%Start with blank string
				valstr = "";
				
				%Add each element
				for e=obj.val
					%Add comma if required
					if valstr ~= ""
						valstr = strcat(valstr, ", ");
					end
					
					valstr = strcat(valstr, obj.getElementStr(e));
				end
				
				valstr = strcat("[", valstr, "]");
				
			elseif obj.dimension == 3 %If a 2D matrix
				
				%Get size
				dim = size(obj.val);
				
				matstr = "";
				
				%For each row
				for r_idx=1:dim(1)
					
					%Get row
					row = obj.val(r_idx,:);
					
					if matstr ~= ""
						matstr = strcat(matstr, "; ");
					end
					
					%Create row string same way create 1D array string
					
					%Start with blank string
					valstr = "";
					
					%Add each element of the row
					for e=row
						%Add comma if required
						if valstr ~= ""
							valstr = strcat(valstr, ", ");
						end

						valstr = strcat(valstr, obj.getElementStr(e));
					end
					
					matstr = strcat(matstr, valstr);
				end
				
				valstr = strcat("[", matstr, "]");
				
			else %Else return element string
				valstr = obj.getElementStr(obj.val);
			end
			
		end
		
		function valstr=getElementStr(obj, e)
			%Get string for value
			if obj.type == "d"
				valstr = num2str(e); %TODO: MOdify format specification
			elseif obj.type == "s"
				valstr = strcat('"', e, '"');
			elseif obj.type == "b"
				valstr = logical2str(e);
			else
				valstr = "ERROR";
			end
		end
		
		function updateCount(obj)
			obj.count = numel(obj.val);
			
			%Get number of dimensions
			
			%ndims gives 2 dimensions even for numeric types (because
			%everything is a matrix)
			if ndims(obj.val) > 2
				obj.dimension = ndims(obj.val);
			elseif ndims(obj.val) == 2
				
				% To see if it's not a matrix, we have to check if it's a 
				% 1x1 matrix.
				if size(obj.val, 1) == 1 && size(obj.val, 2) == 1
					obj.dimension = 1;
				else
					s = size(obj.val);
					if s(1) == 1
						obj.dimension = 2;
					else
						obj.dimension = 3;
					end
				end
			end
		end
		
		function strout=getTypeStr(obj)
			if obj.dimension > 1
				strout = strcat("m<", string(obj.type), ">");
			else
				strout = strcat(obj.type);
			end
		end
		
		function strout=str(obj)
			descstr = strcat(" (", '"', obj.desc, '"', ")")
			strout = strcat("<KvItem(", obj.getTypeStr() , ") ", obj.name , "=", string(obj.val) , descstr, ">");
		end
		
	end		% ************* END METHODS ************************
end