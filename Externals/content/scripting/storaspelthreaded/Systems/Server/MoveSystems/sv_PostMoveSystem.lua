PostMoveSystem = System()

PostMoveSystem.Initialize = function(self)
	--	Set Name
	self:SetName("PostMoveSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter("PostMove", FilterType.Mandatory)
end

PostMoveSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

	for n = 1, #entities do
		local entity = entities[n]
		local id = world:CreateNewEntity()
		world:CreateComponentAndAddTo("CheckVoid", id)

		id = world:CreateNewEntity()
		world:CreateComponentAndAddTo("CheckCheckpoint", id)
		
		world:KillEntity( entity )
	end
end