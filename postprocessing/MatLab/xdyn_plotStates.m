function xdyn_plotStates(states)
% XDYN_PLOTSTATES plots the states of the different bodies stored in
% states
%
% Input :
%   - states : Structure containing time, positions and velocities
%
% Example :
%
%    simu = xdyn_loadResultsFromHdf5File('simu.hdf5');
%    xdyn_plotStates(simu.states);
%
% See also xdyn_plotPositions, xdyn_plotVelocities
xdyn_plotPositions(states);
xdyn_plotVelocities(states);
