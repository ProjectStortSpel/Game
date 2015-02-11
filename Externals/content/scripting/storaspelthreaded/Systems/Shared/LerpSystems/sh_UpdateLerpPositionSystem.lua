UpdateLerpPositionSystem = System()

UpdateLerpPositionSystem.Initialize = function(self)
	--	Set Name
	self:SetName("UpdateLerpPositionSystem")
	
	--	Toggle EntitiesAdded
	self:UsingUpdate()

	--	Set Filter
	self:AddComponentTypeToFilter("Position", FilterType.Mandatory)
	self:AddComponentTypeToFilter("LerpingPosition", FilterType.Mandatory)
end


UpdateLerpPositionSystem.Update = function(self, dt, taskIndex, taskCount)
	local entities = self:GetEntities()
	for i = 1, #entities do
		local entity = entities[i]
		
		local position = world:GetComponent(entity, "Position", 0)
		
		local sX = world:GetComponent(entity, "LerpingPosition", "sX"):GetFloat(0)
		local sY = world:GetComponent(entity, "LerpingPosition", "sY"):GetFloat(0)
		local sZ = world:GetComponent(entity, "LerpingPosition", "sZ"):GetFloat(0)
		local tX = world:GetComponent(entity, "LerpingPosition", "tX"):GetFloat(0)
		local tY = world:GetComponent(entity, "LerpingPosition", "tY"):GetFloat(0)
		local tZ = world:GetComponent(entity, "LerpingPosition", "tZ"):GetFloat(0)
		local _time = world:GetComponent(entity, "LerpingPosition", "Time"):GetFloat(0)
		local _timer = world:GetComponent(entity, "LerpingPosition", "Timer"):GetFloat(0)
		local algorithm = world:GetComponent(entity, "LerpingPosition", "Algorithm"):GetText(0)

		_timer = _timer + dt
		if _time > _timer then
			local t = _timer / _time
			t = self:AlgorithmLerp(t, algorithm)
			
			local X = sX + (tX - sX) * t
			local Y = sY + (tY - sY) * t
			local Z = sZ + (tZ - sZ) * t
			
			position:SetFloat3(X, Y, Z, false)
			
			world:GetComponent(entity, "LerpingPosition", "Timer"):SetFloat(_timer, false)
		else
			position:SetFloat3(tX, tY, tZ, false)
			world:RemoveComponentFrom("LerpingPosition", entity)
		end	
	end
end

UpdateLerpPositionSystem.AlgorithmLerp = function(self, t, algorithm)
	if algorithm == "NormalLerp" then
		return t
	elseif algorithm == "SmoothLerp" then
		return t * t * (3 - 2*t)
	elseif algorithm == "SmootherLerp" then
		return t * t * t * (t * (6*t - 15) + 10)
	elseif algorithm == "ExponentialLerp" then
		return t * t
	elseif algorithm == "OvershotLerp" then
		return t * t * (7 - 6*t)
	else
		return t
	end
end