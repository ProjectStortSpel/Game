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
	
	if #entities > 0 then
		local lerpingRotations = world:GetComponents(entities, "LerpingRotation", "Time")
		local algorithms = world:GetComponents(entities, "LerpingRotation", "Algorithm")
		local timers = world:GetComponents(entities, "LerpingRotation", "Timer")
		local rotations = world:GetComponents(entities, "Rotation", 0)
		local killWhenFinished = world:GetComponents(entities, "LerpingRotation", "KillWhenFinished")
	
		for i = 1, #entities do
			local entity = entities[i]
			
			local _time, _timer, sX, sY, sZ, tX, tY, tZ = lerpingRotations[i]:GetFloat8(0)

			_timer = _timer + dt
			if _time > _timer then
				local t = _timer / _time
				t = self:AlgorithmLerp(t, algorithms[i]:GetText(0))
				
				local X = sX + (tX - sX) * t
				local Y = sY + (tY - sY) * t
				local Z = sZ + (tZ - sZ) * t
				
				rotations[i]:SetFloat3(X, Y, Z, false)
				timers[i]:SetFloat(_timer, false)
			else
				rotations[i]:SetFloat3(tX, tY, tZ, false)
				if killWhenFinished[i]:GetBool() then
					world:KillEntity(entity)
				else
					world:RemoveComponentFrom("LerpingRotation", entity)
				end
			end	
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