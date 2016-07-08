function simu = xdyn_postProcess(filename, plotResult)
% XDYN_POSTPROCESS postprocesses the result of the simulator XDYN.
%
% 1) Create a states variables that holds all displacements of the bodies
% 2) Generate a figure with all displacements
% 3) Animate wave if present
%
% Inputs:
%   - filename    [Optional] : HDF5 result filename.
%                              Name of the group containg results
%   - plotResult  [Optional] : Boolean used to plot the results
%
% Outputs:
%   simu : Structure variable that contains all the displacements of the
%          n bodies simulated
%
% Example:
%   - simu = xdyn_postProcess('simu.hdf5');
%   - simu = xdyn_postProcess({'simu.hdf5', '/outputs', '/inputs'});
%   - simu = xdyn_postProcess({'simu.hdf5', '/outputs', '/inputs'}), true);
%
%   - simu = xdyn_postProcess(simu, true);
%
% This function does not rely on any other functions. This single file
% contains all the code necessary to postprocess a HDF5 file.
if nargin < 2
    plotResult = true;
    if nargin == 0
        filename = 'hdf5Filename_.h5';
    end
end
if isstruct(filename)
    simu = filename;
else
    simu = xdyn_loadResultsFromHdf5File(filename);
end

if plotResult && isfield(simu, 'states')
    xdyn_plotStates(simu.states);
end
scale = 1;
if plotResult
    xdyn_animate3d(simu, scale);
end
