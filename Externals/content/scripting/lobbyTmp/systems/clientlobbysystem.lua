ClientLobbySystem = System()

ClientLobbySystem.Initialize = function(self)
	self:SetName("ClientLobbySystem System")

	Net.Receive("NewGame", ClientLobbySystem.NewGame);
	
	self:AddComponentTypeToFilter("Username", FilterType.Mandatory);
	self:AddComponentTypeToFilter("NetConnection", FilterType.Mandatory);
	
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

	if GameRunning == true or Net.IsRunning() == true then
		return
	end

end
ClientLobbySystem.OnEntityRemoved = function(self, entityId)

	if GameRunning == true or Net.IsRunning() == true then
		return
	end

end


ClientLobbySystem.NewGame = function(self, _ip, _port)
	Console.Print("ClientLobbySystem.NewGame");
	
	local id = Net.StartPack("NewGameLoaded");
	Net.SendToServer(id);
	
end