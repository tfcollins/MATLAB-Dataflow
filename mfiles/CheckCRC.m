function CheckCRC(codeword)

% Asserts
assert(isa(codeword, 'logical') && isreal(codeword) && all(size(codeword) == [18,1]))

persistent crcDetect

if isempty(crcDetect)
    crcDetect   = comm.CRCDetector([1 0 0 1], 'ChecksumsPerFrame',2);
end
% Decode messages with and without errors using a CRC decoder
[~, err]   = step(crcDetect, codeword);
fprintf('CRC check result: [%d %d]\n',int64(err(1)),int64(err(2)));

end
