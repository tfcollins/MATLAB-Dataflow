% Test Graph Generator
clc;
clear all;

numBins = 50:0.1:350;%1e3;

%% Fast Source
g = GraphGenerator;
g.SimDurationSec = 60; % Will be ignored since in BM mode
g.enableBenchmarking = true;
g.libname = 'FastSRC';

%g.AddBlock('GenSignal',0,1,'GS');
%g.AddBlock('FFTSignal',1,1,'FFT');
%g.AddBlock('DumpSignal',1,0,'DS');

g.AddBlock('GenSignal',0,1,'GS');
g.AddBlock('ModSignalDump',1,0,'MSD');

g.Connect('GS',0,'MSD',0);

limit = 1e6; % MAKE SURE TIMING_SAMPLES IS THE SAME AS THIS VALUE
g.WriteCode(limit);
%g.DrawGraph('simple_with_channel.dot');
g.Build();

%system('unset LD_LIBRARY_PATH; ./FastSRC');
g.Run();

[FastTimes,FastProbs] = eval_timings(numBins);

%% Slow Source
g = GraphGenerator;
g.SimDurationSec = 60; % Will be ignored since in BM mode
g.libname = 'SlowSRC';

g.AddBlock('GenSignal',0,1,'GS');
g.AddBlock('DumpSignal',1,0,'DS');

g.Connect('GS',0,'DS',0);

limit = 1e6; % MAKE SURE TIMING_SAMPLES IS THE SAME AS THIS VALUE
g.enableBenchmarking = true;
g.WriteCode(limit);
%g.DrawGraph('simple_with_channel.dot');
g.Build();

%system('unset LD_LIBRARY_PATH; ./SlowSRC');
g.Run();

[SlowTimes,SlowProbs] = eval_timings(numBins);

%% Plots
plot(FastTimes,FastProbs, SlowTimes,SlowProbs);
legend('Fast Source','Slow Source');
xlabel('Latency (ns)');
ylabel('CDF');
ylim([0 1.01]);




