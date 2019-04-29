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
% 3) a rotation phi (roll) around x axis of T1"
%
% See also tbx_geom3d_QUA_TO_CTM, tbx_geom3d_CTM_TO_EUL
%
% SIREHNA
% NC
% 24/03/04
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
