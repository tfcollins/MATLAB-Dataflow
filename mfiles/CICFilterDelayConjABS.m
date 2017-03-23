function output = CICFilterDelayConjABS(inputC)

% Asserts
%assert(isa(inputC, 'double') && ~isreal(inputC) && all(size(inputC) == [80*100,1]))

persistent lastInputs lastOutput lastInputsDelayConj

numTaps = 16;
delay = 16;

% Initialize History
if isempty(lastInputs) || isempty(lastOutput)
    lastInputsDelayConj = complex(zeros(numTaps,1));
    lastInputs = complex(zeros(numTaps,1));
    lastOutput = complex(0);
end

inputDelayConj = coder.nullcopy(complex(zeros(size(inputC))));
output         = coder.nullcopy(complex(zeros(size(inputC))));

% Delay, Conjugate, Multiply
inputDelayConj(1) = conj( lastInputs(1) ) * inputC(1);
% Moving Average
output(1) = lastOutput + inputDelayConj(1) - lastInputsDelayConj(1);

for x = 2:length(inputC)
    
    %output[n] = output[n-1] + input[n] - input[n-taps]
    if (x-numTaps)>0
        % Delay, Conjugate, Multiply
        inputDelayConj(x) = conj(inputC(x-delay)) * inputC(x);
        % Moving Average
        output(x) = output(x-1) + inputDelayConj(x) - inputDelayConj(x-numTaps);
    else
        % Delay, Conjugate, Multiply
        inputDelayConj(x) = conj(lastInputs(x)) * inputC(x);
        % Moving Average
        output(x) = output(x-1) + inputDelayConj(x) - lastInputsDelayConj(x);
    end
    
end

% Save history for next call
lastOutput = output(end);
lastInputs(1:numTaps) = inputC(end-numTaps+1:end);
lastInputsDelayConj(1:numTaps) = inputDelayConj(end-numTaps+1:end);

% % Get magnitude
% outputABS = abs(output);

end
