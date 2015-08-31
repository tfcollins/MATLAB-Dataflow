%% Example build
%
% 1. Create MATLAB functions to be threaded.
%   - Here we will use two: GenCRC.m, CheckCRC.m
%
% 2. Add functions to builder.m in functionsToThread array.
%
% 3. Edit blocks.h
%   - Add a function for each MATLAB function to be threaded
%   - You may have to generate to a dll and look at the headers to get the
%   function prototypes first
%   - MATLAB functions require you to pass pointers for output arrays
%   - Functions must manage memory.  Make sure you delete inputs to blocks
%   after you use them
%
% 4. Edit mainDP.cpp adding objects to support these blocks.
%   - Add header files of the MATLAB functions to be generated to this
%   file.  They are the same names as the MATLAB functions
%   - Add an object per-function added to the blocks.h file, adding these
%   names as the first agument to each of the object constructors.
%   - Add objects to graph, labeling sources and sinks
%
% 5. Run builder
%   - This will create a exe called RX (the name of your
%   outputFunctionName)
%
% 6. Run exe !./RX