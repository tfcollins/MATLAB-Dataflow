function DumpSignal(dataSig)

% Asserts
assert(isa(dataSig, 'double') && ~isreal(dataSig) && all(size(dataSig) == [32768,1]))

if abs(dataSig(1))<0
    fprintf('Should not happend\n');
end

end
