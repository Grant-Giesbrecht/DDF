function eq = compareKVI(A, B)

	if A.type ~= B.type
		eq = false;
		return;
	elseif A.dimension ~= B.dimension
		eq = false;
		return;
	elseif A.val ~= B.val
		eq = false;
		return;
	elseif A.name ~= B.name
		eq = false;
		return;
	elseif A.desc ~= B.desc
		eq = false;
		return;
	elseif A.count ~= B.count
		eq = false;
		return;
	elseif A.isnotfound ~= B.isnotfound
		eq = false;
		return;
	end
	
	eq = true;

end