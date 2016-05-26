function DumpSignal(dataSig)

% Asserts
assert(isa(dataSig, 'double') && ~isreal(dataSig) && all(size(dataSig) == [64,1]))

if abs(dataSig(1))<0
    fprintf('Should not happend\n');
end

end
