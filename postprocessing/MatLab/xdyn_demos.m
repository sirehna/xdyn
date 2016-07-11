function xdyn_demos(demosId)
% XDYN_DEMOS runs one or several demos.
%
% Demos can be referenced with a numeric id, running from 1 to 11, or with
% a name.
% This allows to run demos without having to know the names of the
% demos, or to run a demo from a description.
%
% This following instruction will run the first demo
%
% xdyn_demos(1)
%
% This call is equivalent to
%
%   xdyn_demos('tutorial_stable_cube_test')
%
% One can call several demos by giving several demos id
%
% xdyn_demos([1,8])
%
% This call is equivalent to
%
%   xdyn_demos({'tutorial_stable_cube_test', 'tutorial_05_froude_krylov'})

if nargin == 0
    demosId = 1:11;
end
if ischar(demosId)
    demosId = {demosId};
end
for i = 1:numel(demosId)
    if iscell(demosId)
        id = demosId{i};
    else
        id = demosId(i);
    end
    switch id
        case {1, 'tutorial_stable_cube_test'}
            tutorial_stable_cube_test;
        case {2, 'tutorial_cube_in_waves'}
            tutorial_cube_in_waves;
        case {3, 'tutorial_oscillating_cube_test'}
            tutorial_oscillating_cube_test;
        case {4, 'tutorial_anthineas_in_waves'}
            tutorial_anthineas_in_waves;
        case {5, 'tutorial_01_falling_ball'}
            tutorial_01_falling_ball;
        case {6, 'tutorial_03_waves'}
            tutorial_03_waves;
        case {7, 'tutorial_04_damping'}
            tutorial_04_damping;
        case {8, 'tutorial_05_froude_krylov'}
            tutorial_05_froude_krylov;
        case {9, 'tutorial_06_propulsion'}
            tutorial_06_propulsion;
        case {10, 'tutorial_07_radiation_damping'}
            tutorial_07_radiation_damping;
        case {11, 'tutorial_08_diffraction'}
            tutorial_08_diffraction;
        otherwise
            error
    end
end

%% tutorial_cube_in_waves
function tutorial_cube_in_waves
disp('');
disp('cube_in_waves.yml');
disp('');
param = struct;
param.solver            = 'rk4';
param.dt                = '0.1';
param.tstart            = '0.0';
param.tend              = '10.0';
param.yaml              = '../demos/cube_in_waves.yml';
param.outputFilename    = 'tutorial_cube_in_waves.h5';
importResults = true;
verbose = true;
xdyn_postProcess(xdyn_run(param, importResults, verbose), true);

function tutorial_stable_cube_test
disp('');
disp('stable_cube_test.yml');
disp('');
param = struct;
param.solver            = 'rk4';
param.dt                = '0.1';
param.tstart            = '0.0';
param.tend              = '10.0';
param.yaml              = '../demos/stable_cube_test.yml';
param.outputFilename    = 'tutorial_stable_cube_test.h5';
importResults = true;
verbose = true;
xdyn_postProcess(xdyn_run(param, importResults, verbose), true);

%% tutorial_cube_in_waves
function tutorial_oscillating_cube_test
disp('');
disp('cube_in_waves.yml');
disp('');
param = struct;
param.solver            = 'rk4';
param.dt                = '0.1';
param.tstart            = '0.0';
param.tend              = '10.0';
param.yaml              = '../demos/oscillating_cube_test.yml';
param.outputFilename    = 'oscillating_cube_test.h5';
importResults = true;
verbose = true;
xdyn_postProcess(xdyn_run(param, importResults, verbose), true);

function tutorial_anthineas_in_waves
disp('');
disp('anthineas_in_waves');
disp('');
param = struct;
param.solver            = 'rk4';
param.dt                = '0.1';
param.tstart            = '0.0';
param.tend              = '10.0';
param.yaml              = '../demos/anthineas_in_waves.yml';
param.outputFilename    = 'tutorial_cube_in_waves.h5';
importResults = true;
verbose = true;
xdyn_postProcess(xdyn_run(param, importResults, verbose), true);

%% tutorial_01_falling_ball
function tutorial_01_falling_ball
disp('');
disp('Tutorial 01 falling ball');
disp('');
importResults = true;
verbose = true;
plotResult = true;
param = struct;
param.solver = 'rk4';
param.dt = '0.1';
param.tstart = '0.0';
param.tend = '10.0';
param.exportWaves = false;
param.yaml = '../demos/tutorial_01_falling_ball.yml';
param.outputFilename = 'tutorial_01_falling_ball.h5';
result = xdyn_run(param, importResults, verbose);
simu = xdyn_postProcess(result, plotResult);

% One can fetch simulation results and plot
figure; hold on; grid on; box on;
X = simu.states.ball.x-simu.states.ball.x(1);
Y = simu.states.ball.y-simu.states.ball.y(1);
Z = simu.states.ball.z-simu.states.ball.z(1);
plot(simu.t,sqrt(X.^2+Y.^2+Z.^2));
xlabel('Time (s)');
ylabel('Distance (m)');
title('XDYN - Tutorial falling ball');

function tutorial_03_waves
disp('');
disp('Tutorial 03 waves');
disp('');
param = struct;
param.solver            = 'rk4';
param.dt                = '0.1';
param.tstart            = '0.0';
param.tend              = '10.0';
param.yaml              = '../demos/tutorial_03_waves.yml';
param.outputFilename    = 'tutorial_03_waves.h5';
param.exportWaves       = true;
importResults = true;
verbose = true;
xdyn_postProcess(xdyn_run(param, importResults, verbose), true);

function tutorial_04_damping
disp('');
disp('Tutorial 04 damping');
disp('');
param = struct;
param.solver            = 'rk4';
param.dt                = '0.1';
param.tstart            = '0.0';
param.tend              = '10.0';
param.yaml              = '../demos/tutorial_04_damping.yml';
param.outputFilename    = 'tutorial_04_damping.h5';
importResults = true;
verbose = true;
xdyn_postProcess(xdyn_run(param, importResults, verbose), true);

function tutorial_05_froude_krylov
disp('');
disp('Tutorial 05 Froude Krylov');
disp('');
param = struct;
param.solver            = 'rk4';
param.dt                = '0.1';
param.tstart            = '0.0';
param.tend              = '10.0';
param.yaml              = '../demos/tutorial_05_froude_krylov.yml';
param.outputFilename    = 'tutorial_05_froude_krylov.h5';
param.exportWaves       = true;
importResults           = true;
verbose                 = true;
xdyn_postProcess(xdyn_run(param, importResults, verbose), true);

%% tutorial_06_propulsion
function tutorial_06_propulsion
disp('');
disp('Tutorial 06 propulsion');
disp('');
param = struct;
param.solver            = 'rk4';
param.dt                = '0.1';
param.tstart            = '0.0';
param.tend              = '10.0';
param.yaml              = {'../demos/tutorial_06_propulsion.yml',...
                           '../demos/controlled_forces_commands.yml'};
param.outputFilename    = 'tutorial_06_propulsion.h5';
importResults = true;
verbose = true;
xdyn_postProcess(xdyn_run(param, importResults, verbose), true);

%% tutorial_08_diffraction
function tutorial_07_radiation_damping
disp('');
disp('Tutorial 07 diffraction');
disp('');
param = struct;
param.solver            = 'rk4';
param.dt                = '0.1';
param.tstart            = '0.0';
param.tend              = '10.0';
param.yaml              = {'../demos/tutorial_07_radiation_damping.yml',...
                           '../demos/controlled_forces_commands.yml'};
param.outputFilename    = 'tutorial_07_radiation_damping.h5';
importResults = true;
verbose = true;
xdyn_postProcess(xdyn_run(param, importResults, verbose), true);

%% tutorial_08_diffraction
function tutorial_08_diffraction
disp('');
disp('Tutorial 08 diffraction');
disp('');
param = struct;
param.solver            = 'rk4';
param.dt                = '0.1';
param.tstart            = '0.0';
param.tend              = '10.0';
param.yaml              = '../demos/tutorial_08_diffraction.yml';
param.outputFilename    = 'tutorial_08_diffraction.h5';
importResults = true;
verbose = true;
xdyn_postProcess(xdyn_run(param, importResults, verbose), true);

