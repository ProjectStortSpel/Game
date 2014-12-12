DiscoSystem = System()

DiscoSystem.Update = function(self, dt)

	local entities = self:GetEntities() 
	for i = 1, #entities do
		local entity = entities[i]

		local Pointlight = self:GetComponent(entity, "Pointlight", 0)
		local velocity = self:GetComponent(entity, "Velocity", 0)

		local px, py, pz, inta, intd, ints, colorR, colorG, colorB, range = Pointlight:GetPointlight()
		local vx, vy, vz = velocity:GetFloat3()

		px = px + vx * dt
		py = py + vy * dt
		pz = pz + vz * dt

		Pointlight:SetPointlight(px, py, pz, inta, intd, ints, colorR, colorG, colorB, range)
	end
end

DiscoSystem.Initialize = function(self)
	self:AddComponentTypeToFilter("Pointlight", FilterType.Mandatory)
	self:AddComponentTypeToFilter("Velocity", FilterType.Mandatory)
	
	print("Disco System initialized!")
end

DiscoSystem.PostInitialize = function(self)

end

DiscoSystem.OnEntityAdded = function(self, entityId)
end

DiscoSystem.OnEntityRemoved = function(self, entityId)
end