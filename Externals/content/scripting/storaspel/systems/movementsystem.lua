MovementSystem = System()

MovementSystem.Run = function(self, dt)
	print("Run")
end

MovementSystem.Initialize = function(self)
	print("Initialize")
	self:AddComponentTypeToFilter("Velocity", FilterType.Mandatory)
	--self:AddComponentTypeToFilter("Velocity", FilterType.Excluded)
end

MovementSystem.OnEntityAdded = function(self, entityId)
	print("OnEntityAdded")
end

MovementSystem.OnEntityRemoved = function(self, entityId)
	print("OnEntityRemoved")
end

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(MovementSystem)