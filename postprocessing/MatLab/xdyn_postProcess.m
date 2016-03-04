function states = xdyn_postProcess(filename, dataGroupName, plotResult)
% Postprocess the result of the simulator
%
% 1) Generate a figure with all displacements
% 2) Animate wave if present
% 3) Create a states variables that holds all displacements of the bodies
%
% Inputs:
%   - filename      [Optional] : HDF5 result filename
%   - dataGroupName [Optional] : Name of the group containg results
%
% Outputs:
%   states : Structure variable that contains all the displacements of the
%            n bodies simulated
%
% Example:
%   - states = xdyn_postProcess('simu.hdf5');
%   - states = xdyn_postProcess('simu.hdf5', '/outputs');
%   - states = xdyn_postProcess('simu.hdf5', '/outputs', true);
%
% This function does not rely on any other functions. This single file
% contains all the code necessary to postprocess a HDF5 file
if nargin < 3
    plotResult = true;
    if nargin < 2
        dataGroupName = 'hdf5Group_';
        if nargin == 0
            filename = 'hdf5Filename_.h5';
        end
    end
end

if exist(filename,'file')~=2
    disp('Input file does not exist');
    return
end
dataGroupName = ensureStringHasHeadingPattern(dataGroupName,'/');

info = h5info(filename);
listOfGroups = info.Groups;
cmp = strcmp(dataGroupName, {listOfGroups.Name});
if ~any(cmp)
    disp('Data group name does not exit');
    return;
end
dataGroups = info.Groups(cmp).Groups;
for i=1:numel(dataGroups)
    name = dataGroups(i).Name;
    if strcmp(name,strcat(dataGroupName,'/states'))
        states = extractStates(filename, name, [dataGroupName,'/t']);
        if plotResult
            plotStates(states);
        end
    elseif strcmp(name, strcat(dataGroupName,'/waves')) && plotResult
        animateWaves(filename, name);
    end
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function states = extractStates(filename, name, time)
info = h5info(filename, name);
dataGroups = info.Groups;
nObject = numel(dataGroups);
s = cell(1,nObject);
states = struct('name',s,'t',s,'x',s,'y',s,'z',s,'quat',s,'eul',s);
for i=1:nObject
    states(i).name = dataGroups(i).Name;
    states(i).t = h5read(filename, time);
    states(i).x = h5read(filename, [dataGroups(i).Name '/X']);
    states(i).y = h5read(filename, [dataGroups(i).Name '/Y']);
    states(i).z = h5read(filename, [dataGroups(i).Name '/Z']);
    states(i).quat = ...
    [h5read(filename, [dataGroups(i).Groups(1).Name '/Qr']),...
     h5read(filename, [dataGroups(i).Groups(1).Name '/Qi']),...
     h5read(filename, [dataGroups(i).Groups(1).Name '/Qj']),...
     h5read(filename, [dataGroups(i).Groups(1).Name '/Qk'])];
    states(i).eul = tbx_geom3d_QUA_TO_EUL(states(i).quat);
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function H = plotStates(states)
H = figure;
yLabel = {'X (m)','Y (m)','Z (m)','Phi (deg)','Theta (deg)','Psi (deg)'};
scale = [1 1 1 180/pi 180/pi 180/pi];
bodyColor = '-bgmyk';
nObject = length(states);
XYZEul = cell(1,nObject);
for i=1:nObject
    XYZEul{i} = [states(i).x, states(i).y, states(i).z, states(i).eul];
end
for j=1:6
    subplot(2,3,j);
    box on
    grid on
    hold on
    xlabel('T (s)');
    ylabel(yLabel{j});
    for i=1:nObject
        plot(states(i).t, scale(j)*XYZEul{i}(:,j), bodyColor(i));
    end
end
return
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function animateWaves(filename, group)
wavesElevation = tbx_wave_importWaveElevationFromHdf5(filename,group);
if ~isempty(wavesElevation)
    dt = wavesElevation.t(2)-wavesElevation.t(1);
    waveColormap = 'cool';
    tbx_wave_animate(wavesElevation, dt, waveColormap);
end
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
[X , Y] = ndgrid(x(i,:),y(i,:));
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
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function varargout = tbx_geom3d_QUA_TO_EUL(q0,q1,q2,q3,order,epsilon)
% TBX_GEOM3D_QUA_TO_EUL computes euler angles from quaternion.
%
% phi_theta_psi = tbx_geom3d_QUA_TO_EUL(q0,q1,q2,q3)
% [phi, theta, psi] = tbx_geom3d_QUA_TO_EUL(q0,q1,q2,q3)
%
% phi_theta_psi = tbx_geom3d_QUA_TO_EUL(q0,q1,q2,q3,order)
% [phi, theta, psi] = tbx_geom3d_QUA_TO_EUL(q0,q1,q2,q3,order)
%
% phi_theta_psi = tbx_geom3d_QUA_TO_EUL(q0,q1,q2,q3,order,epsilon)
% [phi, theta, psi] = tbx_geom3d_QUA_TO_EUL(q0,q1,q2,q3,order,epsilon)
%
% phi_theta_psi = tbx_geom3d_QUA_TO_EUL(Q)
% [phi, theta, psi] = tbx_geom3d_QUA_TO_EUL(Q)
%
% phi_theta_psi = tbx_geom3d_QUA_TO_EUL(Q,order)
% [phi, theta, psi] = tbx_geom3d_QUA_TO_EUL(Q,order)
%
% phi_theta_psi = tbx_geom3d_QUA_TO_EUL(Q,order,epsilon)
% [phi, theta, psi] = tbx_geom3d_QUA_TO_EUL(Q,order,epsilon)
%
% Inputs:
%   - q0,q1,q2,q3 : Scalar representing the quaternion
%   - Q           : Vector of 4 elements representing the quaternion
%                   Can also be a matrix with 4 cols, and several lines,
%                   where each line represent a quaternion
%   - order       : [Optional] Rotation composition order.
%                   Either 123,132,213,231,312,321
%                   Default is 321;
%   - epsilon     : [Optional] Precision used to detect singularity when
%                   converting ctm to euler angle.
%                   Default is 1e-13;
%
%
% If the rotation that transform trihedron T1 into Trihedron T2
% is of angle mu around an axis u [ux uy uz],
% then the associated quaternion is defined by :
%
% q  = [q0 q1 q2 q3]
% q0 = cos(mu/2)
% q1 = ux * sin(mu/2)
% q2 = uy * sin(mu/2)
% q3 = uz * sin(mu/2)
%
% The Euler angles are the angles of the three elementary rotations that
% transform T1 trihedron into T2 trihedron.
% For order 321, it corresponds to :
% 1) a rotation psi (yaw) around z axis of T1
% 2) a rotation theta (pitch) around y axis of T1'
% 3) a rotation phi (roll) around x axis of T1''
%
% See also tbx_geom3d_QUA_TO_CTM, tbx_geom3d_CTM_TO_EUL
%
% SIREHNA
% NC
% 24/03/04
%==========================================================================
% SVN info
% SVN $Id: tbx_geom3d_QUA_TO_EUL.m 937 2013-04-30 09:56:50Z gj $
% SVN $HeadURL: http://130.66.124.6/svn/matlab_toolbox/geom3D/tbx_geom3d_QUA_TO_EUL.m $
%==========================================================================
if nargin == 1
    qua = q0;
    order = 321;
    epsilon = 1e-13;
elseif nargin == 2
    qua = q0;
    order = q1;
    epsilon = 1e-13;
elseif nargin == 3
    qua = q0;
    order = q1;
    epsilon = q2;
elseif nargin == 4
    qua = [q0 q1 q2 q3];
    order = 321;
    epsilon = 1e-13;
elseif nargin == 5
    qua = [q0 q1 q2 q3];
    epsilon = 1e-13;
elseif nargin == 6
    qua = [q0 q1 q2 q3];
elseif nargin > 6
    error('there should be no more than 6 input arguments');
end
n=numel(qua);
if n == 4
    ctm = tbx_geom3d_QUA_TO_CTM(qua);
    eul = tbx_geom3d_CTM_TO_EUL(ctm,order,epsilon);
elseif mod(n,4)==0
    n=n/4;
    eul=zeros(n,3);
    for i=1:n
        ctm = tbx_geom3d_QUA_TO_CTM(qua(i,:));
        eul(i,:) = tbx_geom3d_CTM_TO_EUL(ctm,order,epsilon);
    end
else
    error('Input should have 4 elements, or a multiple of 4 elements');
end
if nargout<=1
    varargout{1} = eul;
elseif nargout == 3
    varargout{1} = eul(1);
    varargout{2} = eul(2);
    varargout{3} = eul(3);
else
    error('this function can have 1 or 3 outputs');
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function ctm = tbx_geom3d_QUA_TO_CTM(q0,q1,q2,q3)
% TBX_GEOM3D_QUA_TO_CTM computes coordinate transform matrix from
% quaternion.
%
% ctm = tbx_geom3d_QUA_TO_CTM(q)
% ctm = tbx_geom3d_QUA_TO_CTM(q0,q1,q2,q3)
%
% If the rotation that transform trihedron T1 into Trihedron T2
% is of angle mu around an axis u [ux uy uz],
% then the associated quaternion is defined by :
% q=[q0 q1 q2 q3]
% q0=cos(mu/2)
% q1=ux * sin(mu/2)
% q2=uy * sin(mu/2)
% q3=uz * sin(mu/2)
%
% The coordinates transform matrix on output is the one that transform
% coordinates in T1 into coordinates in T2 :
% V_T2 = CTM_T2_T1 * V_T1
%
% See also tbx_geom3d_QUA_TO_ROT
%
% SIREHNA
% NC
% 24/03/04
%==========================================================================
% SVN info
% SVN $Id: tbx_geom3d_QUA_TO_CTM.m 772 2012-12-03 19:45:48Z gj $
% SVN $HeadURL: http://130.66.124.6/svn/matlab_toolbox/geom3D/tbx_geom3d_QUA_TO_CTM.m $
%==========================================================================
if(nargin == 1)
    q = q0;
    n = numel(q);
    if(n ~= 4)
        error('the argument should be a vector of length 4 : [q0 q1 q2 q3]');
    end
elseif (nargin == 4)
    q = [q0 q1 q2 q3];
    n = numel(q);
    if(n ~= 4)
        error('the 4 arguments should be scalar');
    end
else
    error('there should be 1 vector [q0 q1 q2 q3] or 4 scalar arguments');
end
q0q0 = q(1)*q(1);
q1q1 = q(2)*q(2);
q2q2 = q(3)*q(3);
q3q3 = q(4)*q(4);
q1q2 = q(2)*q(3);
q2q3 = q(3)*q(4);
q3q1 = q(4)*q(2);
q0q1 = q(1)*q(2);
q0q2 = q(1)*q(3);
q0q3 = q(1)*q(4);
ctm=[
    q0q0+q1q1-q2q2-q3q3 , 2*(q1q2+q0q3) , 2*(q3q1-q0q2) ;
    2*(q1q2-q0q3) , q0q0-q1q1+q2q2-q3q3 , 2*(q2q3+q0q1) ;
    2*(q3q1+q0q2) , 2*(q2q3-q0q1) , q0q0-q1q1-q2q2+q3q3 ];
function varargout = tbx_geom3d_CTM_TO_EUL(c,order,epsilon)
% TBX_GEOM3D_CTM_TO_EUL computes euler angles from coordinate transform
% matrix.
%
% phi_theta_psi = tbx_geom3d_CTM_TO_EUL(c)
%
% [phi, theta, psi] = tbx_geom3d_CTM_TO_EUL(c)
% [phi, theta, psi] = tbx_geom3d_CTM_TO_EUL(c,order)
% [phi, theta, psi] = tbx_geom3d_CTM_TO_EUL(c,order,epsilon)
%
% Inputs:
%   - c       : 3x3 CTM
%   - order   : [Optional] Define the order in which the rotation are
%               applied.
%               order is defined on three digits
%               Default is 321, corresponding to the standard Euler
%               rotation defined below.
%   - epsilon : [Optional] Absolute epsilon used to detect zero value for
%               singularities. Default value is 1e-13
%
% Output:
%   - Euler angle
%
% The coordinate transform matrix transform coordinates in T1 trihedron
% into coordinates in T2 trihedron :
% V_T2 = CTM_T2_T1 * V_T1
%
% The Euler angles are the angles of the three elementary rotations that
% transform T1 trihedron into T2 trihedron :
% For order 321, the composition is as:
% 1) a rotation psi (yaw) around z axis of T1
% 2) a rotation theta (pitch) around y axis of T1'
% 3) a rotation phi (roll) around x axis of T1''
%
% SIREHNA
% NC
% 24/03/04
%==========================================================================
% SVN info
% SVN $Id: tbx_geom3d_CTM_TO_EUL.m 1094 2013-11-19 14:27:21Z gj $
% SVN $HeadURL: http://130.66.124.6/svn/matlab_toolbox/geom3D/tbx_geom3d_CTM_TO_EUL.m $
%==========================================================================

if nargin < 3
    epsilon = 1e-13;
    if nargin < 2
        order = 321;
    end
end
if order == 321
    aux = hypot(c(1,1),c(1,2)); % cos(theta)
    theta = atan2(-c(1,3),aux);
    if(aux > epsilon)
        phi = atan2(c(2,3),c(3,3));
        psi = atan2(c(1,2),c(1,1));
    else
        phi = 0;
        psi = atan2(-c(2,1),c(2,2));
    end
elseif order == 123
    aux = hypot(c(1,1),c(2,1)); % cos(theta)
    theta = atan2(c(3,1),aux);
    if(aux > epsilon)
        phi = atan2(-c(3,2),c(3,3));
        psi = atan2(-c(2,1),c(1,1));
    else
        phi = 0;
        psi = atan2(c(1,2),c(2,2));
    end
elseif order == 213
    aux = hypot(c(3,1),c(3,3)); % cos(phi)
    phi = atan2(-c(3,2),aux);
    if(aux > epsilon)
        theta = atan2(c(3,1),c(3,3));
        psi = atan2(c(1,2),c(2,2));
    else
        theta = 0;
        psi = atan2(c(2,1),c(1,1));
    end
elseif order == 312
    aux = hypot(c(2,1),c(2,2)); % cos(phi)
    phi = atan2(c(2,3),aux);
    if(aux > epsilon)
        theta = atan2(-c(1,3),c(3,3));
        psi = atan2(-c(2,1),c(2,2));
    else
        theta = 0;
        psi = atan2(c(1,2),c(1,1));
    end
elseif order == 132
    aux = hypot(c(2,2),c(2,3)); % cos(psi)
    psi = atan2(-c(2,1),aux);
    if(aux > epsilon)
        phi = atan2(c(2,3),c(2,2));
        theta = atan2(c(3,1),c(1,1));
    else
        phi = 0;
        theta = atan2(-c(1,3),c(1,2));
    end
elseif order == 231
    aux = hypot(c(1,1),c(1,3)); % cos(psi)
    psi = atan2(c(1,2),aux);
    if(aux > epsilon)
        phi = atan2(-c(3,2),c(2,2));
        theta = atan2(-c(1,3),c(1,1));
    else
        phi = 0;
        theta = atan2(c(2,3),-c(2,1));
    end
else
    error([mfilename ':e0'],...
           'Unknown order');
end

if (nargout<=1)
    varargout{1} = [phi theta psi]';
elseif(nargout == 3)
    varargout{1} = phi;
    varargout{2} = theta;
    varargout{3} = psi;
else
    error('this function can have 1 or 3 outputs');
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function str = tbx_string_ensureStringHasHeadingPattern(str,headingPattern)
% TBX_STRING_ENSURESTRINGHasHeadingPattern checks if string has a heading
% pattern.
%
% This function is usefull to add path to a filename for example.
%
% str = tbx_string_ensureStringHasHeadingPattern(str,headingPattern)
%
% Inputs :
%   - str             : String or cell array of strings
%   - headingPattern  : String containing the trailing pattern
%
% Outputs :
%   - str             : Modified string or cell array
%
% See also tbx_string_ensureStringHasHeadingPattern
%
% Compliant with MatLab 6
%
% SIREHNA
% GJ
%==========================================================================
% SVN info
% SVN $Id: tbx_string_ensureStringHasHeadingPattern.m 1104 2013-12-06 10:49:06Z gj $
% SVN $HeadURL: http://130.66.124.6/svn/matlab_toolbox/string/tbx_string_ensureStringHasHeadingPattern.m $
%==========================================================================
if ischar(str)
    str = ensureStringHasHeadingPattern(str,headingPattern);
elseif iscell(str)
    for i = 1:numel(str)
        str{i} = ensureStringHasHeadingPattern(str{i},headingPattern);
    end
end
return;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function str = ensureStringHasHeadingPattern(str,headingPattern)
if ~strncmp(str,headingPattern,length(headingPattern));
    str = [headingPattern str];
end
return;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function str = tbx_string_ensureStringHasTrailingPattern(str,trailingPattern)
% TBX_STRING_ENSURESTRINGHASTRAILINGPATTERN checks if string has a trailing
% pattern.
%
% This function usefull to add extension to a filename for example
%
% str = tbx_string_ensureStringHasTrailingPattern(str,trailingPattern)
%
% Inputs :
%   - str             : String or cell array of strings
%   - trailingPattern : String containing the trailing pattern
%
% Outputs :
%   - str             : Modified string or cell array
%
% See also  tbx_string_ensureStringHasHeadingPattern
%
% Compliant with MatLab 6
%
% SIREHNA
% GJ
%==========================================================================
% SVN info
% SVN $Id: tbx_string_ensureStringHasTrailingPattern.m 585 2011-11-28 08:16:23Z gj $
% SVN $HeadURL: http://130.66.124.6/svn/matlab_toolbox/string/tbx_string_ensureStringHasTrailingPattern.m $
%==========================================================================
if ischar(str)
    str = ensureStringHasTrailingPattern(str,trailingPattern);
elseif iscell(str)
    for i= 1:numel(str)
        str{i} = ensureStringHasTrailingPattern(str{i},trailingPattern);
    end
end
return;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function str = ensureStringHasTrailingPattern(str,trailingPattern)
if ~strncmp(fliplr(str),fliplr(trailingPattern),length(trailingPattern));
    str = [str trailingPattern];
end
return;
