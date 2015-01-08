ServerLobbySystem = System()


ServerLobbySystem.Initialize = function(self)
	self:SetName("ServerLobbySystem System");

	self:AddComponentTypeToFilter("Null", FilterType.Mandatory);
	
	print("ServerLobbySystem initialized!");
end

ServerLobbySystem.Update = function(self, dt)
	
	if not GameRunning and Net.IsRunning() then
		ServerLobbySystem:UpdateServerOnline();
	end
end

ServerLobbySystem.UpdateServerOnline = function(self)
	
	if Input.GetKeyState(Key.Return) == InputState.Pressed and not Console.IsOpen() then
		Console.Print("Game started");
		GameRunning = true;
		local id = Net.StartPack("NewGame");
		Net.Broadcast(id);
		Console.Print("NewGame");
	end
end
