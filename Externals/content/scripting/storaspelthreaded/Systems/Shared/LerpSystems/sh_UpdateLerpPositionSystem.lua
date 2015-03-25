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


UpdateLerpPositionSystem.Update = function(self, dt)
	local entities = self:GetEntities()
	
	if #entities > 0 then
		local lerpingPositions = world:GetComponents(entities, "LerpingPosition", "Time")
		local algorithms = world:GetComponents(entities, "LerpingPosition", "Algorithm")
		local timers = world:GetComponents(entities, "LerpingPosition", "Timer")
		local positions = world:GetComponents(entities, "Position", 0)
	
		for i = 1, #entities do
			local entity = entities[i]
			
			local _time, _timer, sX, sY, sZ, tX, tY, tZ = lerpingPositions[i]:GetFloat8(0)

			_timer = _timer + dt
			if _time > _timer then
				local t = _timer / _time
				local t1, t2, t3 = self:AlgorithmLerp(t, algorithms[i]:GetText(0))
				
				local X = sX + (tX - sX) * t1
				local Y = sY + (tY - sY) * t2
				local Z = sZ + (tZ - sZ) * t3
				
				positions[i]:SetFloat3(X, Y, Z, false)
				timers[i]:SetFloat(_timer, false)
			else
				positions[i]:SetFloat3(tX, tY, tZ)
				world:RemoveComponentFrom("LerpingPosition", entity)
			end	
		end
	end
end

UpdateLerpPositionSystem.AlgorithmLerp = function(self, t, algorithm)
	if algorithm == "NormalLerp" then
		return t, t, t
	elseif algorithm == "SmoothLerp" then
		local f = t * t * (3 - 2*t)
		return f, f, f
	elseif algorithm == "SmootherLerp" then
		local f = t * t * t * (t * (6*t - 15) + 10)
		return f, f, f
	elseif algorithm == "ExponentialLerp" then
		local f = t * t
		return f, f, f
	elseif algorithm == "OvershotLerp" then
		local f = t * t * (7 - 6*t)
		return f, f, f
	elseif algorithm == "EnterRiver" then
		local f = t * t * (3 - 2*t)
		local g = 0
		if (t > 0.5) then
			g = 4 * (t-0.5) * (t-0.5) * (3 - 4*(t-0.5))
		end
		return f, g, f
	elseif algorithm == "ExitRiver" then
		local f = t * t * (3 - 2*t)
		local g = 1
		if (t < 0.5) then
			g = 4 * t * t * (3 - 4*t)
		end
		return f, g, f
	else
		return t, t, t
	end
end