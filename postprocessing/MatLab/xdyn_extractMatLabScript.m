function xdyn_extractMatLabScript(filename)
% XDYN_EXTRACTMATLABSCRIPT extracts the MatLab script contained in a HDF5 result
% file

if nargin == 0
    filename = 'can_simulate_both_environment_and_system_for_several_time_steps.h5';
end
find_all_matlab_script(filename);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function find_all_matlab_script(filename, basename)
if nargin == 1
    basename = '/';
end
info = h5info(filename, basename);
if ~isempty(info.Datasets)
    for i=1:numel(info.Datasets)
        if isDatasetAMatLabScript(info.Datasets(i))
            script = h5read(filename, [basename, '/', info.Datasets(i).Name]);
            writeToFile(info.Datasets(i).Name,script);
        end
    end
end
if ~isempty(info.Groups)
    for i=1:numel(info.Groups)
        find_all_matlab_script(filename,info.Groups(i).Name)
    end
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function res = isDatasetAMatLabScript(dataset)
res = false;
if strcmp(dataset.Datatype.Class,'H5T_STRING') && strcmp(dataset.Name(end-1:end),'.m')
    res = true;
end
return;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function writeToFile(filename,textToWrite)
if ~iscell(textToWrite)
    error([mfilename ':e0'],'Input has to be a string');
end
fid = fopen(filename,'w+');
for i=1:numel(textToWrite)
    fprintf(fid,'%s',textToWrite{i});
end
fclose(fid);
return;