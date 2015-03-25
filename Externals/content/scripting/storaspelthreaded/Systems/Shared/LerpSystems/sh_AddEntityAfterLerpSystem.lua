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
		
		local unitId = world:GetComponent(entity, "TargetUnit", "Unit"):GetInt()
	
		if not world:EntityHasComponent(unitId, "HasStunnedIndicator") or not world:EntityHasComponent(unitId, "ActionGuard") then
			local newEntity = world:CreateNewEntity()
			world:CreateComponentAndAddTo("AddStunnedIndicator", newEntity)
			world:GetComponent(newEntity, "AddStunnedIndicator", 0):SetInt(unitId)
		end
	
	end
	
	id = Net.StartPack("SERVER_BULLET_IMPACT_PARTICLE")
	Net.WriteFloat(id, x)
	Net.WriteFloat(id, y)
	Net.WriteFloat(id, z)
	Net.Broadcast(id)

	-- SOUND
	local audioId = Net.StartPack("Client.PlaySoundC")
	Net.WriteString(audioId, "SmallStoneImpact" .. math.random(1, 2))
	Net.WriteString(audioId, "SmallStoneImpact" .. entity)
	Net.WriteBool(audioId, false)
	Net.Broadcast(audioId)
	audioId = Net.StartPack("Client.SetSoundPosition")
	Net.WriteString(audioId, "SmallStoneImpact" .. entity)
	Net.WriteFloat(audioId, x)
	Net.WriteFloat(audioId, y)
	Net.WriteFloat(audioId, z)
	Net.Broadcast(audioId)
	audioId = Net.StartPack("Client.SetSoundVolume")
	Net.WriteString(audioId, "SmallStoneImpact" .. entity)
	Net.WriteInt(audioId, 5)
	Net.Broadcast(audioId)
	
	if hitPlayer then
		audioId = Net.StartPack("Client.PlaySoundC")
		Net.WriteString(audioId, "HitByStone" .. math.random(1, 2))
		Net.WriteString(audioId, "HitByStone" .. entity)
		Net.WriteBool(audioId, false)
		Net.Broadcast(audioId)
		audioId = Net.StartPack("Client.SetSoundPosition")
		Net.WriteString(audioId, "HitByStone" .. entity)
		Net.WriteFloat(audioId, x)
		Net.WriteFloat(audioId, y)
		Net.WriteFloat(audioId, z)
		Net.Broadcast(audioId)
		audioId = Net.StartPack("Client.SetSoundVolume")
		Net.WriteString(audioId, "HitByStone" .. entity)
		Net.WriteInt(audioId, 8)
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
			
			local audioId = Net.StartPack("Client.PlaySoundC")
			Net.WriteString(audioId, "BigStoneImpact")
			Net.WriteString(audioId, "BigStoneImpact" .. entity)
			Net.WriteBool(audioId, false)
			Net.Broadcast(audioId)
			audioId = Net.StartPack("Client.SetSoundPosition")
			Net.WriteString(audioId, "BigStoneImpact" .. entity)
			Net.WriteFloat(audioId, x)
			Net.WriteFloat(audioId, y)
			Net.WriteFloat(audioId, z)
			Net.Broadcast(audioId)
			audioId = Net.StartPack("Client.SetSoundVolume")
			Net.WriteString(audioId, "BigStoneImpact" .. entity)
			Net.WriteInt(audioId, 60)
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