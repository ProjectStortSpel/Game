CameraNewRoundSystem = System()

CameraNewRoundSystem.Initialize = function ( self )
	--	Set Name
	self:SetName("CameraNewRoundSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter("NewRound", FilterType.Mandatory)
end

CameraNewRoundSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)
	local id = world:CreateNewEntity()
	world:CreateComponentAndAddTo("CameraOnPlayer", id)
end