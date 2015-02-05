UpdateLerpRotationSystem = System()

UpdateLerpRotationSystem.Initialize = function(self)
	--	Set Name
	self:SetName("UpdateLerpRotationSystem")
	
	--	Toggle EntitiesAdded
	self:UsingUpdate()

	--	Set Filter
	self:AddComponentTypeToFilter("Rotation", FilterType.Mandatory)
	self:AddComponentTypeToFilter("LerpingRotation", FilterType.Mandatory)
end


UpdateLerpRotationSystem.Update = function(self, dt, taskIndex, taskCount)
	local entities = self:GetEntities()
	for i = 1, #entities do
		local entity = entities[i]
		
		local rotation = self:GetComponent(entity, "Rotation", 0)
		
		local sX = world:GetComponent(entity, "LerpingRotation", "sX"):GetFloat(0)
		local sY = world:GetComponent(entity, "LerpingRotation", "sY"):GetFloat(0)
		local sZ = world:GetComponent(entity, "LerpingRotation", "sZ"):GetFloat(0)
		local tX = world:GetComponent(entity, "LerpingRotation", "tX"):GetFloat(0)
		local tY = world:GetComponent(entity, "LerpingRotation", "tY"):GetFloat(0)
		local tZ = world:GetComponent(entity, "LerpingRotation", "tZ"):GetFloat(0)
		local _time = world:GetComponent(entity, "LerpingRotation", "Time"):GetFloat(0)
		local _timer = world:GetComponent(entity, "LerpingRotation", "Timer"):GetFloat(0)
		local algorithm = world:GetComponent(entity, "LerpingRotation", "Algorithm"):GetString(0)

		_timer = _timer + dt
		if _time > _timer then
			local t = _timer / _time
			t = self:AlgorithmLerp(t, algorithm)
			
			local X = sX + (tX - sX) * t
			local Y = sY + (tY - sY) * t
			local Z = sZ + (tZ - sZ) * t
			
			rotation:SetFloat3(X, Y, Z, false)
			
			world:GetComponent(entity, "LerpingRotation", "Timer"):SetFloat(_timer, false)
		else
			rotation:SetFloat3(tX, tY, tZ, false)
			world:RemoveComponentFrom("LerpingRotation", entity)
		end	
	end
end

UpdateLerpRotationSystem.AlgorithmLerp = function(self, t, algorithm)
	if algorithm == "NormalLerp" then
		return t
	elseif algorithm == "SmoothLerp" then
		return t * t * (3 - 2*t)
	elseif algorithm == "SmootherLerp" then
		return t * t * t * (t * (6*t - 15) + 10)
	else
		return t
	end
end