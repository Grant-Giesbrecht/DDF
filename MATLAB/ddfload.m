function ddfload(varargin)

	
	local_dir = mfilename('fullpath'); % Get full path to this file
	trim_idx = find(local_dir=='/', 1, 'last'); % Find index for trimming to directory
	local_dir = local_dir(1:trim_idx-1);
	
	% ====================== NOTE TO USER =================================
	% Settings file for ddfload must be saved here, or this string must be
	% modified to reflect the accurate path if changed. This path is
	% relative to the location of this file's ("ddfload.m")
	% directory. If you want to specify an absoulte path, you can remove 
	% the call to fullfile and instead specify the absolute path as a
	% string.
	%
	settings_file = fullfile(local_dir, "../Data/ddfload.conf");

	% Read config file
	confs = loadConfig(settings_file, 'ShowMessages', false);
	if islogical(confs)
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
			
		elseif strcmp(shortcut, "-eng")
			
			datafile_name = "engineering.ddf";
			workspace_abbrev = "eng";
			
		else 
			
			displ("ERROR: Flag '", shortcut, "' not recognized.");
			return;
			
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