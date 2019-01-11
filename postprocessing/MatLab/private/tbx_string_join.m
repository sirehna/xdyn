function str = tbx_string_join(str, delimiter)
% TBX_STRING_JOIN joins a string or a cell array of strings.
%
% str = tbx_string_join(str)
% str = tbx_string_join(str,delimiter)
%
% Inputs:
%  - str         : String or cell array of strings for which one wants
%                  to white spaces.
%  - delimiter   : [Optional] A character that is used to join
%                  the string. Default is ' '
%
% Outputs:
%  - str         : Joined string
%
% The function is based on strtok function
%
% Compliant with MatLab 6
%
% SIREHNA
% GJ
if nargin == 1
    delimiter = ' ';
end
if isempty(str)
    str = '';
end
if iscell(str)
    res = str{1};
    for i = 2:numel(str)
        res = [res delimiter str{i}];
    end
    str = res;
end
return;
