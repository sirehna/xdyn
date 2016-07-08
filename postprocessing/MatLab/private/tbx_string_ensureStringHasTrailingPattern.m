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