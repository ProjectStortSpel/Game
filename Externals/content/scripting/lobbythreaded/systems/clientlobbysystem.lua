ClientLobbySystem = System()

ClientLobbySystem.Initialize = function(self)
	self:SetName("ClientLobbySystem System")
	
	self:UsingUpdate()
	self:UsingEntitiesAdded()
	self:UsingEntitiesRemoved()

	Net.Receive("NewGame", self.NewGame);
	
	self:AddComponentTypeToFilter("Username", FilterType.Mandatory);
	self:AddComponentTypeToFilter("NetConnection", FilterType.Mandatory);
	
	print("ClientLobbySystem initialized!")
end

ClientLobbySystem.Update = function(self, dt, taskIndex, taskCount)
	
	if GameRunning then
		return
	end
	
	
	if Net.IsConnected() then
	
		local text = "Connected to server.";	
		graphics:RenderSimpleText(text, 55, 5);
	
	end

end

ClientLobbySystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

	if GameRunning then
		return
	end

end
ClientLobbySystem.EntitiesRemoved = function(self, dt, taskIndex, taskCount, entities)

	if GameRunning then
		return
	end

end


ClientLobbySystem.NewGame = function(self, _ip, _port)
	Console.Print("ClientLobbySystem.NewGame");
end