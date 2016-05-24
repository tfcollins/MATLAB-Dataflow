function setbuildargsBoost(buildInfo)
global functionsToThread outputFunctionName benchmarking;
% The example being compiled requires threading support.
% The flag requests that the thread library be included
% in the build

    %compileFlags = {'-std=c++11 -lboost_thread -lboost_system'};
    % add '-g -pg' for debugging
    if benchmarking
    compileFlags = {'-std=c++11','-DBENCHMARKING'};
    else
    compileFlags = {'-std=c++11'};
    end
    addCompileFlags(buildInfo, compileFlags);

    % add '-g -pg' for debugging
    linkFlags = {'-pthread'};
    %end
    %linkFlags = {'-lboost_thread -lboost_system'};
    addLinkFlags(buildInfo, linkFlags);

    library = {'boost_thread'};
    addSysLibs(buildInfo, library);
    library = {'boost_system'};
    addSysLibs(buildInfo, library);
    % Not used anymore
    %library = {'boost_signals'};
    %addSysLibs(buildInfo, library);

    % Generate header blocks
    system(['python2 process_header.py ',strjoin(functionsToThread,' '),' ',outputFunctionName{:},' >include/blocks_gen.h']);
