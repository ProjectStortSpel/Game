MovementSystem = System()
MovementSystem.hej = 0
MovementSystem.Update = function(self, dt)

	if self.hej < 10 then
		id = world:CreateNewEntity()
		print("ID ", id)
		world:CreateComponentAndAddTo("Position", id)
		world:CreateComponentAndAddTo("Velocity", id)
		world:CreateComponentAndAddTo("Render", id)
		self.hej = self.hej + 1
	end

end

MovementSystem.Initialize = function(self)
	print("Initialize")
	self:AddComponentTypeToFilter("Position", FilterType.Mandatory)
	
	self:AddComponentTypeToFilter("Velocity", FilterType.Excluded)
end

MovementSystem.OnEntityAdded = function(self, entityId)
	print("OnEntityAdded (LUA)")
	local id = world:CreateNewEntity()
	print("ID ", id)
	world:CreateComponentAndAddTo("Position", id)
	world:CreateComponentAndAddTo("Velocity", id)
	world:CreateComponentAndAddTo("Render", id)
end

MovementSystem.OnEntityRemoved = function(self, entityId)
	print("OnEntityRemoved")
end

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(MovementSystem)