function str = tbx_string_split(str,delimiters)
% TBX_STRING_SPLIT splits a string or a cell array of strings.
%
% Developped specially for Matlab 6.
% Use the following command for MatLab 7.
%   textscan(str,'%s','delimiter',delimiters);
%
% str = tbx_string_split(str)
% str = tbx_string_split(str,delimiters)
%
% Inputs:
%  - str         : String or cell array of strings for which one wants
%                  to white spaces.
%  - delimiters  : [Optional] A set of characters that are used to split
%                  the string. Default is ',;'
%
% Outputs:
%  - str         : String or cell array of strings for which one wants
%                  to white spaces.
%
% The function is based on strtok function
%
% Compliant with MatLab 6
%
% SIREHNA
% GJ
%==========================================================================
% SVN info
% SVN $Id: tbx_string_split.m 940 2013-05-06 16:24:36Z gj $
% SVN $HeadURL: http://130.66.124.6/svn/matlab_toolbox/string/tbx_string_split.m $
%==========================================================================
if nargin == 1
    delimiters = ',;';
end
if ischar(str)
    str = string_split(str,delimiters);
elseif iscell(str)
    for i= 1:numel(str)
        str{i} = string_split(str{i},delimiters);
    end
end
return;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function strCells = string_split(str,delimiters)
strCells={};
while ~isempty(str)
    [strCells{end+1},str] = strtok(str,delimiters);
end
return;
