function results = xdyn_run(param, importResults, verbose)
% XDYN_RUN allows to run XDYN from MatLab.
%
% Inputs:
%   param : Contains data to run xdyn simulator
%           `param` is a structure with the following fields:
%              - yaml           : YAML filename or cell array of YAML filenames
%                                 containing input data for simulation.
%              - solver         : Name of solver to use: Can be euler, rk4
%                                 Default is 'rk4'
%              - dt             : Time step used for integration
%                                 Default is 0.1
%              - tstart         : Start time
%                                 Default is 0
%              - tend           : End time for simulation
%                                 Default is 10
%              - commands       : [Optional] Yaml file used for commands
%                                 Default is ''
%              - outputFilename : [Optional] Name of the HDF5 used to store
%                                 results
%                                 Default is ''
%              - exportWaves    : [Optional] Boolean used to export mesh wave
%                                 elevation
%                                 Default is true
%           Missing parameters are automatically added.
%           If `param` is a char array, it is considered as the YAML
%           filename. All other parameters will be the default parameters
%           If `param` is a cell array of char array, it is considered as
%           the set of YAML files to used for simulation. 
%   importResults : [Optional] Boolean used to import in MatLab the results.
%                   Default is true
%   verbose : [Optional] Boolean used to disp various command
if nargin < 3
    verbose = false;
    if nargin < 2
        importResults = true;
        if nargin < 1
            disp('Examples :');
            disp('    xdyn_run(''simu.yml'')');
            disp('    xdyn_run(''simu.yml'',''simu.h5'')');
            disp('    xdyn_run(struct(''yaml'',''simu.yml''),''simu.h5'')');
            disp('');
            disp('    xdyn_run(struct(''yaml'',''simu.yml''),''simu.h5'')');
            error('You need to provide at least one argument containing simulations parameters');
        end
    end
end
if ispc
    extension = '.exe';
else
    extension = '';
end
simulatorExe = fullfile(fileparts(mfilename),'..','bin',['sim' extension]);
if ischar(param) || iscell(param)
    tmp = param;
    param = struct;
    param.yaml = tmp;
end
if ischar(param.yaml)
    param.yaml = {param.yaml};
end
defaultParam = struct;
defaultParam.solver = 'rk4';
defaultParam.dt = '0.1';
defaultParam.tstart = '0.0';
defaultParam.tend = '10.0';
defaultParam.commands = '';
defaultParam.exportWaves = true;
defaultParam.outputFilename = 'res.h5';
param = tbx_struct_addMissingFields(defaultParam, param);
[pathstr, name, ext] = fileparts(param.outputFilename);
if ~(strcmpi(ext,'.h5') || strcmpi(ext,'.hdf5'))
    
end
cmdLine = [simulatorExe ...
           ' ' stringifyCellArrayOfStrings(param.yaml) ...
           ' --solver ' param.solver ...
           ' --dt ' num2str(param.dt) ...
           ' --tstart ' num2str(param.tstart) ...
           ' --tend ' num2str(param.tend) ...
           ' --output ' param.outputFilename
           ];
if ~isempty(param.commands)
    cmdLine = [cmdLine ' --command ' param.commands];
end
if param.exportWaves
    cmdLine = [cmdLine ' --waves true'];
end
if verbose
    disp(cmdLine);
end
[status, result] = system(cmdLine,'-echo');
if verbose
end
if importResults
    plotResult = false;
    results = xdyn_postProcess(param.outputFilename, plotResult);
else
    results = [];
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function res = stringifyCellArrayOfStrings(data)
res = '';
for i = 1:numel(data)
    res = [res ' ' data{i}];
end
return;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function structure = tbx_struct_addMissingFields(structureRef, structure)
% TBX_STRUCT_ADDMISSINGFIELDS sets the default field value for missing
% fields in input structure.
%
% tbx_struct_addMissingFields is generally used for functions that take
% options passed with a structure.
% tbx_struct_addMissingFields can add missing fields to this option
% structure.
%
% For example, let's consider the following function.
% One can give options with partial fieldnames. Those missing are replaced
% with the default values.
% % function result = addition(a,b,options)
% % structureRef.dispInput = false;
% % structureRef.dispResult = false;
% % if nargin < 3
% %     structure = structureRef;
% % end
% % structure = tbx_struct_addMissingFields(structureRef,structure);
% % result = a+b;
% % if options.dispInput
% %     disp([mfilename ' : Input a = ' num2str(a)]);
% %     disp([mfilename ' : Input b = ' num2str(b)]);
% % end
% % if options.dispResult
% %     disp([mfilename ' : Result addition = ' num2str(result)]);
% % end
% % return;
%
% structure = tbx_struct_addMissingFields(structureRef,structure);
%
% Inputs :
%   - structureRef : Structure containing all default fields values
%   - structure    : Structure to be completed with default fieldnames and
%                    associated value. If empty, the returned structure
%                    will be 'structureRef'.
%
% Output :
%   - structure   : Structure completed with default fieldnames and
%                   associated value.
%
% Compliant with MatLab 6
%
% SIREHNA
% GJ
%==========================================================================
% SVN info
% SVN $Id: tbx_struct_addMissingFields.m 1430 2015-11-05 11:01:38Z gj $
% SVN $HeadURL: http://130.66.124.6/svn/matlab_toolbox/struct/tbx_struct_addMissingFields.m $
%==========================================================================

if nargin == 0
    help(mfilename);
    return;
elseif nargin == 1
    structure = structureRef;
    return;
elseif nargin == 2 && (isempty(structure) || isempty(fieldnames(structure)))
    structure = structureRef;
    return;
elseif nargin == 2 && isempty(structureRef)
    return;
end
tbx_assert(numel(structureRef)==1,...
    'Number of elements that is used as reference has to be one.');
tbx_assert(isstruct(structure),'Second argument has to be a structure');
fnames = fieldnames(structureRef);
for i=1:numel(fnames)
    for j=1:numel(structure)
        if ~isfield(structure(j),fnames{i})
            structure(j).(fnames{i}) = structureRef.(fnames{i});
        end
    end
end
return;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function tbx_assert(cond,msg)
% TBX_ASSERT asserts a condition associated to a message.
%
% TBX_ASSERT - Similar to the C "assert" function
%
% tbx_assert(cond,msg)
%
% cond should be an expression which is believed to be true.
% msg is a message describing the assertion.
%
% e.g. tbx_assert(x>5 && x<20,'x is within the allowed limits')
%
% Compliant with MatLab 6
%
% SIREHNA
% GJ
%==========================================================================
% SVN info
% SVN $Id$
% SVN $HeadURL$
%==========================================================================
%
%ASSERT - Similar to the C assert function
%
% ASSERT(cond,msg)
%
% cond should be an expression which is believed to be true.
% msg is a message describing the assertion.
%
% e.g. ASSERT(x>5 && x<20,'x is within the allowed limits')
%
% This should not be thought of as "normal" error handling.
% It is designed to help catch bugs in the code, and so the
% call stack is printed in the command window (so that it
% is not lost if the error is caught).
%
% Note: In R2007a and later, MATLAB has an "assert" function which is
% similar to this one but without the additional printing.  Use this
% one if your code contains lots of "catch" statements, which might
% otherwise hide an error thrown by the MATLAB "assert" function.

% Copyright 2006-2010 The MathWorks, Inc.

% Copyright (c) 2006, The MathWorks, Inc.
% All rights reserved.
%
% Redistribution and use in source and binary forms, with or without
% modification, are permitted provided that the following conditions are
% met:
%
%     * Redistributions of source code must retain the above copyright
%       notice, this list of conditions and the following disclaimer.
%     * Redistributions in binary form must reproduce the above copyright
%       notice, this list of conditions and the following disclaimer in
%       the documentation and/or other materials provided with the distribution
%     * Neither the name of the The MathWorks, Inc. nor the names
%       of its contributors may be used to endorse or promote products derived
%       from this software without specific prior written permission.
%
% THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
% AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
% IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
% ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
% LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
% CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
% SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
% INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
% CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
% ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
% POSSIBILITY OF SUCH DAMAGE.

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
