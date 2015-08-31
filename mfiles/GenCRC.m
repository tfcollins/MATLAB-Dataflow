function output = GenCRC()

persistent crcGen

if isempty(crcGen)
    % Encode the message words using a CRC generator
    crcGen      = comm.CRCGenerator('z^3 + 1', 'ChecksumsPerFrame',2);
end

x = logical([1 0 1 1 0 1 0 1 1 1 0 1]');
output = step(crcGen, x);


end