function ddfload(varargin)
%DDFLOAD Loads data from a DDF file saved in the data directory.
%
%	DDFLOAD(FILENAME) Looks for a file with the name FILENAME in the data
%	directory.
%
%	DDFLOAD(FLAGS...) Loads the data specified by the given flag(s) into
%	the workspace.
%
%	 Flag		Meaning
%	----------  --------------------------------
%	    -where	Prints the location of the data directory the configuration
%			    file, and this function.
%	      -sci	Load "scientific.ddf" which contains a selection of commonly
%				used constants in the pysical sciences.
%	     -elec	Load "elec_eng.ddf" which contains a selection of commonly used
%				constants in electrical engineering.
%	      -fav	Load "favorites.ddf". This file is user customizable to include
%				the user's most used variables.
%
%	See also DDFIO.
	
	file_path = mfilename('fullpath'); % Get full path to this file
	trim_idx = find(file_path=='/', 1, 'last'); % Find index for trimming to directory
	local_dir = file_path(1:trim_idx-1);
	
	% ====================== NOTE TO USER =================================
	% Settings file for ddfload must be saved here, or this string must be
	% modified to reflect the accurate path if changed. This path is
	% relative to the location of this file's ("ddfload.m")
	% directory. If you want to specify an absoulte path, you can remove 
	% the call to fullfile and instead specify the absolute path as a
	% string.
	%
	settings_file = fullfile(local_dir, "../ddfload_Data/ddfload.conf");

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

		skip_save = false;
		csc = char(shortcut);
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
			
		elseif strcmp(shortcut, "-elec")
			
			datafile_name = "elec_eng.ddf";
			workspace_abbrev = "elec";
			
		elseif strcmp(shortcut, "-where")
			
			%Prevent saving variables
			skip_save = true;
			
			%Display paths
			displ("    Configuration File Path: '", settings_file, "'");
			displ("    Data Directory: '", confs('datapath') , "'");
			displ("    Path to this function: '", file_path, ".m'");
		
		elseif csc(1) ~= '-' % Interpret as DDF filename
			
			% Parse file
			[filepath, name, ext] = fileparts(shortcut);
			
			% If no extension provided, add .ddf
			% TODO: Allow user to override and force extension free file to
			% be readable.
			if strcmp(ext, "")
				shortcut = strcat(shortcut, ".ddf");
				ext = ".ddf";
			end
			
			% Check that file exists
			if ~isfile(fullfile(confs('datapath'), shortcut))
				displ("ERROR: Failed to find file '", shortcut, "'.");
				continue;
			end
			
			% Specify how to read the file
			datafile_name = strcat(name, ext);
			workspace_abbrev = name;
			
		else 
			
			%Prevent saving variables
			skip_save = true;
			
			displ("ERROR: Flag '", shortcut, "' not recognized.");
			return;
			
		end
		
		if ~skip_save
			
			%Get path to data directory
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
	
end