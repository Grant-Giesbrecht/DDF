function ddfload(varargin)

	% Settings file for ddfload must be saved here, or this string must be
	% modified to reflect the accurate path if changed.
	settings_file = "../Data/ddfload.conf";

	% Read config file
	confs = loadConfig(settings_file, 'ShowMessages', false);
	if islogical(class(confs))
		displ("ERROR: Failed to load configuration file '", settings_file, "'.");
		return;
	end
	
	%Check that required key(s) exist
	if ~mapContains(confs, 'datapath')
		displ("ERROR: Configuration file '", settings_file, "' does not contain key 'datapath'.");
		return;
	end
	
	%Load data
	for shortcut=varargin

		shortcut = string(shortcut);
		datafile_name = "favorites.ddf";
		workspace_abbrev = "fav";

		if strcmp(shortcut, "-sci")
			
			datafile_name = "scientific.ddf";
			workspace_abbrev = "sci";
			
		elseif strcmp(shortcut, "-conv")
			
			datafile_name = "conversions.ddf";
			workspace_abbrev = "conv";
			
		elseif strcmp(shortcut, "-fav")
			
			datafile_name = "favorites.ddf";
			workspace_abbrev = "fav";
			
		end
		
		%Get path
		filepath = fullfile(confs('datapath'), datafile_name);

		%Read data file
		ddf = DDFIO(filepath);
		if ddf.numVar() == 0
			displ("ERROR: Failed to load ", datafile_name, " (", filepath , ").");
			assignin('base', strcat("ddf_", string(workspace_abbrev)), ddf);
			return
		end

		% Assign in workspace
		ddf.assignAll();
		assignin('base', strcat("ddf_", string(workspace_abbrev)), ddf);
	end
	
end