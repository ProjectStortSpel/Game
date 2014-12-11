TileMovementSystem = System()

TileMovementSystem.Update = function(self, dt)

	local entities = self:GetEntities()
	for i = 1, #entities do
		local entity = entities[i]

		local position = self:GetComponent(entity, "Position", 0)
		local velocity = self:GetComponent(entity, "Velocity", 0)

		local px, py, pz = position:GetFloat3()
		local vx, vy, vz = velocity:GetFloat3()

		px = px + vx * dt
		py = py + vy * dt
		pz = pz + vz * dt

		position:SetFloat3(px, py, pz)
	end
end

TileMovementSystem.Initialize = function(self)
	self:AddComponentTypeToFilter("Position", FilterType.Mandatory)
	self:AddComponentTypeToFilter("Velocity", FilterType.Mandatory)
	
	print("MovementSystem initialized!")
end

TileMovementSystem.PostInitialize = function(self)
end

TileMovementSystem.OnEntityAdded = function(self, entityId)
	print("OnEntityAdded (LUA)")
end

TileMovementSystem.OnEntityRemoved = function(self, entityId)
	print("OnEntityRemoved")
end