classdef GraphGenerator  < handle
    % Flowgraph C++ File Generator
    %
    % To do:
    %   Manage initializer functions better, but ok for now I think
    %
    properties
        libname = 'RX'
        graphname = 'MyGraph'
        SimDurationSec = 60; % set SimDuration<=0 for continuous run
        blocks = {}
        blocksUniqueNames = {}
    end

    methods
        function Connect(self,blockA,portA,blockB,portB)
            out = [];
            for b=1:length(self.blocksUniqueNames)
                if strcmp(self.blocksUniqueNames{b},blockA)
                    out = b;
                    outname = self.blocksUniqueNames{b};
                end
            end
            % Check block in graph
            if isempty(out)
                error(['No block named:',blockA]);
            else
                for b=1:length(self.blocksUniqueNames)
                    if b==out
                        continue;
                    end
                    % Check block in graph
                    if strcmp(self.blocksUniqueNames{b},blockB)
                        in = b;
                        inname = self.blocksUniqueNames{b};
                    end
                end
            end
            % Set connections
%             self.blocks{in}.connects.atin{portB+1} = {self.blocks{out}.name,num2str(portA)};
%             self.blocks{out}.connects.atout{portA+1} = {self.blocks{in}.name,num2str(portB)};
            self.blocks{in}.connects.atin{portB+1} = {outname,num2str(portA)};
            self.blocks{out}.connects.atout{portA+1} = {inname,num2str(portB)};

        end
        %% Add Blocks To Graph
        function AddBlock(self,blockname,inports,outports,unique_name)

            sig = {'bname','pnum'};
            if inports>0;  i = {};for k=1:inports;    i = {i{:},sig};end; else i = cell(0);end;
            if outports>0; o = {};for k=1:outports;   o = {o{:},sig};end; else o = cell(0);end;

            connections = struct('atin',{i},...
                                 'atout',{o});

            block = struct('name',blockname,...
                'inports',inports,...
                'outports',outports,...
                'connects',connections);

            self.blocks = {self.blocks{:},block};
            if nargin<5
            % Add unique name to each block
            un = 0;
            loop = true;
            while loop
                loop = false;
                for b = 1:length(self.blocksUniqueNames)
                    if strcmp(self.blocksUniqueNames{b},[blockname,'_',num2str(un)])
                        loop = true;
                    end
                end
                if ~loop
                    blocknameUnique = [blockname,'_',num2str(un)];
                else
                    un = un + 1;
                end
            end
            else
               blocknameUnique = unique_name;
            end
            self.blocksUniqueNames = {self.blocksUniqueNames{:},blocknameUnique};

        end
        %% Write C++ Code For Flowgraph SRC file
        function WriteCode(self,benchmark)

            % Write Backend stuff
            main = {'// Backend dataflow',...
                '#include "flowMP.h"',...
                '#include "utils.h"',...
                '#include "graph.h"',...
                '#include <fstream>',...
                '#ifdef _WIN32 // Windows still not fully working (boost problems)',...
                '     #include <Windows.h>',...
                '#endif'};

            % Write Block Headers
            bh = {'','// Block Header Files'};
            for b = 1:length(self.blocks)
                bh = {bh{:}, ['#include "',self.blocks{b}.name,'.h"'] };
            end

            % Write library header
            libh = {'','// Library header files',...
                    ['#include "',self.libname,'_terminate.h"'],...
                    ['#include "',self.libname,'_initialize.h"']};

            % Block wrapper header
            bwh = {'','// Block definitions','#include "blocks_gen.h"'};

            % Now in main function
            mf = {'','using namespace std;','','int main()','{'};

            % Form block instaniations
            bi = {'','    // Create instances of blocks'};
            for b=1:length(self.blocks)
               if self.blocks{b}.inports == 0
                blkstr = [self.blocks{b}.name, ' Source'];
               elseif self.blocks{b}.inports > 0 &&...
                  self.blocks{b}.outports == 0
                blkstr = [self.blocks{b}.name, ' Sink'];
               else
                   blkstr = [self.blocks{b}.name, ' Block'];
               end

                bi = {bi{:},['    Worker block',num2str(b),'(',...
                    self.blocks{b}.name,'BLK, ',...
                    num2str(self.blocks{b}.inports), ', ',...
                    num2str(self.blocks{b}.outports),', ',...
                    self.libname,'_initialize',', ',...
                    self.libname,'_terminate', ', "',...
                    blkstr,'");']};
            end

            % Form connections
            cb = {'','    //// Connect blocks together (Port numbers start with 0) ////'};
            for b = 1:length(self.blocks)

                for c = 1:length(self.blocks{b}.connects.atout)

                    ct = self.blocks{b}.connects.atout{c};% Extract cells

                    % Get connected block number
                    for bb = 1:length(self.blocks)
                        if strcmp(self.blocksUniqueNames{bb},ct{1})
                            connected2 = bb;
                        end
                    end

                    % Write connection string
                        cb = {cb{:},...
                        ['    connect(',...
                         'block',num2str(b),', ',num2str(c-1),','...
                         ' block',num2str(connected2),', ',num2str(ct{2}),   ');']};
                end
            end

            % Add blocks to actual flowgraph
            cg = {''   ,'    //// Create graph and add blocks ////'};
            cg = {cg{:},['    Graph FlowGraph("',self.graphname,'");']};
            blockcollect = {};
            for b = 1:length(self.blocks)
                %blockcollect = {blockcollect{:},['&',self.blocks{b}.name]}; %#ok<*CCAT>
                blockcollect = {blockcollect{:},['&block',num2str(b)]}; %#ok<*CCAT>
            end
            cg = {cg{:},['    FlowGraph.Blocks = {',strjoin(blockcollect,', '),'};']};

            % Label sources and sinks
            lsource = {};
            lsink = {};
            for b = 1:length(self.blocks)
                if self.blocks{b}.inports == 0
                    lsource = {lsource{:},['"',self.blocks{b}.name, ' Source"']}; %#ok<*CCAT>
                elseif self.blocks{b}.outports == 0
                    lsink = {lsink{:},['"',self.blocks{b}.name, ' Sink"']}; %#ok<*CCAT>
                end
            end
            wlso = {'','    // Label Sources and Sinks (Strings from block thread names)',...
                    ['    FlowGraph.Sources = {',strjoin(lsource,','),'};']};
            wlsi = {['    FlowGraph.Sinks = {',strjoin(lsink,','),'};']};

            % Run graph
            rg = {'','    //// Run Graph ////'};
            rg = {rg{:},['    cout<<"MAIN>> Type: ',...
                ' top -H -p `pidof ',self.libname,'` in a terminal to view thread utilization";']};
            rg = {rg{:},['    double SimDuration = ',num2str(self.SimDurationSec),';// set SimDuration<=0 for continuous run']};
            rg = {rg{:},'    cout<<"Starting up (purposely delayed)";'};
            rg = {rg{:},'    FlowGraph.run(SimDuration);// Turn blocks on'};


            % Tail
            tail = {'','    return 0;','','}',''};
            % Enable Benchmarking
            if nargin==2
                numBlocks = length(self.blocks);
                bm = {'','    // Enable Benchmarking',['    block',num2str(numBlocks),'.m_BenchMarkingCount = ',num2str(benchmark),';']};
                % Combine
                mainf = {main{:},bh{:},libh{:},bwh{:},mf{:},bi{:},cb{:},cg{:},bm{:},wlso{:},wlsi{:},rg{:},tail{:}};
                t = strjoin(mainf,'\n');
            else
                % Combine
                mainf = {main{:},bh{:},libh{:},bwh{:},mf{:},bi{:},cb{:},cg{:},wlso{:},wlsi{:},rg{:},tail{:}};
                t = strjoin(mainf,'\n');
            end
            %disp(t);

            % Write to file
            filename = ['mainPD_',self.libname,'_gen.cpp'];
            fid = fopen(['src/',filename],'w');
            fprintf(fid,t);
            fclose(fid);

        end
        %% Create Visual Representation of Flowgraph
        function DrawGraph(self,filename)
            % Top part of dot file
            heading = {['digraph ',self.graphname,'{']};
            extras = {'    fillcolor=red;',...
                '    rankdir=LR;',...
                '    node[shape=box,style=filled,fillcolor="#FFFBD0",color=black];'};

            % Add connections
            cb = {};
            for b = 1:length(self.blocks)
                if (~isempty(self.blocks{b}.connects.atout) || ~isempty(self.blocks{b}.connects.atin))
                for c = 1:length(self.blocks{b}.connects.atout)

                    ct = self.blocks{b}.connects.atout{c};

                    % Get connected block number
                    for bb = 1:length(self.blocks)

                        if bb == b
                            continue;
                        end
                        if strcmp( self.blocksUniqueNames{bb},ct{1})
                            connected2 = bb;
                        end
                    end
                    % Write connection string
                    cb = {cb{:},['    ',self.blocksUniqueNames{b},'->',...
                        self.blocksUniqueNames{connected2},';']};
                end
                else % Place blocks not connected to anything
                    cb = {cb{:},['    ',self.blocksUniqueNames{b},';']};
                end
            end
           % Tail
           tail = {'}'};

           % Combine
           together = {heading{:},extras{:},cb{:},tail{:}};
           dotfile = strjoin(together,'\n');
           %disp(dotfile);

           % Write to file
           fid = fopen(['graphs/',filename],'w');
           fprintf(fid,dotfile);
           fclose(fid);

           % Convert dot file to graph
           dot = '/usr/bin/dot';
           if ismac
              dot = '/usr/local/bin/dot';
           end
           system([dot,' graphs/',filename,' -Tpng -o graphs/',filename,'.png']);

           % Show Graph
           I = imread(['graphs/',filename,'.png']);
           figure('Name',['FlowGraph: ',self.graphname],'NumberTitle','off');
           imshow(I);
           pause(1);

        end

        function Build(self)

            f2t = {};
            for b = 1:length(self.blocks)
                f2t = {f2t{:},self.blocks{b}.name}; %#ok<*CCAT>
            end
            f2t = unique(f2t);
            disp('Starting Build');
            builder(f2t,self.libname);
        end
    end

end
