classdef StringIdx
	properties
		str
		idx
	end
	methods
		function obj=StringIdx(val, start_idx)
			obj.str = val;
			obj.idx = start_idx;
		end
	end
end