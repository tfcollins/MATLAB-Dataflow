function freqSig = FFTSignal(dataSig)

% Asserts
assert(isa(dataSig, 'double') && ~isreal(dataSig) && all(size(dataSig) == [64,1]))

persistent dspfft

if isempty(dspfft)
    dspfft = dsp.FFT('FFTLength',64);
end

freqSig = step(dspfft,dataSig);

end
