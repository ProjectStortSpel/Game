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
			local t1, t2, t3 = self:AlgorithmLerp(t, algorithm)
			
			local X = sX + (tX - sX) * t1
			local Y = sY + (tY - sY) * t2
			local Z = sZ + (tZ - sZ) * t3
			
			position:SetFloat3(X, Y, Z, false)
			
			world:GetComponent(entity, "LerpingPosition", "Timer"):SetFloat(_timer, false)
		else
			position:SetFloat3(tX, tY, tZ, false)
			if world:GetComponent(entity, "LerpingPosition", "KillWhenFinished"):GetBool() then
				world:KillEntity(entity)
			else
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