DiscoSystem = System()
local TotalTime = 0

DiscoSystem.Update = function(self, dt)

	if GameRunning == false then
		return;
	end

	TotalTime = TotalTime + dt
	
	local entities = self:GetEntities() 
	for i = 1, #entities do
		local entity = entities[i]

		local Pointlight = self:GetComponent(entity, "Pointlight", 0)
		local RotateAround = self:GetComponent(entity, "RotateAround", 0)

		local px, py, pz, inta, intd, ints, colorR, colorG, colorB, range = Pointlight:GetPointlight()
		local rx, ry, rz, radius, factor = RotateAround:GetFloat5()
		
		px = rx + radius * math.sin((TotalTime + ry) * factor) 
		py = py
		pz = rz + radius * math.cos((TotalTime + ry) * factor)
		
		Pointlight:SetPointlight(px, py, pz, inta, intd, ints, colorR, colorG, colorB, range)
	end
end

DiscoSystem.Initialize = function(self)
	self:SetName("Disco System")

	self:AddComponentTypeToFilter("Pointlight", FilterType.Mandatory)
	self:AddComponentTypeToFilter("RotateAround", FilterType.Mandatory)
	
	print("Disco System initialized!")
end

DiscoSystem.PostInitialize = function(self)

end

DiscoSystem.OnEntityAdded = function(self, entityId)

	if GameRunning == false then
		return;
	end

	local RotateAround = self:GetComponent(entity, "RotateAround", 0)
	local rx, ry, rz, radius, factor = RotateAround:GetFloat5()
	
	ry = math.random()*100
	print("Random: ", ry)
	factor = 4
	RotateAround:SetFloat5(rx, ry, rz, radius, factor)
end

DiscoSystem.OnEntityRemoved = function(self, entityId)

	if GameRunning == false then
		return;
	end

end