LerpSystem = System()
LerpSystem.LerpTime = 0.2

LerpSystem.Update = function(self, dt)
	local entities = self:GetEntities("LerpTime")
	for i = 1, #entities do
		local entity = entities[i]
		
		local position = self:GetComponent(entity, "Position", 0)
		local timer = self:GetComponent(entity, "LerpTime", 0)
		local _time, _timer = timer:GetFloat2(0)
				
		local targetpos = self:GetComponent(entity, "LerpTargetPosition", 0)
		local tX, tY, tZ = targetpos:GetFloat3(0)
		
		_timer = _timer + dt
		if _time > _timer then

			local startpos = self:GetComponent(entity, "LerpStartPosition", 0)
			local sX, sY, sZ = startpos:GetFloat3(0)

			local X = sX + (tX - sX) * _timer / _time
			local Y = sY + (tY - sY) * _timer / _time
			local Z = sZ + (tZ - sZ) * _timer / _time
			
			position:SetFloat3(X, Y, Z)
			
			timer:SetFloat2(_time, _timer + dt)
			
		else
		
			position:SetFloat3(tX, tY, tZ)

			world:RemoveComponentFrom("LerpTime", entity)
			world:RemoveComponentFrom("LerpTargetPosition", entity)
			world:RemoveComponentFrom("LerpStartPosition", entity)
		
		end
		
		
	end
end

LerpSystem.Initialize = function(self)
	self:AddComponentTypeToFilter("Position", FilterType.Mandatory)
	self:AddComponentTypeToFilter("LerpTargetPosition", FilterType.Mandatory)
end

LerpSystem.OnEntityAdded = function(self, entityId)

	if not world:EntityHasComponent(entityId, "LerpStartPosition") then
		world:CreateComponentAndAddTo("LerpStartPosition", entityId)
	end
	
	local position = self:GetComponent(entityId, "Position", 0)
	local X, Y, Z = position:GetFloat3(0)

	local startposition = self:GetComponent(entityId, "LerpStartPosition", 0)
	startposition:SetFloat3(X, Y, Z)

	if not world:EntityHasComponent(entityId, "LerpTime") then
		world:CreateComponentAndAddTo("LerpTime", entityId)
	end
	
	local timer = self:GetComponent(entityId, "LerpTime", 0)
	timer:SetFloat2(self.LerpTime, 0)
	
end