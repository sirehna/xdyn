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