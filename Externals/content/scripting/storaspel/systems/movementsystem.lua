MovementSystem = System()

MovementSystem.Update = function(self, dt)

end

MovementSystem.Initialize = function(self)
	print("Initialize")
	self:AddComponentTypeToFilter("Velocity", FilterType.Mandatory)
	self:AddComponentTypeToFilter("Position", FilterType.Excluded)
end

MovementSystem.OnEntityAdded = function(self, entityId)
	print("OnEntityAdded")
	--id = world:CreateNewEntity()
	--world:CreateComponentAndAddTo("Position", id)
end

MovementSystem.OnEntityRemoved = function(self, entityId)
	print("OnEntityRemoved")
end

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(MovementSystem)