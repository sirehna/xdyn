function wavesElevation = tbx_wave_importWaveElevationFromHdf5(hdf5File, hdf5Group, castToDouble)
% TBX_WAVE_IMPORTWAVEELEVATIONFROMHDF5 imports wave elevations from a HDF5 file
%
% Inputs :
%       - hdf5File       : Name of the hdf5 file.
%       - hdf5Group      : Name of the group where the different variables
%                          will be exported.
%       - castToDouble   : Option used to cast outputs to double, in case
%                          inputs are single. 
%
% Output:
%       - wavesElevation : Structure containing fields
%                              - t   : Time vector
%                              - x   : X space vector
%                              - y   : Y space vector
%                              - eta : Array with 2 or 3 dimension
%                                      representing wave elevation
%
% Examples:
%   - wavesElevation = tbx_wave_importWaveElevationFromHdf5(hdf5File)
%   - wavesElevation = tbx_wave_importWaveElevationFromHdf5(hdf5File, hdf5Group)
%   - wavesElevation = tbx_wave_importWaveElevationFromHdf5(hdf5File, hdf5Group, castToDouble)
%
% See also tbx_wave_exportWaveElevationToHdf5, tbx_wave_animate
%
%==========================================================================
% SVN info
% SVN $Id: tbx_wave_importWaveElevationFromHdf5.m 1384 2015-07-20 08:08:25Z gj $
% SVN $HeadURL: http://130.66.124.6/svn/matlab_toolbox/wave/tbx_wave_importWaveElevationFromHdf5.m $
%==========================================================================
if nargin < 3
    castToDouble = false;
    if nargin < 2
        hdf5Group = '/WaveElevation';
        if nargin < 1
            hdf5File = 'WaveElevation.h5';
        end
    end
end
hdf5Group = tbx_string_ensureStringHasHeadingPattern(hdf5Group,'/');
hdf5Group = tbx_string_ensureStringHasTrailingPattern(hdf5Group,'/');
info = h5info(hdf5File,hdf5Group);
datasets = {info.Datasets(:).Name};
expectedDataset = {'t','x','y','z'};
for i =1:numel(expectedDataset)
    if ~isInDataset(expectedDataset{i},datasets)
        error([expectedDataset{i} ' dataset in not present in file']);
    end
end

t = h5read(hdf5File,[hdf5Group 't']);
x = h5read(hdf5File,[hdf5Group 'x'])';
y = h5read(hdf5File,[hdf5Group 'y'])';
Z = permute(h5read(hdf5File,[hdf5Group 'z']),[3 2 1]);

wavesElevation.t = t;
wavesElevation.x = x;
wavesElevation.y = y;
wavesElevation.eta = Z;

if isInDataset('phis',datasets)
    phis = permute(h5read(hdf5File,[hdf5Group 'phis']),[3 2 1]);
    wavesElevation.phis = phis;
end

if castToDouble
    fS = fieldnames(wavesElevation);
    for i=1:numel(fS)
        wavesElevation.(fS{i}) = double(wavesElevation.(fS{i}));
    end
end

function r = isInDataset(datasetName,datasets)
r = any(strcmp(datasetName,datasets));