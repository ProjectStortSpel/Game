package.path = package.path .. ";Misc/?.lua"

if Client then

require "cl_LoadAudio"

end

if Server then

require "sv_Resources"

end