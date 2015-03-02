UpdateLerpScaleSystem = System()

UpdateLerpScaleSystem.Initialize = function(self)
	--	Set Name
	self:SetName("UpdateLerpScaleSystem")
	
	--	Toggle EntitiesAdded
	self:UsingUpdate()

	--	Set Filter
	self:AddComponentTypeToFilter("Scale", FilterType.Mandatory)
	self:AddComponentTypeToFilter("LerpingScale", FilterType.Mandatory)
end


UpdateLerpScaleSystem.Update = function(self, dt)
	local entities = self:GetEntities()
	for i = 1, #entities do
		local entity = entities[i]
		
		local _time, _timer, sX, sY, sZ, tX, tY, tZ = world:GetComponent(entity, "LerpingScale", "Time"):GetFloat8(0)
		local algorithm = world:GetComponent(entity, "LerpingScale", "Algorithm"):GetText(0)

		_timer = _timer + dt
		if _time > _timer then
			local t = _timer / _time
			t = self:AlgorithmLerp(t, algorithm)
			
			local X = sX + (tX - sX) * t
			local Y = sY + (tY - sY) * t
			local Z = sZ + (tZ - sZ) * t
			
			world:GetComponent(entity, "Scale", 0):SetFloat3(X, Y, Z, false)
			world:GetComponent(entity, "LerpingScale", "Timer"):SetFloat(_timer, false)
		else
			world:GetComponent(entity, "Scale", 0):SetFloat3(tX, tY, tZ, false)
			if world:GetComponent(entity, "LerpingScale", "KillWhenFinished"):GetBool() then
				world:KillEntity(entity)
			else
				world:RemoveComponentFrom("LerpingScale", entity)
			end
		end	
	end
end

UpdateLerpScaleSystem.AlgorithmLerp = function(self, t, algorithm)
	if algorithm == "NormalLerp" then
		return t
	elseif algorithm == "SmoothLerp" then
		return t * t * (3 - 2*t)
	elseif algorithm == "SmootherLerp" then
		return t * t * t * (t * (6*t - 15) + 10)
	elseif algorithm == "OvershotLerp" then
		local f = t * t * t * (8 - 7*t)
		return f, f, f
	elseif algorithm == "ExponentialLerp" then
		return t * t
	else
		return t
	end
end