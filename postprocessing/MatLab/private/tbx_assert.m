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
