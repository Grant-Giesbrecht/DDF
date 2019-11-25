function file_version=load_kv(filename)

    header = "";
    version = -1;

    line_num = 0;

    %Open file
    fid = fopen(filename);

    %Read file line by line
    while(~feof(fid))
        sline = fgetl(fid);
        line_num = line_num+1;

        words = strsplit(sline, {' ', ']', '[', ';', ','}, 'CollapseDelimiters', true);

        %Skip empty lines
        if (isempty(sline))
            continue;
        end

        %Convert to character array
        cline = char(sline);

        %Scan through line
        if (cline(1) == '#') %Read header
            if (strcmp(cline(2:end),'HEADER') == 1 )
                while(~feof(fid))
                    sline = fgetl(fid);
                    cline = char(sline);
                    line_num = line_num+1;
                    if (strcmp(cline(1:end), '#HEADER') == 1)
                        break;
                    end
                    header = strcat(header, newline, sline);
                end
                continue;

            elseif (strcmp(cline(2:8), 'VERSION') == 1)
                version = str2double(cline(9:end));
            else
                display(['Failed on line ', num2str(line_num), ' because of an unrecognized macro.']);
            end

        elseif (cline(1) == '/')
            continue;
        elseif (cline(1) == ' ')
            continue;
        else
            if (cline(1) == 's')
                assignin('base', string(words(2)), strcat(words(3:end)));
            elseif(cline(1) == 'd')
                assignin('base', string(words(2)), str2double(words(3)));
            elseif(cline(1) == 'b')
                assignin('base', string(words(2)), logical(lower((words(3)))));
            elseif(cline(1) == 'm')
                if (cline(3) == 'd')
                    assignin('base', string(words(2)), str2double(words(3:end)));
                elseif(cline(3) == 's')
                    assignin('base', string(words(2)), words(3:end));
                elseif(cline(3) == 'b')
                    assignin('base', string(words(2)), logical(lower(words(3:end))));
                else
                    disp(['Failed on line ', num2str(line_num)]);
                end
            else
                disp(['Failed on line ', num2str(line_num)]);
                return
            end
        end

        cfn = char(filename);
        k = find(cfn == '.');
		j = find(cfn == '/', 1, 'last');
		if (isempty(j))
			j=1;
		else
			j=j+1;
		end
        fn_start = cfn(j:k(1)-1);
        assignin('base', [fn_start, '_header'], (header));

    end

    file_version=version;
    
end



