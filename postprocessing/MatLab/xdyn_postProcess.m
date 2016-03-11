function results = xdyn_postProcess(filename, plotResult)
% XDYN_POSTPROCESS postprocesses the result of the simulator XDYN.
%
% 1) Create a states variables that holds all displacements of the bodies
% 2) Generate a figure with all displacements
% 3) Animate wave if present
%
% Inputs:
%   - filename    [Optional] : HDF5 result filename.
%                              Name of the group containg results
%   - plotResult  [Optional] :
%
% Outputs:
%   states : Structure variable that contains all the displacements of the
%            n bodies simulated
%
% Example:
%   - results = xdyn_postProcess('simu.hdf5');
%   - results = xdyn_postProcess({'simu.hdf5', '/outputs'});
%   - results = xdyn_postProcess({'simu.hdf5', '/outputs'}), true);
%
% This function does not rely on any other functions. This single file
% contains all the code necessary to postprocess a HDF5 file
if nargin < 2
    plotResult = true;
    if nargin == 0
        filename = 'hdf5Filename_.h5';
    end
end
if ischar(filename)
    filename = {filename,'/outputs'};
end
tbx_assert(length(filename)==2);
dataGroupName = filename{2};
filename = filename{1};
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
results = struct;
for i=1:numel(dataGroups)
    name = dataGroups(i).Name;
    if strcmp(name,strcat(dataGroupName,'/states'))
        results.states = extractStates(filename, name, [dataGroupName,'/t']);
        if plotResult
            plotStates(results.states);
        end
    elseif strcmp(name, strcat(dataGroupName,'/waves'))
        results.waves = tbx_wave_importWaveElevationFromHdf5(filename, name);
        if plotResult
            animateWaves(results.waves);
        end
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

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function tbx_assert(cond,msg)
% TBX_ASSERT asserts a condition associated to a message.
if nargin == 0
    help(mfilename);
end
if nargin == 1
    % evalin('caller',mfilename)
    msg = [ mfilename ' : Assert failed'];
end

if ~cond
    fprintf(1,'Assertion Failure: %s\n',msg);
    dbstack;
    error([mfilename ':e0'],'Assertion Failure: %s',msg);
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function data = tbx_io_stlRead(file, option)
% TBX_IO_STLREAD reads ascii and binary STL files.
%
% data = tbx_io_stlRead(file);
% data = tbx_io_stlRead(file, option);
%
% % Example of use 1
% figure
% data = tbx_io_stlRead(file);
% % patch('Faces',data.faces,'Vertices',data.vertices,'FaceColor','g');
% patch('Faces',data.faces,'Vertices',data.vertices,'FaceColor','g','EdgeColor','none');
% axis equal
%
% % Example of use 2
% figure
% for i=0:39
%     data = tbx_io_stlRead(sprintf('split%03d.stl',i));
%     patch('Faces',data.faces, 'Vertices',data.vertices,...
%           'FaceColor',rand(1,3), 'EdgeColor','none');
% end
% axis equal
%
% private:
% [f,v,c]=rndread from cad2matdemo Don Riley Jun 2003 : read ASCII STL with color
%   <http://www.mathworks.com/matlabcentral/fileexchange/3642-cad2matdemo-m>
%
% [v,f,n]=import_stl_fast(file, 1) Eric Trautmann 31 Mar 2011 : read ASCII fast, no color
%  <http://www.mathworks.com/matlabcentral/fileexchange/30923-fast-stl-import-function>
%
% stlread(file) Francis Esmonde-White, May 2010 : read STL binary
%  <http://www.mathworks.com/matlabcentral/fileexchange/29906-binary-stl-file-reader>
%  [v, f, n, c, stltitle] = stlread('MyModel.stl');
%
% SIREHNA
% GJ
%==========================================================================
% SVN info
% SVN $Id: tbx_io_stlWrite.m 1093 2013-11-19 08:26:11Z gj $
% SVN $HeadURL: http://130.66.124.6/svn/matlab_toolbox/io/tbx_io_stlWrite.m $
%==========================================================================
data=[];
if isempty(file), return; end
if nargin == 1, option=''; end
if isempty(option)
  % try all importers
  try
    data = tbx_io_stlRead(file, 'ascii');
  end
  if isempty(data),
    data = tbx_io_stlRead(file, 'binary');
  end
  return;
end

v=[]; f=[]; n=[]; c=[]; stltitle='';
% binary format
if strncmp(option, 'bin',3)
  [v, f, n, c, stltitle] = stlread(file);
else  % ascii, text
  % first tries the fast method
  try
    [v,f,n]=import_stl_fast(file, 1);
  end

  % then the slower one
  if isempty(n) || isempty(v)
    [f,v,c]=rndread(file);
  end
end

if isempty(v), return; end

data.faces   = f;
data.vertices= v;
if ~isempty(c)
  data.FaceVertexCData=c;
end
if ~isempty(strtrim(stltitle))
  data.Title=[ file ' ' strtrim(stltitle) ];
end
if ~isempty(n)
  data.Normals=n;
end

% ------------------------------------------------------------------------------
function varargout=import_stl_fast(filename,mode)
% function to import ASCII STL files into matlab.  This update uses the
% textscan function instead of reading the file line by line, which can
% significantly reduce import times for very large STL files.
%--------------------------------------------------------------
%
% inputs:       filename:   string
%               mode:       integer 1 or 2, see outputs:
%
% outputs:      mode 1: [points,triangles,tri norms]
%               mode 2: [vertices, tri norms]
%
% author: Eric Trautmann, 3/31/2011
%         etrautmann@gmail.com
%         based on code by Luigi Giaccari.
%         future revisions will include support for binary files
%-----------------------------------------------------------------
% Ex:
% filename = 'file.stl'
% [p,t,tnorm] = import_stl_fast(filename,1)

%  Copyright (c) 2011, Eric Trautmann
%  All rights reserved.
%
%  Redistribution and use in source and binary forms, with or without
%  modification, are permitted provided that the following conditions are
%  met:
%
%      * Redistributions of source code must retain the above copyright
%        notice, this list of conditions and the following disclaimer.
%      * Redistributions in binary form must reproduce the above copyright
%        notice, this list of conditions and the following disclaimer in
%        the documentation and/or other materials provided with the distribution
%
%  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
%  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
%  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
%  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
%  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
%  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
%  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
%  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
%  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
%  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
%  POSSIBILITY OF SUCH DAMAGE.

if nargin<2
    mode=1;%default value
end

if ~(mode==1 || mode==2)
    error('invalid mode')
end

if nargout<3 && mode==1
    error('invalid input number /mode setting')
end
if nargout>2 && mode==2
    error('invalid input number /mode setting')
end


%=====================================================
%open file
fid=fopen(filename, 'r'); %Open the file, assumes STL ASCII format.
if fid == -1
    error('File could not be opened, check name or path.')
end

fmt = '%*s %*s %f32 %f32 %f32 \r\n %*s %*s \r\n %*s %f32 %f32 %f32 \r\n %*s %f32 %f32 %f32 \r\n %*s %f32 %f32 %f32 \r\n %*s \r\n %*s \r\n';
C=textscan(fid, fmt, 'HeaderLines', 1);
fclose(fid);
if isempty(C), varargout{1}=[]; return; end

%extract normal vectors and vertices
tnorm = cell2mat(C(1:3));
tnorm = tnorm(1:end-1,:); %strip off junk from last line

v1 = cell2mat(C(4:6));
v2 = cell2mat(C(7:9));
v3 = cell2mat(C(10:12));

if (C{4}(end)== NaN)
    v1 = v1(1:end-1,:); %strip off junk from last line
    v2 = v2(1:end-1,:); %strip off junk from last line
    v3 = v3(1:end-1,:); %strip off junk from last line
end

v_temp = [v1 v2 v3]';
v = zeros(3,numel(v_temp)/3);

v(:) = v_temp(:);
v = v';

varargout = cell(1,nargout);
switch mode
    case 1
        [p,t]=fv2pt(v,length(v)/3);%gets points and triangles

        varargout{1} = p;
        varargout{2} = t;
        varargout{3} = tnorm;
    case 2
        varargout{1} = v;
        varargout{2} = tnorm;
end


%%
function [p,t]=fv2pt(v,fnum)

%gets points and triangle indexes given vertex and facet number
c=size(v,1);

%triangles with vertex id data
t=zeros(3,fnum);
t(:)=1:c;

%now we have to keep unique points fro vertex
[p,i,j]=unique(v,'rows'); %now v=p(j) p(i)=v;
t(:)=j(t(:));
t=t';

% ------------------------------------------------------------------------------

function [fout, vout, cout] = rndread(filename)
% Reads CAD STL ASCII files, which most CAD programs can export.
% Used to create Matlab patches of CAD 3D data.
% Returns a vertex list and face list, for Matlab patch command.
%
% filename = 'hook.stl';  % Example file.
%

%  Copyright (c) 2003, Don Riley
%  All rights reserved.
%
%  Redistribution and use in source and binary forms, with or without
%  modification, are permitted provided that the following conditions are
%  met:
%
%      * Redistributions of source code must retain the above copyright
%        notice, this list of conditions and the following disclaimer.
%      * Redistributions in binary form must reproduce the above copyright
%        notice, this list of conditions and the following disclaimer in
%        the documentation and/or other materials provided with the distribution
%
%  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
%  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
%  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
%  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
%  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
%  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
%  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
%  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
%  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
%  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
%  POSSIBILITY OF SUCH DAMAGE.

fid=fopen(filename, 'r'); %Open the file, assumes STL ASCII format.
if fid == -1
    error('File could not be opened, check name or path.')
end
%
% Render files take the form:
%
%solid BLOCK
%  color 1.000 1.000 1.000
%  facet
%      normal 0.000000e+00 0.000000e+00 -1.000000e+00
%      normal 0.000000e+00 0.000000e+00 -1.000000e+00
%      normal 0.000000e+00 0.000000e+00 -1.000000e+00
%    outer loop
%      vertex 5.000000e-01 -5.000000e-01 -5.000000e-01
%      vertex -5.000000e-01 -5.000000e-01 -5.000000e-01
%      vertex -5.000000e-01 5.000000e-01 -5.000000e-01
%    endloop
% endfacet
%
% The first line is object name, then comes multiple facet and vertex lines.
% A color specifier is next, followed by those faces of that color, until
% next color line.
%
CAD_object_name = sscanf(fgetl(fid), '%*s %s');  %CAD object name, if needed.
%                                                %Some STLs have it, some don't.
vnum=0;       %Vertex number counter.
VColor = 0;
%
while feof(fid) == 0                    % test for end of file, if not then do stuff
    tline = fgetl(fid);                 % reads a line of data from file.
    fword = sscanf(tline, '%s ');       % make the line a character string
% Check for color
    if strncmpi(fword, 'c',1) == 1;    % Checking if a "C"olor line, as "C" is 1st char.
       VColor = sscanf(tline, '%*s %f %f %f'); % & if a C, get the RGB color data of the face.
    end                                % Keep this color, until the next color is used.
    if strncmpi(fword, 'v',1) == 1;    % Checking if a "V"ertex line, as "V" is 1st char.
       vnum = vnum + 1;                % If a V we count the # of V's

       v(:,vnum) = sscanf(tline, '%*s %f %f %f'); % & if a V, get the XYZ data of it.
       c(:,vnum) = VColor;              % A color for each vertex, which will color the faces.
    end                                 % we "*s" skip the name "color" and get the data.
end
%   Build face list; The vertices are in order, so just number them.
%
fnum = vnum/3;      %Number of faces, vnum is number of vertices.  STL is triangles.
flist = 1:vnum;     %Face list of vertices, all in order.
F = reshape(flist, 3,fnum); %Make a "3 by fnum" matrix of face list data.
%
%   Return the faces and vertexs.
%
fout = F';  %Orients the array for direct use in patch.
vout = v';  % "
cout = c';
%
fclose(fid);

% ------------------------------------------------------------------------------
function [v, f, n, c, stltitle] = stlread(filename, verbose)
% This function reads an STL file in binary format into vertex and face
% matrices v and f.
%
% USAGE: [v, f, n, c, stltitle] = stlread(filename, verbose);
%
% verbose is an optional logical argument for displaying some loading
%   information (default is false).
%
% v contains the vertices for all triangles [3*n x 3].
% f contains the vertex lists defining each triangle face [n x 3].
% n contains the normals for each triangle face [n x 3].
% c is optional and contains color rgb data in 5 bits [n x 3].
% stltitle contains the title of the specified stl file [1 x 80].
%
% To see plot the 3D surface use:
%   patch('Faces',f,'Vertices',v,'FaceVertexCData',c);
% or
%   plot3(v(:,1),v(:,2),v(:,3),'.');
%
% Duplicate vertices can be removed using:
%   [v, f]=patchslim(v, f);
%
% For more information see:
%  http://www.esmonde-white.com/home/diversions/matlab-program-for-loading-stl-files
%
% Based on code originally written by:
%    Doron Harlev
% and combined with some code by:
%    Eric C. Johnson, 11-Dec-2008
%    Copyright 1999-2008 The MathWorks, Inc.
%
% Re-written and optimized by Francis Esmonde-White, May 2010.

%  Copyright (c) 2011, Francis Esmonde-White
%  Copyright (c) 2011, Eric Johnson
%  All rights reserved.
%
%  Redistribution and use in source and binary forms, with or without
%  modification, are permitted provided that the following conditions are
%  met:
%
%      * Redistributions of source code must retain the above copyright
%        notice, this list of conditions and the following disclaimer.
%      * Redistributions in binary form must reproduce the above copyright
%        notice, this list of conditions and the following disclaimer in
%        the documentation and/or other materials provided with the distribution
%
%  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
%  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
%  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
%  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
%  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
%  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
%  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
%  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
%  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
%  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
%  POSSIBILITY OF SUCH DAMAGE.

use_color=(nargout>=4);

fid=fopen(filename, 'r'); %Open the file, assumes STL Binary format.
if fid == -1
    error('File could not be opened, check name or path.')
end

if ~exist('verbose','var')
    verbose = false;
end

ftitle=fread(fid,80,'uchar=>schar'); % Read file title
numFaces=fread(fid,1,'int32'); % Read number of Faces

T = fread(fid,inf,'uint8=>uint8'); % read the remaining values
fclose(fid);

stltitle = char(ftitle');

if verbose
    fprintf('\nTitle: %s\n', stltitle);
    fprintf('Number of Faces: %d\n', numFaces);
    disp('Please wait...');
end

% Each facet is 50 bytes
%  - Three single precision values specifying the face normal vector
%  - Three single precision values specifying the first vertex (XYZ)
%  - Three single precision values specifying the second vertex (XYZ)
%  - Three single precision values specifying the third vertex (XYZ)
%  - Two color bytes (possibly zeroed)

% 3 dimensions x 4 bytes x 4 vertices = 48 bytes for triangle vertices
% 2 bytes = color (if color is specified)

trilist = 1:48;

ind = reshape(repmat(50*(0:(numFaces-1)),[48,1]),[1,48*numFaces])+repmat(trilist,[1,numFaces]);
Tri = reshape(typecast(T(ind),'single'),[3,4,numFaces]);

n=squeeze(Tri(:,1,:))';
n=double(n);

v=Tri(:,2:4,:);
v = reshape(v,[3,3*numFaces]);
v = double(v)';

f = reshape(1:3*numFaces,[3,numFaces])';

if use_color
    c0 = typecast(T(49:50),'uint16');
    if (bitget(c0(1),16)==1)
        trilist = 49:50;
        ind = reshape(repmat(50*(0:(numFaces-1)),[2,1]),[1,2*numFaces])+repmat(trilist,[1,numFaces]);
        c0 = reshape(typecast(T(ind),'uint16'),[1,numFaces]);
        r=bitshift(bitand(2^16-1, c0),-10);
        g=bitshift(bitand(2^11-1, c0),-5);
        b=bitand(2^6-1, c0);
        c=[r; g; b]';
    else
        c = zeros(numFaces,3);
    end
end

if verbose
    disp('Done!');
end
% ------------------------------------------------------------------------------

