ClientAbilityStoneSystem = System()
ClientAbilityStoneSystem.Timers = {}
ClientAbilityStoneSystem.Timers.__mode = "k"

ClientAbilityStoneSystem.Initialize = function ( self )
	--	Set Name
	self:SetName("ClientAbilityStoneSystem")
	self:UsingEntitiesAdded()
	self:UsingEntitiesRemoved()
	
	--	Filters
	self:AddComponentTypeToFilter("StoneTimer", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("TempTimer", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("MoveRiver", FilterType.RequiresOneOf)
end

ClientAbilityStoneSystem.AddTextTimer = function(self, entity, timeCounter)
	
	local lifeSpan = 0
	if timeCounter == nil then
		lifeSpan = world:GetComponent(entity, "StoneTimer", "LifeSpan"):GetInt()
		print("AddTextTimer: " .. tostring(lifeSpan))
	else
		lifeSpan = timeCounter
	end
	
	local id = world:CreateNewEntity()
	world:CreateComponentAndAddTo("Model", id)
	world:CreateComponentAndAddTo("Parent", id)
	world:CreateComponentAndAddTo("KillWhenOrphan", id)
	world:CreateComponentAndAddTo("Position", id)
	world:CreateComponentAndAddTo("Rotation", id)
	world:CreateComponentAndAddTo("Scale", id)
	world:CreateComponentAndAddTo("TempTimer", id)
	
	world:GetComponent(id, "Model", 0):SetModel("center", "text", 1)
	world:GetComponent(id, "Parent", 0):SetInt(entity)
	world:GetComponent(id, "Position", 0):SetFloat3(0.0, 0.25, 0.0001)
	world:GetComponent(id, "Rotation", 0):SetFloat3(0.0, 0.0, 0.0)
	world:GetComponent(id, "Scale", 0):SetFloat3(0.960, 0.480, 1.0)
	
	world:CreateComponentAndAddTo("TextTexture", id)
	world:GetComponent(id, "TextTexture", "Name"):SetText(entity + lifeSpan)
	world:GetComponent(id, "TextTexture", "Text"):SetText(tostring(lifeSpan))
	world:GetComponent(id, "TextTexture", "FontIndex"):SetInt(0)
	world:GetComponent(id, "TextTexture", "R"):SetFloat(1)
	world:GetComponent(id, "TextTexture", "G"):SetFloat(1)
	world:GetComponent(id, "TextTexture", "B"):SetFloat(1)
end

ClientAbilityStoneSystem.DecreaseLifeSpan = function(self)

	local tempTimers = self:GetEntities("TempTimer")
	
	for i = 1, #tempTimers do
	
		local parentId = world:GetComponent(tempTimers[i], "Parent", 0):GetInt()
		local timeCounter = world:GetComponent(tempTimers[i], "TextTexture", "Text"):GetText()
		local timeNr = tonumber(timeCounter) - 1
		print("TimeNr: " .. tostring(timeNr))
		world:KillEntity(tempTimers[i])
		
		self:AddTextTimer(parentId, timeNr)
		
	
	end
	
	

end

ClientAbilityStoneSystem.EntitiesAdded = function(self, dt, entities)

	for n = 1, #entities do
		local entity = entities[n]
		
		if world:EntityHasComponent(entity, "MoveRiver") then
			self:DecreaseLifeSpan()
		end
		
		if world:EntityHasComponent(entity, "StoneTimer") then
			self:AddTextTimer(entity)
		end	
	end
end

ClientAbilityStoneSystem.EntitiesRemoved = function(self, dt, entities)

	local tempTimers = self:GetEntities("TempTimer")

	for n = 1, #entities do
	
		for i = 1, #tempTimers do
			
			local parentId = world:GetComponent(tempTimers[i], "Parent", 0):GetInt()
			if entities[n] == parentId then
				world:KillEntity(tempTimers[i])
			end
			
		end
	
	end
end
