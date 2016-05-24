function output = GenSignal()


persistent hModulator

if isempty(hModulator)
    % Encode the message words using a CRC generator
    hModulator = comm.RectangularQAMModulator(8,'BitInput',true);
end

% Create binary data for 32, 3-bit symbols
data = randi([0 1],3*32768,1);
% Modulate and plot the data
o = step(hModulator, data);
output = double(o);

end
