function result = builder(f2t,ofn,eb)
% Function specific information
global functionsToThread outputFunctionName benchmarking; % Only way to pass to post codegen functions
functionsToThread = f2t;%{'GenCRC','AddNoise','CheckCRC'};
additionalSourceFiles = {'flowMP.h'};
outputFunctionName = {ofn};%{'RX'};
benchmarking = eb;

% Include mfiles
addpath(genpath('mfiles'));

% Coder config setup
cfg = coder.config('exe');
cfg.CustomSource = ['mainPD_',ofn,'_gen.cpp:flowMP.cpp:graph.cpp'];
%cfg.GenerateCodeReplacementReport = true;
if ispc % Windows
    cfg.CustomLibrary = 'C:\Boost\lib\';
    cfg.CustomInclude = 'C:\Boost\include\boost-1_49\';
end
cfg.CustomInclude = 'src/:include/';
cfg.TargetLang='C++';
cfg.PostCodeGenCommand = 'setbuildargsBoost(buildInfo)';
cfg.BuildConfiguration='Faster Runs';
cfg.EnableOpenMP = false;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Do Not Edit Below
%%%%%%%%%%%%%%%%%%%%%%%%%%%%
enableDebugging = false;

% Run Codegen
if enableDebugging
    result = codegen('-config','cfg',...
        functionsToThread{:},...
        additionalSourceFiles{:},...
        '-o',outputFunctionName{:},...
        '-v','-g');
else
    result = codegen('-config','cfg',...
        functionsToThread{:},...
        additionalSourceFiles{:},...
        '-o',outputFunctionName{:},...
        '-v');
end

% % Run Code
% !./RX
%
% % Run C++ profiler and graph maker
% !gprof RX | gprof2dot | dot -Tpng -o output.png

end
