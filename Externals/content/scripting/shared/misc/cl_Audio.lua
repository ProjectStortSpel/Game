Net.Receive("Client.SetDistance",
	function(id, ip, port)
		local near = Net.ReadFloat(id)
		local far = Net.ReadFloat(id)
		Audio.SetDistance(near, far)
	end
)
Net.Receive("Client.SetCameraPosition",
	function(id, ip, port)
		local x = Net.ReadFloat(id)
		local y = Net.ReadFloat(id)
		local z = Net.ReadFloat(id)
		Audio.SetCameraPosition(x, y, z)
	end
)
Net.Receive("Client.LoadMusic",
	function(id, ip, port)
		local filepath = Net.ReadString(id)
		Audio.LoadMusic(filepath)
	end
)
Net.Receive("Client.PlayMusic",
	function(id, ip, port)
		Audio.PlayMusic()
	end
)
Net.Receive("Client.PauseMusic",
	function(id, ip, port)
		Audio.PauseMusic()
	end
)
Net.Receive("Client.ResumeMusic",
	function(id, ip, port)
		Audio.ResumeMusic()
	end
)
Net.Receive("Client.StopMusic",
	function(id, ip, port)
		Audio.StopMusic()
	end
)
Net.Receive("Client.FadeInMusic",
	function(id, ip, port)
		local ms = Net.ReadInt(id)
		Audio.FadeInMusic(ms)
	end
)
Net.Receive("Client.FadeOutMusic",
	function(id, ip, port)
		local ms = Net.ReadInt(id)
		Audio.FadeOutMusic(ms)
	end
)
  
Net.Receive("Client.LoadSound",
	function(id, ip, port)
		local name = Net.ReadString(id)
		local filepath = Net.ReadString(id)
		Audio.LoadSound(name, filepath)
	end
)
Net.Receive("Client.PlaySound",
	function(id, ip, port)
		local name = Net.ReadString(id)
		local loop = Net.ReadBool(id)
		Audio.PlaySound(name, loop)
	end
)
Net.Receive("Client.PlaySoundC",
	function(id, ip, port)
		local name = Net.ReadString(id)
		local channelName = Net.ReadString(id)
		local loop = Net.ReadBool(id)
		Audio.PlaySound(name, channelName, loop)
	end
)
Net.Receive("Client.PauseSound",
	function(id, ip, port)
		local channelName = Net.ReadString(id)
		Audio.PauseSound(channelName)
	end
)
Net.Receive("Client.ResumeSound",
	function(id, ip, port)
		local channelName = Net.ReadString(id)
		Audio.ResumeSound(channelName)
	end
)
Net.Receive("Client.StopSound",
	function(id, ip, port)
		local channelName = Net.ReadString(id)
		Audio.StopSound(channelName)
	end
)
Net.Receive("Client.SetSoundPosition",
	function(id, ip, port)
		local channelName = Net.ReadString(id)
		local x = Net.ReadFloat(id)
		local y = Net.ReadFloat(id)
		local z = Net.ReadFloat(id)
		
		Audio.SetSoundPosition(channelName, x, y, z)
	end
)
Net.Receive("Client.FadeInSound",
	function(id, ip, port)
		local name = Net.ReadString(id)
		local ms = Net.ReadInt(id)
		local loop = Net.ReadBool(id)
		Audio.FadeInSound(name, ms, loop)
	end
)
Net.Receive("Client.FadeInSoundC",
	function(id, ip, port)
		local name = Net.ReadString(id)
		local channelName = Net.ReadString(id)
		local ms = Net.ReadInt(id)
		local loop = Net.ReadBool(id)
		Audio.FadeInSound(name, channelName, ms, loop)
	end
)
Net.Receive("Client.FadeOutSound",
	function(id, ip, port)
		local channelName = Net.ReadString(id)
		local ms = Net.ReadInt(id)
		Audio.FadeOutSound(channelName, ms)
	end
)
Net.Receive("Client.SetSoundVolume",
	function(id, ip, port)
		local channelName = Net.ReadString(id)
		local volume = Net.ReadInt(id)
		Audio.SetSoundVolume(channelName, volume)
	end
)