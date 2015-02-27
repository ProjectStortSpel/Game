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


UpdateLerpRotationSystem.Update = function(self, dt)
	local entities = self:GetEntities()
	for i = 1, #entities do
		local entity = entities[i]
		
		local rotation = world:GetComponent(entity, "Rotation", 0)
		
		local _time, _timer, sX, sY, sZ, tX, tY, tZ = world:GetComponent(entity, "LerpingRotation", "Time"):GetFloat8(0)
		local algorithm = world:GetComponent(entity, "LerpingRotation", "Algorithm"):GetText(0)

		_timer = _timer + dt
		if _time > _timer then
			local t = _timer / _time
			t = self:AlgorithmLerp(t, algorithm)
			
			local X = sX + (tX - sX) * t
			local Y = sY + (tY - sY) * t
			local Z = sZ + (tZ - sZ) * t
			
			world:GetComponent(entity, "Rotation", 0):SetFloat3(X, Y, Z, false)
			world:GetComponent(entity, "LerpingRotation", "Timer"):SetFloat(_timer, false)
		else
			world:GetComponent(entity, "Rotation", 0):SetFloat3(tX, tY, tZ, false)
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
	elseif algorithm == "ExponentialLerp" then
		return t * t
	else
		return t
	end
end