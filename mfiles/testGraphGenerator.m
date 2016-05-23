% Test Graph Generator
clc;
clear all;

%% Simple Flowgraph
g = GraphGenerator;

g.AddBlock('src',0,1)
g.AddBlock('awgn',1,2)
g.AddBlock('filter',1,1)
g.AddBlock('sink',2,0)

g.Connect('src_0',0,'awgn_0',0)
g.Connect('awgn_0',0,'sink_0',0)
g.Connect('awgn_0',1,'filter_0',0)
g.Connect('filter_0',0,'sink_0',1)

g.DrawGraph('simple_with_channel.dot');


pause(4);

%% FFT Example
% Construct graph and build all code
clear all;close all;
g = GraphGenerator;
g.SimDurationSec = 30;
g.libname = 'MyFFT';

g.AddBlock('GenSignal',0,1,'GS');
g.AddBlock('FFTSignal',1,1,'FFT1');
g.AddBlock('FFTSignal',1,1,'FFT2');
g.AddBlock('DumpSignal',1,0,'DS');

g.Connect('GS',0,'FFT1',0);
g.Connect('FFT1',0,'FFT2',0);
g.Connect('FFT2',0,'DS',0);

limit = 100;
g.WriteCode(limit);

g.DrawGraph('simple_with_channel.dot');

g.Build();

%% Construct graph and build all code
clear all;close all;
g = GraphGenerator;
g.SimDurationSec = 30;

g.AddBlock('GenCRC',0,1);
%g.AddBlock('AddNoise',1,1);
g.AddBlock('CheckCRC',1,0);

%g.Connect('GenCRC',0,'AddNoise',0);
%g.Connect('AddNoise',0,'CheckCRC',0);
g.Connect('GenCRC_0',0,'CheckCRC_0',0);

limit = 100;
g.WriteCode(limit);

g.DrawGraph('simple_with_channel.dot');

g.Build();

%!./RX
%Graph(limit)
