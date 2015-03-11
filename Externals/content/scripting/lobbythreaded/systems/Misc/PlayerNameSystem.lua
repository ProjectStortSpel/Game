PlayerNameSystem = System()
PlayerNameSystem.spin = 0.1

PlayerNameSystem.Initialize = function ( self )
	--	Set Name
	self:SetName("PlayerNameSystem")

	--	Filters
	self:AddComponentTypeToFilter("PlayerName", FilterType.Mandatory)
end

PlayerNameSystem.PostInitialize = function(self)
	local nameEntity = world:CreateNewEntity()
	world:CreateComponentAndAddTo("PlayerName", nameEntity)
	world:SetComponent(nameEntity, "PlayerName", "Name", "DefaultName")
end