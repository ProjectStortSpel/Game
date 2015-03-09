PlayerNameSystem = System()
PlayerNameSystem.spin = 0.1

PlayerNameSystem.Initialize = function ( self )
	--	Set Name
	self:SetName("PlayerNameSystem")
	
	self:InitializeNetworkEvents()
	
	--	Filters
	self:AddComponentTypeToFilter("PlayerName", FilterType.Mandatory)
end

PlayerNameSystem.PostInitialize = function(self)

	local nameEntity = world:CreateNewEntity()
	world:CreateComponentAndAddTo("PlayerName", nameEntity)
	world:SetComponent(nameEntity, "PlayerName", "Name", "DefaultName")
	print(nameEntity)
end

PlayerNameSystem.OnConnectedToServer = function(self, _ip, _port, _message)
	local entity = self:GetEntities()
	local name = world:GetComponent(entity[1], "PlayerName", "Name"):GetText()
	
	local id = Net.StartPack("SERVER_RECEIVE_PLAYER_NAME")
	Net.WriteString(id, name)
	Net.SendToServer(id)	
end