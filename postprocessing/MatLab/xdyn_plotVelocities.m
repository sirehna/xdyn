function H = xdyn_plotVelocities(states)
% XDYN_PLOTVELOCITIES creates a figure that displays the evolution of
% velocities expressed in bodies frames.
%
% Input:
%   - states : Structure containing time and velocities information
%
% Output:
%   - H : Handle to the created figure
%
% Example :
%
%    simu = xdyn_loadResultsFromHdf5File('simu.hdf5');
%    xdyn_plotVelocities(simu.states);
%
% See also xdyn_loadResultsFromHdf5File
H = figure;
yLabel = {'U (m/s)','V (m/s)','W (m/s)','P (deg/s)','Q (deg/s)','R (deg/s)'};
scale = [1 1 1 180/pi 180/pi 180/pi];
bodyColor = '-bgmyk';
names = fieldnames(states);
nObject = numel(fieldnames(states));
UVWPQR = cell(1,nObject);
for i=1:nObject
    n = names{i};
    UVWPQR{i} = [states.(n).u,...
                 states.(n).v,...
                 states.(n).w,...
                 states.(n).p,...
                 states.(n).q,...
                 states.(n).r];
end
if nObject==1
    bodyColor = 'k';
end
if isempty(states.(n).eul)
    nRow = 1;
    nCol = 3;
else
    nRow = 2;
    nCol = 6;
end
for j=1:nCol
    subplot(nRow,3,j);
    box on
    grid on
    hold on
    xlabel('T (s)');
    ylabel(yLabel{j});
    for i=1:nObject
        n = names{i};
        plot(states.(n).t, scale(j)*UVWPQR{i}(:,j), bodyColor(i));
    end
end
return
