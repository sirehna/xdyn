function varargout = tbx_geom3d_CTM_TO_QUA(c)
% TBX_GEOM3D_CTM_TO_QUA computes quaternion from coordinate transform matrix.
%
% q = tbx_geom3d_CTM_TO_QUA(c)
%
% [q0, q1, q2, q3] = tbx_geom3d_CTM_TO_QUA(c)
%
% The coordinates transform matrix on output is the one that transform
% coordinates in T1 into coordinates in T2 :
% V_T2 = CTM_T2_T1 * V_T1
%
% If the rotation that transform trihedron T1 into Trihedron T2
% is of angle mu around an axis u [ux uy uz],
% then the associated quaternion is defined by :
% q=[q0 q1 q2 q3]
% q0 = cos(mu/2)
% q1 = ux * sin(mu/2)
% q2 = uy * sin(mu/2)
% q3 = uz * sin(mu/2)
%
% SIREHNA
% NC
% 24/03/04
s0=+c(1,1)+c(2,2)+c(3,3);
s1=+c(1,1)-c(2,2)-c(3,3);
s2=-c(1,1)+c(2,2)-c(3,3);
s3=-c(1,1)-c(2,2)+c(3,3);
if(s0 > s1 && s0 > s2 && s0 > s3)
    aux = 0.5/sqrt(1+s0);
    qua = aux*[...
        1+s0;
        c(2,3)-c(3,2);
        c(3,1)-c(1,3);
        c(1,2)-c(2,1)];
elseif( s1 > s2 && s1 > s3 )
    aux = 0.5/sqrt(1+s1); % this is 0.5 / (2*q0)=1/(4*q0)
    qua = aux*[...
        c(2,3)-c(3,2);
        1+s1;
        c(1,2)+c(2,1);
        c(3,1)+c(1,3)];
elseif(s2 > s3)
    aux = 0.5/sqrt(1+s2);
    qua = aux*[
        c(3,1)-c(1,3);
        c(1,2)+c(2,1);
        1+s2;
        c(2,3)+c(3,2)];
else
    aux = 0.5/sqrt(1+s3);
    qua = aux*[
        c(1,2)-c(2,1);
        c(3,1)+c(1,3);
        c(2,3)+c(3,2);
        1+s3];
end
if nargout <= 1
    varargout{1} = qua;
elseif nargout == 4
    varargout{1} = qua(1);
    varargout{2} = qua(2);
    varargout{3} = qua(3);
    varargout{4} = qua(4);
else
    error('Zero, one, or four output arguments are expected.');
end
