PostMoveSystem = System()

PostMoveSystem.Initialize = function(self)
	self:SetName("PostMoveSystem System")

	self:AddComponentTypeToFilter("PostMove", FilterType.Mandatory)

	print("PostMoveSystem initialized!")
end

PostMoveSystem.OnEntityAdded = function(self, entity)
	
	local id = world:CreateNewEntity()
	world:CreateComponentAndAddTo("CheckVoid", id)

	id = world:CreateNewEntity()
	world:CreateComponentAndAddTo("CheckCheckpoint", id)
	
	world:KillEntity( entity )
end

PostMoveSystem.OnEntityRemoved = function(self, entity)

	

end