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
% 3) a rotation phi (roll) around x axis of T1"
%
% SIREHNA
% NC
% 24/03/04

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
