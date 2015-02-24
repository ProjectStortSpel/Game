PostMoveSystem = System()

PostMoveSystem.Initialize = function(self)
	--	Set Name
	self:SetName("PostMoveSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter("PostMove", FilterType.Mandatory)
end

PostMoveSystem.EntitiesAdded = function(self, dt, entities)

	for n = 1, #entities do
		local entity = entities[n]
		local id = world:CreateNewEntity()
		world:CreateComponentAndAddTo("CheckVoid", id)
		
		world:KillEntity( entity )
	end
end