function xdyn_animate3d(simu, scale)
% XDYN_ANIMATE3D creates a animation from a simulation
%
% Inputs:
%   - simu  : Structure containing results from a simulation
%   - scale : [Optional] Time scale. This parameter is used to speed up, or
%             slow down animation.
%             Default is 1.
%
% See also xdyn_loadResultsFromHdf5File

if nargin < 2
    scale = 1;
end
H = figure;
set(H,'WindowKeyPressFcn',@funPauseCallback);
set(H,'UserData',false);
dt = scale * (simu.t(2) - simu.t(1));
ti = title('X-DYN');
set(gca,'zdir','reverse');
hold on
axis equal
view(3);
hasMeshes = isfield(simu,'meshes') && ~isempty(fieldnames(simu.meshes));
if isfield(simu, 'states') && hasMeshes
    facecolor = 'g';
    edgecolor = 'k'; % 'None'
    hMeshes = tbx_meshes_create(simu, facecolor, edgecolor);
    if isfield(simu, 'waves')
        waveColormap = 'cool';
        hs = tbx_wave_create(simu.waves, waveColormap, H);
    end
    objs = fieldnames(simu.states);
    nObj = numel(objs);
    for i = 1:numel(simu.t)
        t = simu.t(i);
        for j = 1:nObj
            obj = objs{j};
            s = simu.states.(obj);
            tran = [s.x(i);s.y(i);s.z(i)];
            rot = tbx_geom3d_QUA_TO_ROT(s.quat(i,:));
            nVert = (rot * simu.meshes.(obj).points + repmat(tran,1,simu.meshes.(obj).nPoints))';
            set(hMeshes(j),'Vertices', nVert);
        end
        if isfield(simu,'waves')
            hs = tbx_wave_update(hs,simu.waves, i);
        end
        set(ti,'String',['T = ' num2str(t)]);
        funPause(H,dt);
    end
elseif isfield(simu,'waves')
    animateWaves(simu.waves);
else
    delete(H);
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function funPause(H,dt)
ud = get(H,'UserData');
if ~ud
    pause(dt);
else
    pause;
end
drawnow;
figure(H);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function funPauseCallback(H,~)
ud = get(H,'UserData');
if isempty(ud)
    set(H,'UserData',false);
end
ud = ~get(H,'UserData');
set(H,'UserData',ud);
figure(H);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function hMeshes = tbx_meshes_create(simu, facecolor, edgecolor)
if nargin < 3
    edgecolor = 'k';
    if nargin < 2
        facecolor = 'g';
    end
end
meshNames = fieldnames(simu.meshes);
nMeshes = numel(meshNames);
hMeshes = zeros(1, nMeshes);
for i = 1:nMeshes
    meshName = meshNames{i};
    hMeshes(i) = ...
        patch('Faces',simu.meshes.(meshName).faces,...
              'Vertices',simu.meshes.(meshName).points',...
              'FaceColor',facecolor,'EdgeColor',edgecolor);
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function wavesElevation = animateWaves(filename, group)
if ischar(filename)
    wavesElevation = tbx_wave_importWaveElevationFromHdf5(filename,group);
elseif isstruct(filename)
    wavesElevation = filename;
end
if ~isempty(wavesElevation)
    dt = wavesElevation.t(2)-wavesElevation.t(1);
    waveColormap = 'cool';
    tbx_wave_animate(wavesElevation, dt, waveColormap);
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function hs = tbx_wave_create(wavesElevation, waveColormap, H)
if nargin < 3
    H = [];
    if nargin < 2
        waveColormap = 'cool';
        if nargin < 1
            x = 0:1:100;
            y = 0:1:200;
            t = 0:0.1:20;
            wavesR = tbx_wave_freeWaveGenerationDemo(1,true);
            wavesElevation = tbx_wave_elevation(x,y,t,wavesR);
        end
    end
end

t = wavesElevation.t;
x = wavesElevation.x;
y = wavesElevation.y;
Z = wavesElevation.eta;

n = numel(t);
if size(x,1)==1 && n>1
    x = repmat(x,n,1);
end
if size(y,1)==1 && n>1
    y = repmat(y,n,1);
end

i = 1;
[X, Y] = ndgrid(x(i,:),y(i,:));
if isempty(H)
    H = figure;
    view(3);
end
hs = surf(X,Y,Z(:,:,i));
colormap(waveColormap);
set(hs,'EdgeColor','None');
return;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function hs = tbx_wave_update(hs,wavesElevation, i)
x = wavesElevation.x;
y = wavesElevation.y;
Z = wavesElevation.eta;
[X,Y] = ndgrid(x(i,:),y(i,:));
set(hs,'XData',X,'YData',Y,'ZData',Z(:,:,i));

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function tbx_wave_animate(wavesElevation, dt, waveColormap)
% TBX_WAVE_ANIMATE animates wave elevation on a grid.
%
% waves_elevation = tbx_wave_elevation(wavesElevation)
% waves_elevation = tbx_wave_elevation(wavesElevation, dt)
% waves_elevation = tbx_wave_elevation(wavesElevation, dt, waveColormap)
%
%==========================================================================
% SVN info
% SVN $Id: tbx_wave_animate.m 1322 2014-11-21 10:41:51Z gj $
% SVN $HeadURL: http://130.66.124.6/svn/matlab_toolbox/wave/tbx_wave_animate.m $
%==========================================================================
if nargin < 3
    waveColormap = 'cool';
    if nargin < 2
        dt = 0.1;
        if nargin < 1
            x = 0:1:100;
            y = 0:1:200;
            t = 0:0.1:20;
            wavesR = tbx_wave_freeWaveGenerationDemo(1,true);
            wavesElevation = tbx_wave_elevation(x,y,t,wavesR);
        end
    end
end

t = wavesElevation.t;
x = wavesElevation.x;
y = wavesElevation.y;
Z = wavesElevation.eta;

n = numel(t);
if size(x,1)==1 && n>1
    x = repmat(x,n,1);
end
if size(y,1)==1 && n>1
    y = repmat(y,n,1);
end

i = 1;
[X, Y] = ndgrid(x(i,:),y(i,:));
figure
view(3)
hs = surf(X,Y,Z(:,:,i));
colormap(waveColormap);
set(hs,'EdgeColor','None');
ht = title(['T = ' num2str(t(i))]);
pause(dt);

for i = 2:n
    [X,Y] = ndgrid(x(i,:),y(i,:));
    set(hs,'XData',X,'YData',Y,'ZData',Z(:,:,i));
    set(ht,'String',['T = ' num2str(t(i))]);
    pause(dt);
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function wavesElevation = tbx_wave_importWaveElevationFromHdf5(hdf5File, hdf5Group)
% TBX_WAVE_IMPORTWAVEELEVATIONFROMHDF5 imports wave elevations from a HDF5 file
%
% Inputs :
%       - hdf5File       : Name of the hdf5 file.
%       - hdf5Group      : Name of the group where the different variables
%                          will be exported.
%
% Output:
%       - wavesElevation : Structure containing fields
%                              - t   : Time vector
%                              - x   : X space vector
%                              - y   : Y space vector
%                              - eta : Array with 2 or 3 dimension
%                                      representing wave elevation
%
% See also tbx_wave_exportWaveElevationToHdf5, tbx_wave_animate
%
%==========================================================================
% SVN info
% SVN $Id: tbx_wave_importWaveElevationFromHdf5.m 1327 2015-01-12 16:39:24Z gj $
% SVN $HeadURL: http://130.66.124.6/svn/matlab_toolbox/wave/tbx_wave_importWaveElevationFromHdf5.m $
%==========================================================================
if nargin < 2
    hdf5Group = '/WaveElevation';
    if nargin < 1
        hdf5File = 'WaveElevation.h5';
    end
end
hdf5Group = tbx_string_ensureStringHasHeadingPattern(hdf5Group,'/');
hdf5Group = tbx_string_ensureStringHasTrailingPattern(hdf5Group,'/');
t = h5read(hdf5File,[hdf5Group 't']);
x = h5read(hdf5File,[hdf5Group 'x'])';
y = h5read(hdf5File,[hdf5Group 'y'])';
Z = permute(h5read(hdf5File,[hdf5Group 'z']),[3 2 1]);

wavesElevation.t = t;
wavesElevation.x = x;
wavesElevation.y = y;
wavesElevation.eta = Z;
