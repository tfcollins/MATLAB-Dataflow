function ModSignalDump(dataSig)

% Asserts
assert(isa(dataSig, 'double') && ~isreal(dataSig) && all(size(dataSig) == [64,1]))


persistent hModulator

if isempty(hModulator)
    % Encode the message words using a CRC generator
    hModulator = comm.RectangularQAMModulator(8,'BitInput',false);
end

% Create binary data for 32, 3-bit symbols
data = (real(dataSig));
% Modulate and plot the data
o = step(hModulator, data);

if abs(o(1))<0
    fprintf('Should not happend\n');
end


end
