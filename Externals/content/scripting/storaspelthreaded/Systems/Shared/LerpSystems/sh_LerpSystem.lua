LerpSystem = System()
LerpSystem.LerpTime = 0.5

LerpSystem.Initialize = function(self)

	--	Set Name
	self:SetName("LerpSystem")
	
	--	Toggle EntitiesAdded
	self:UsingUpdate()
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter("Position", FilterType.Mandatory)
	self:AddComponentTypeToFilter("LerpTargetPosition", FilterType.Mandatory)
end


LerpSystem.Update = function(self, dt, taskIndex, taskCount)
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

			local t = _timer / _time
			
			-- smoothstep for nice flow
			t = t * t * t * (t * (6*t - 15) + 10)
			
			local X = sX + (tX - sX) * t
			local Y = sY + (tY - sY) * t
			local Z = sZ + (tZ - sZ) * t
			
			position:SetFloat3(X, Y, Z)
			
			timer:SetFloat2(_time, _timer)
			
		else
		
			position:SetFloat3(tX, tY, tZ)

			-- TODO: IF SOMETHING GETS REMOVED AT THE SAME FRAME AS IT GETS ADDED IT WILL BE REMOVED
			world:RemoveComponentFrom("LerpTime", entity)
			world:RemoveComponentFrom("LerpTargetPosition", entity)
			world:RemoveComponentFrom("LerpStartPosition", entity)
		
		end
		
		
	end
end


LerpSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

	for n = 1, #entities do
		local entityId = entities[n]
		
		if not world:EntityHasComponent(entityId, "LerpStartPosition") then
			world:CreateComponentAndAddTo("LerpStartPosition", entityId)
		end
		
		local position = self:GetComponent(entityId, "Position", 0)
		local X, Y, Z = position:GetFloat3(0)

		local startposition = self:GetComponent(entityId, "LerpStartPosition", 0)
		startposition:SetFloat3(X, Y, Z)

		if not world:EntityHasComponent(entityId, "LerpTime") then
			world:CreateComponentAndAddTo("LerpTime", entityId)
			local timer = self:GetComponent(entityId, "LerpTime", 0)
			timer:SetFloat2(self.LerpTime, 0)
		end

		local target = self:GetComponent(entityId, "LerpTargetPosition", 0)
		local tX, tY, tZ = target:GetFloat3()
		
		print("LerpStartPosition")
		print("X: " .. X)
		print("Y: " .. Y)
		print("Z: " .. Z)
		
		print("LerpTargetPosition")
		print("X: " .. tX)
		print("Y: " .. tY)
		print("Z: " .. tZ)
		
	end
end