kvbase = KvFile("ex_base.kv");

%Add neccesary variables to baseline
kvbase.clear();
kvbase = KvFile("ex_base.kv");
kvbase.add("This is // des//igned to be? trickier", "A", "Tricky! //Comment ?Fake description");
kvbase.add(false, "B", "Desc ");
kvbase.get("moons").desc = "Not all moons ";
kvbase.get("baz").desc = "Nons[ense] word ";

kv_descom = KvFile("ex_comments_desc.kv");

compareKV(kv_descom, kvbase)


function eq = compareKV(A, B)
	for v = A.varsFlat
		if ~compareKVI(v, B.get(v.name))
			eq = false;
			return;
		end
	end
	
	for v = A.vars1D
		if ~compareKVI(v, B.get(v.name))
			eq = false;
			return;
		end
	end
	
	for v = A.vars2D
		if ~compareKVI(v, B.get(v.name))
			eq = false;
			return;
		end
	end
	
	eq = true;
end

