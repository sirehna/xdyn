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
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function rot = tbx_geom3d_QUA_TO_ROT(q0,q1,q2,q3)
% TBX_GEOM3D_QUA_TO_ROT computes coordinate transform matrix from
% quaternion.
%
% rot = tbx_geom3d_QUA_TO_ROT(q)
% rot = tbx_geom3d_QUA_TO_ROT(q0,q1,q2,q3)
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
% V_T2 = ROT_T2_T1 * V_T1
%
% See also tbx_geom3d_QUA_TO_CTM
%
% SIREHNA
%==========================================================================
% SVN info
% SVN $Id: tbx_geom3d_QUA_TO_ROT.m 772 2012-12-03 19:45:48Z gj $
% SVN $HeadURL: http://130.66.124.6/svn/matlab_toolbox/geom3D/tbx_geom3d_QUA_TO_ROT.m $
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
rot  = [
    q0q0+q1q1-q2q2-q3q3 , 2*(q1q2-q0q3) , 2*(q3q1+q0q2) ;
    2*(q1q2+q0q3) , q0q0-q1q1+q2q2-q3q3 , 2*(q2q3-q0q1) ;
    2*(q3q1-q0q2) , 2*(q2q3+q0q1) , q0q0-q1q1-q2q2+q3q3];
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
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
