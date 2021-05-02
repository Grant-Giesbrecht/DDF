kvbase = KvFile;

% ************************* COMMENTS AND DESCRIPTION TEST *****************

%Add neccesary variables to baseline
kvbase.clear();
kvbase = KvFile("ex_base.kv");
kvbase.add("This is // des//igned to be? trickier", "A", "Tricky!");
kvbase.add(false, "B", "Desc ");
kvbase.get("moons").desc = "Not all moons ";
kvbase.get("baz").desc = "Nons[ense] word ";

%Read in 'ex_comments_desc.kv'
kv_descom = KvFile("ex_comments_desc.kv");

if compareKV(kv_descom, kvbase, 'LooseDesc')
	disp("Passed comments and description test");
else
	disp("Failed comments and descrition test");
end
% compareKV(kv_descom, kvbase)

%********************************* MATRICES TEST **************************

kvbase.clear();
kvbase = KvFile("ex_base.kv");
kvbase.add(["This is a ; test string//", "This string is ont the same row"; "This string is on a new row", "This string is also on the 2nd row //;"], "string_test", "string desc");
kvbase.add([true, false;false, true], "bool_test", "bool desc");

kv_mat = KvFile("ex_matrix.kv");

if compareKV(kv_mat, kvbase, 'LooseDesc')
	disp("Passed matrix test");
else
	disp("Failed matrix test");
end







function eq = compareKV(A, B, varargin)

	desc_mode = 'StrictDesc'; %Require exact match

	if nargin > 2
		if string(varargin{1}) == "LooseDesc" %Allow whitepsace on edges to mismatch
			desc_mode = 'LooseDesc';
		end
		if string(varargin{1}) == "StrictDesc"
			desc_mode = 'StrictDesc';
		end
	end

	for v = A.varsFlat
		if ~compareKVI(v, B.get(v.name), desc_mode)
			disp("Failed to match:")
			disp(v);
			disp("and")
			disp(B.get(v.name))
			eq = false;
			return;
		end
	end
	
	for v = A.vars1D
		if ~compareKVI(v, B.get(v.name), desc_mode)
			eq = false;
			return;
		end
	end
	
	for v = A.vars2D
		if ~compareKVI(v, B.get(v.name), desc_mode)
			eq = false;
			return;
		end
	end
	
	eq = true;
end

