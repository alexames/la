package.cpath = package.cpath .. ";../bin/?.so;../bin/?.dll"

require "la"
require "audio"
prepareaudio "music"
la.Play()
while la.GetBeat() < lastbeat do
end

