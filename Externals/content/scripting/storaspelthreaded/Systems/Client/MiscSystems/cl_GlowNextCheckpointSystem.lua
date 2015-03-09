GlowNextCheckpointSystem = System()

GlowNextCheckpointSystem.Initialize = function(self)
	--	Set Name
	self:SetName("GlowNextCheckpointSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	
	--	Filters
	self:AddComponentTypeToFilter("Checkpoint",			FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("GlowCheckpoint",		FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("GlowingCheckpoint",	FilterType.RequiresOneOf)
end

GlowNextCheckpointSystem.GetGlowingRune = function(self, RuneNumber)

	local	allRunes	=	self:GetEntities("GlowingCheckpoint")
	for tRune = 1, #allRunes do
		if world:GetComponent(allRunes[tRune], "GlowingCheckpoint", "Stage"):GetInt() == RuneNumber then
			return allRunes[tRune]
		end
	end
	
	return	-1
end

GlowNextCheckpointSystem.EntitiesAdded = function(self, dt, newEntities)
	
	for nEntity = 1, #newEntities do
	
		local	newEntity	=	newEntities[nEntity]
		if world:EntityHasComponent(newEntity, "GlowCheckpoint") then
			
			local	cNumber		=	world:GetComponent(newEntity, "GlowCheckpoint", "Stage"):GetInt()
			local	tempRune	=	self:GetGlowingRune(cNumber)
			
			if tempRune ~= -1 then
			
				world:GetComponent(tempRune, "Color", 0):SetFloat3(0.2*cNumber, 0.2*cNumber, 0.2*cNumber)
				
			end
			
			world:KillEntity(newEntity)
		end
		
		if world:EntityHasComponent(newEntity, "Checkpoint") then
			local	X, Z	=	world:GetComponent(newEntity, "MapPosition", "X"):GetInt2()
			local	cNumber	=	world:GetComponent(newEntity, "Checkpoint", "Number"):GetInt()
			
			local	newGlow	=	world:CreateNewEntity()
			world:CreateComponentAndAddTo("Position", newGlow)
			world:CreateComponentAndAddTo("Rotation", newGlow)
			world:CreateComponentAndAddTo("Scale", newGlow)
			world:CreateComponentAndAddTo("Model", newGlow)
			world:CreateComponentAndAddTo("Color", newGlow)
			world:CreateComponentAndAddTo("GlowingCheckpoint", newGlow)
			
			world:GetComponent(newGlow, "Model", 0):SetModel("rune1", "runes", 1)
			world:GetComponent(newGlow, "Position", 0):SetFloat3(X, 0.08, Z)
			world:GetComponent(newGlow, "Rotation", 0):SetFloat3(0, 0, 0)--math.pi/4 * math.random(1, 10), 0)
			world:GetComponent(newGlow, "Scale", 0):SetFloat3(0.5, 1.0, 0.5)
			world:GetComponent(newGlow, "Color", 0):SetFloat3(0.1, 0.1, 0.1)
			world:GetComponent(newGlow, "GlowingCheckpoint", "Stage"):SetInt(cNumber)
			
			print("ADDED newGlow with id " .. newGlow .. " with number " .. cNumber)
		end
		
	end

end
