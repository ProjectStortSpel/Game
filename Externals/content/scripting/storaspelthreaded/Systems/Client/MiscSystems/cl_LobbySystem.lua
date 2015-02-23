LobbySystem = System()
LobbySystem.Name = "LobbyMenu"

LobbySystem.Initialize = function ( self )
	--	Set Name
	self:SetName(self.Name.."System")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter("LobbyPlayerReady", FilterType.RequiresOneOf)
end

LobbySystem.EntitiesAdded = function(self, dt, entities)
	for n = 1, #entities do
		local entityId = entities[n]
		
		Net.SendToServer(Net.StartPack("Server.ReadyCheck"))
		
		world:KillEntity(entityId)
	end
end