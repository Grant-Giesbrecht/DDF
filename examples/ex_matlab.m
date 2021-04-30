kvbase = KvFile("ex_base.kv");

%Add neccesary variables to baseline
kvbase.clear();
kvbase = KvFile("ex_base.kv");
kvbase.add("This is // des//igned to be? trickier", "A", "Tricky!");
kvbase.add(false, "B", "Desc ");
kvbase.get("moons").desc = "Not all moons ";
kvbase.get("baz").desc = "Nons[ense] word ";

kv_descom = KvFile("ex_comments_desc.kv");

compareKV(kv_descom, kvbase, 'LooseDesc');
% compareKV(kv_descom, kvbase)

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

