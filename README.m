%% Dataflow with MATLAB
%
%% Boost Requirement
% This demo has only been built and run on linux.  It requires the boost
% development packages that are the same version as those used to compile
% the uhd drivers.  These are usually the same as the boost version shipped
% with MATLAB.  Just look at the libraries in (matlabroot)/bin/glnxa64/,
% they should look like libboost_chrono.so.x.xx.x, where the x's are the
% version numbers.  I would recommend building it from source rather than
% trying to find the prebuild binaries.
%
%% Building The Demo
% With the USRP support packet installed, run the script *builder* to build
% the executable.  This will produce an exe file called RX.  It can be run
% from MATLAB !./RX
%
%% Code Structure
% The code is setup into four specific pieces.  First of all the backbone
% utility functions.  These are all in C++, and provide the flow control in
% the program to multi-thread everything.  They should not be editted and
% include *flowMP.h* and *utils.h*
%
% The graph setup is done by editting the *mainPD.cpp* C++ file.  It
% outlines how data should flow from block to block.  It should be rather
% self explanatory.
%
% The individual C++ function interfaces to be threaded are located in the
% file *blocks.h*.  These functionals must contain the C++ function
% generated from MATLAB and handle memory.  This management maybe added to
% the backbone, but is difficult due to the custom data types associated
% with MATLAB Coder.
%
% The final piece is the MATLAB Functions.  These must be added to the
% builder script list in the variable *functionsToThread*.
%
%% Discussion (Threads, Memory)
%
% * Threading model
% All threading is done in *flowMP.h* and *utils.h*.  Every Worker object
% created in a simulation will run in it's own thread.  Connected workers,
% connected by there ports, shared a common queue.  Each queue has an
% associated mutex, conditional, and atomic integer.  They allow
% concurrency control.  To maintain throughput everytime a thread adds data
% to a queue it will signal the consuming thread to wake up and process
% data.  If the thread is busy it will be notified by a non-zero atomic
% integer queue size.  This way if there is data in the queue and the
% thread missed the signal, it will automatically process available data.
%
% * Data management
% The backend threading only moves pointers around.  It is not responsible
% to allocating and freeing memory.  This memory managment may be added in
% the future, but due to MATLAB's custom datatypes it can be difficult to
% manage.
%
%% Useful Tools
%
% * Intel vTune Amplifier: C++ speed profiling
% * Valgrind Massif: Heap and stack memory usage
% * gprof: C++ speed profiling

% [EOF]
