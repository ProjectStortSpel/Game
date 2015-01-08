MovementSystem = System()

MovementSystem.Update = function(self, dt)

	if GameRunning == false then
		return;
	end
	
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

MovementSystem.Initialize = function(self)
	self:SetName("Movement System")
	self:AddComponentTypeToFilter("Position", FilterType.Mandatory)
	self:AddComponentTypeToFilter("Velocity", FilterType.Mandatory)
	
	print("MovementSystem initialized!")
end

MovementSystem.PostInitialize = function(self)

end

MovementSystem.OnEntityAdded = function(self, entityId)

	if GameRunning == false then
		return;
	end

	print("OnEntityAdded (LUA)")
end

MovementSystem.OnEntityRemoved = function(self, entityId)

	if GameRunning == false then
		return;
	end

	print("OnEntityRemoved")
end