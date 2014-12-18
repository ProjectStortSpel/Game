ClientLobbySystem = System()

ClientLobbySystem.Initialize = function(self)
	self:SetName("ClientLobbySystem System")

	Net.Receive("NewGame", ClientLobbySystem.NewGame);
	
	self:AddComponentTypeToFilter("Null", FilterType.Mandatory);
	
	print("ClientLobbySystem initialized!")
end

ClientLobbySystem.Update = function(self, dt)
	
	if GameRunning == true or Net.IsRunning() == true then
		return
	end
	
	
	if Net.IsConnected() then
		local text = "Connected to server.";	
		graphics:RenderSimpleText(text, 70, 5);
	end
	
	if Net.IsRunning() == false then
		graphics:RenderSimpleText("This is a lobby state!", 70, 4);
	end

end

ClientLobbySystem.OnEntityAdded = function(self, entityId)

end
ClientLobbySystem.OnEntityRemoved = function(self, entityId)

end


ClientLobbySystem.NewGame = function(self, _ip, _port)
	Console.Print("ClientLobbySystem.NewGame");

	GameRunning = true

	local id = Net.StartPack("NewGameLoaded");
	Net.SendToServer(id);
	
end