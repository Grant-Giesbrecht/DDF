classdef KvItem < handle
	properties
		type
		dimension
		val
		name
		desc
	end		 % ************* END PROPERTIES *****************
	
	methods  % ************* METHODS ************************
		
		function obj=KvItem(value, name, desc)
			
			%Get value, name, and description
			obj.val = value;
			obj.name = name;
			obj.desc = desc;
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
					obj.dimension = 2;
				end
			end
			
		end % ************* END CONSTRUCTOR ********************
		
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