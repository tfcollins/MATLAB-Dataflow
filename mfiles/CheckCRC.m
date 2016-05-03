function CheckCRC(codeword)

% Asserts
assert(isa(codeword, 'double') && isreal(codeword) && all(size(codeword) == [18,1]))

persistent crcDetect frames

if isempty(crcDetect)
    crcDetect   = comm.CRCDetector([1 0 0 1], 'ChecksumsPerFrame',2);
    %fprintf(['CRC check result: [%d %d]'],int64(0),int64(0));
    fprintf('CRC: [%d %d] | Frame Count: (%5d)',int64(0),int64(0),int64(10000));
    frames = 0;
end


% Decode messages with and without errors using a CRC decoder
[~, err]   = step(crcDetect, codeword);

% Increment processed frames
frames = frames + 1;
removespaces = repmat('\b',1,39);
fprintf([removespaces,'CRC: [%d %d] | Frame Count: (%5d)'],...
    int64(err(1)),int64(err(2)),int64(frames));

end
