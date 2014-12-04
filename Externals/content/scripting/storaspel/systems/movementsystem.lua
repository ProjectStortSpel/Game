MovementSystem = System()

MovementSystem.Update = function(self, dt)

end

MovementSystem.Initialize = function(self)
	print("Initialize")
	self:AddComponentTypeToFilter("Position", FilterType.Mandatory)
	
	self:AddComponentTypeToFilter("Render", FilterType.Excluded)
end

MovementSystem.OnEntityAdded = function(self, entityId)
	print("OnEntityAdded (LUA)")
end

MovementSystem.OnEntityRemoved = function(self, entityId)
	print("OnEntityRemoved")
end

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(MovementSystem)