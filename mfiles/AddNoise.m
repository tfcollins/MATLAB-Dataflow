function sigWithNoise = AddNoise(signal)

% Asserts
assert(isa(signal, 'logical') && isreal(signal) && all(size(signal) == [64,1]))

persistent AWGN
if isempty(AWGN)
    AWGN = comm.AWGNChannel('EbNo', 15, ...
        'BitsPerSymbol', log2(2));
end

% Add noise
sigWithNoise = step(AWGN, double(signal));

end