% Graph Config
function Graph()

% Source
codeword = GenCRC();
% Process
sigWithNoise = AddNoise(codeword);
% Sink
CheckCRC(sigWithNoise);