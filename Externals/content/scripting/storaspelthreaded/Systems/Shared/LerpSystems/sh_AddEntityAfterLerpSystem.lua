AddEntityAfterLerpSystem = System()

AddEntityAfterLerpSystem.Initialize = function(self)
	--	Set Name
	self:SetName("AddEntityAfterLerpSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesRemoved()
	
	--	Set Filter
	self:AddComponentTypeToFilter("AddEntityAfterLerp", FilterType.Mandatory)
	self:AddComponentTypeToFilter("LerpingPosition", FilterType.Mandatory)
end

AddEntityAfterLerpSystem.BulletImpact = function(self, entity, hitPlayer)
	
	local x, y, z = world:GetComponent(entity, "Position", 0):GetFloat3()
	local id = -1
	
	if hitPlayer then
		id = Net.StartPack("SERVER_BULLET_IMPACT_PLAYER_PARTICLE")
	else
		id = Net.StartPack("SERVER_BULLET_IMPACT_PARTICLE")
	end
	
	Net.WriteFloat(id, x)
	Net.WriteFloat(id, y)
	Net.WriteFloat(id, z)
	Net.Broadcast(id)

	-- SOUND
	local audioId = Net.StartPack("Client.PlaySound")
	Net.WriteString(audioId, "SmallStoneImpact" .. math.random(1, 2))
	Net.WriteBool(audioId, false)
	Net.Broadcast(audioId)
	
	if hitPlayer then
		audioId = Net.StartPack("Client.PlaySound")
		Net.WriteString(audioId, "HitByStone" .. math.random(1, 2))
		Net.WriteBool(audioId, false)
		Net.Broadcast(audioId)
	end
	
end

AddEntityAfterLerpSystem.EntitiesRemoved = function(self, dt, entities)
	for i = 1, #entities do
		local entity = entities[i]
		
		local compName = world:GetComponent(entity, "AddEntityAfterLerp", "ComponentName"):GetText()
		
		if compName == "AddStoneImpact" then
			
			local parentId = world:GetComponent(entity, "Parent", "EntityId"):GetInt()
			local x, y, z = world:GetComponent(parentId, "Position", 0):GetFloat3()
			
			local id = Net.StartPack("SERVER_STONE_IMPACT_PARTICLE")
			Net.WriteFloat(id, x)
			Net.WriteFloat(id, y)
			Net.WriteFloat(id, z)
			Net.Broadcast(id)
			
			local audioId = Net.StartPack("Client.PlaySound")
			Net.WriteString(audioId, "BigStoneImpact")
			Net.WriteBool(audioId, false)
			Net.Broadcast(audioId)
				
		elseif compName == "AddBulletImpact" then
			self:BulletImpact(entity, false)
		
		elseif compName == "AddBulletImpactPlayer" then
			self:BulletImpact(entity, true)
		elseif compName == "FallDownSound" then
		
			local x, y, z = world:GetComponent(entity, "Position", 0):GetFloat3()
			local newEntity = world:CreateNewEntity()
			world:CreateComponentAndAddTo("FallDownSound", newEntity)
			world:GetComponent(newEntity, "FallDownSound", 0):SetFloat3(x, y, z)
			
		end
		
		if world:EntityHasComponent(entity, "AddEntityAfterLerp") then
			world:RemoveComponentFrom("AddEntityAfterLerp", entity)
		end
		
	end
end