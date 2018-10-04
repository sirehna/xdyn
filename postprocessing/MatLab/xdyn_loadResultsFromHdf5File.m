function simu = xdyn_loadResultsFromHdf5File(filename)
% XDYN_LOADRESULTSFROMHDF5FILE loads data and results of a X-Dyn
% simulation.
%
% Input:
%   - filename : HDF5 filename
%
% Output:
%   - simu : Structure containing all data stored in the HDF5 file

if ischar(filename)
    filename = {filename,'/outputs','/inputs'};
end
tbx_assert(length(filename)==3);
outputsGroupName = filename{2};
inputsGroupName = filename{3};
filename = filename{1};
if exist(filename,'file')~=2
    error(['Input file ''' filename ''' does not exist']);
end
outputsGroupName = tbx_string_ensureStringHasHeadingPattern(outputsGroupName, '/');
inputsGroupName = tbx_string_ensureStringHasHeadingPattern(inputsGroupName, '/');

info = h5info(filename);
simu = struct;
simu.info = h5readatt(filename,'/','Description');
listOfGroups = info.Groups;
cmp = strcmp(outputsGroupName, {listOfGroups.Name});
if ~any(cmp)
    disp('Data group name does not exit');
    return;
end
outputs = info.Groups(cmp).Groups;
for i=1:numel(outputs)
    name = outputs(i).Name;
    if strcmp(name, [outputsGroupName,'/states'])
        simu.t = extractTime(filename, [outputsGroupName,'/t']);
        simu.states = extractStates(filename, name, [outputsGroupName,'/t']);
    elseif strcmp(name, [outputsGroupName,'/waves'])
        simu.waves = tbx_wave_importWaveElevationFromHdf5(filename, name);
        simu.t = simu.waves.t;
    elseif strcmp(name, [outputsGroupName,'/spectra'])
        simu.spectra = extractSpectra(filename, name);
    end
end

cmp = strcmp(inputsGroupName, {listOfGroups.Name});
if any(cmp)
    inputs = info.Groups(cmp).Groups;
    simu.meshes=struct;
    for i=1:numel(inputs)
        name = inputs(i).Name;
        m = [inputsGroupName,'/meshes'];
        if strcmp(name, m)
            hdf5Name = inputs(i).Groups.Name;
            meshName = getNameFromHdf5Hierarchy(hdf5Name);
            simu.meshes.(meshName) = struct;
            simu.meshes.(meshName).points = h5read(filename,[hdf5Name '/points']);
            simu.meshes.(meshName).nPoints = size(simu.meshes.(meshName).points,2);
            simu.meshes.(meshName).faces = h5read(filename,[hdf5Name '/faces'])';
        end
    end
    if isempty(fieldnames(simu.meshes))
        simu = rmfield(simu,'meshes');
    end
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function t = extractTime(filename, time)
t = h5read(filename, time);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function states = extractStates(filename, name, time)
info = h5info(filename, name);
outputs = info.Groups;
nObject = numel(outputs);
s = cell(1,nObject);
state = struct('t',s,'x',s,'y',s,'z',s,'quat',s,'eul',s);
states = struct;
for i=1:nObject
    name = getNameFromHdf5Hierarchy(outputs(i).Name);
    states.(name) = state;
    fileInfos = h5info(filename, getGroupFromHdf5Hierarchy(time));
    if ~isempty(fileInfos.Datasets)
        varNames = {fileInfos.Datasets(:).Name};
        if ismember('t',varNames)
            states.(name).t = h5read(filename, time);
        end
    end
    objectInfos = h5info(filename,[outputs(i).Name]);
    varNames = {objectInfos.Datasets(:).Name};
    cc = 'XYZUVWPQR';
    for k = 1:length(cc)
        c = cc(k);
        if ismember(c,varNames)
            states.(name).(lower(c)) = h5read(filename, [outputs(i).Name '/' c]);
        end
    end
    if ~isempty(outputs.Groups)
        states.(name).quat = ...
        [h5read(filename, [outputs(i).Groups(1).Name '/Qr']),...
         h5read(filename, [outputs(i).Groups(1).Name '/Qi']),...
         h5read(filename, [outputs(i).Groups(1).Name '/Qj']),...
         h5read(filename, [outputs(i).Groups(1).Name '/Qk'])];
        states.(name).eul = tbx_geom3d_QUA_TO_EUL(states.(name).quat);
    else
        if ismember('PHI',varNames) && ...
           ismember('THETA',varNames) && ...
           ismember('PSI',varNames)
            states.(name).eul = zeros(length(states.(name).t),3);
            states.(name).eul(:,1) = h5read(filename, [outputs(i).Name '/PHI']);
            states.(name).eul(:,2) = h5read(filename, [outputs(i).Name '/THETA']);
            states.(name).eul(:,3) = h5read(filename, [outputs(i).Name '/PSI']);
        end
    end
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function spectra = extractSpectra(filename, name)
info = h5info(filename, name);
outputs = info.Groups;
nObject = numel(outputs);
spectra = struct;
for i=1:nObject
    name = ['spectr' getNameFromHdf5Hierarchy(outputs(i).Name)];
    spectra.(name) = struct;
    objectInfos = h5info(filename, [outputs(i).Name]);
    varNames = {objectInfos.Datasets(:).Name};
    for k = 1:length(varNames)
        c = varNames{k};
        spectra.(name).(lower(c)) = h5read(filename, [outputs(i).Name '/' c]);
    end
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function name = getNameFromHdf5Hierarchy(m)
names = tbx_string_split(m,'/');
name = names{end};
return
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function name = getGroupFromHdf5Hierarchy(m)
names = tbx_string_split(m,'/');
name = ['/' tbx_string_join(names(1:end-1),'/')];
return

