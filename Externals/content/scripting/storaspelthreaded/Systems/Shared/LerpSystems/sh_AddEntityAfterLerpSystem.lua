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
				
		elseif compName == "AddBulletImpact" then
		
			local x, y, z = world:GetComponent(entity, "Position", 0):GetFloat3()
			
			local id = Net.StartPack("SERVER_BULLET_IMPACT_PARTICLE")
			Net.WriteFloat(id, x)
			Net.WriteFloat(id, y)
			Net.WriteFloat(id, z)
			Net.Broadcast(id)
		
		end
		
	end
end