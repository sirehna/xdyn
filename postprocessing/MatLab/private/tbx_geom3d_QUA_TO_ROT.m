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

