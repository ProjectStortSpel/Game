GlowNextCheckpointSystem = System()
GlowNextCheckpointSystem.CurrentGlowing	=	-1
GlowNextCheckpointSystem.TotalTime		=	-1

GlowNextCheckpointSystem.Initialize = function(self)
	--	Set Name
	self:SetName("GlowNextCheckpointSystem")
	
	--	Toggle EntitiesAdded
	self:UsingUpdate()
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

GlowNextCheckpointSystem.Update = function(self, dt)
	
	if self.CurrentGlowing == -1 then
		return
	end
	self.TotalTime	=	self.TotalTime + dt
	
	local	R	=	0.75 + math.sin(2.5*self.TotalTime)*0.25
	local	G	=	0.75 + math.sin(2.5*self.TotalTime)*0.25
	local	B	=	0.75 + math.sin(2.5*self.TotalTime)*0.25
	world:GetComponent(self.CurrentGlowing, "Color", 0):SetFloat3(R,G,B)

end

GlowNextCheckpointSystem.EntitiesAdded = function(self, dt, newEntities)
	
	local	tempCurrentGlow	=	-1
	local	tempHighest		=	-1
	for nEntity = 1, #newEntities do
	
		local	newEntity	=	newEntities[nEntity]
		if world:EntityHasComponent(newEntity, "GlowCheckpoint") then
			
			local	cNumber		=	world:GetComponent(newEntity, "GlowCheckpoint", "Stage"):GetInt()
			local	tempRune	=	self:GetGlowingRune(cNumber)
			
			if tempRune ~= -1 then
			
				world:GetComponent(tempRune, "Color", 0):SetFloat3(0.5, 0.5, 0.5)
				if tempCurrentGlow == -1 then
					tempCurrentGlow	=	tempRune
				end
				
				if cNumber > tempHighest then
					tempHighest	=	cNumber
				end
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
			
			world:GetComponent(newGlow, "Model", 0):SetModel("rune5", "runes", 1)
			world:GetComponent(newGlow, "Position", 0):SetFloat3(X, 0.08, Z)
			world:GetComponent(newGlow, "Rotation", 0):SetFloat3(0, 0, 0)--math.pi/4 * math.random(1, 10), 0)
			world:GetComponent(newGlow, "Scale", 0):SetFloat3(0.5, 1.0, 0.5)
			world:GetComponent(newGlow, "Color", 0):SetFloat3(0.25, 0.25, 0.25)
			world:GetComponent(newGlow, "GlowingCheckpoint", "Stage"):SetInt(cNumber)
			
			print("ADDED newGlow with id " .. newGlow .. " with number " .. cNumber)
		end
		
	end
	
	if tempCurrentGlow ~= -1 then
		if self.CurrentGlowing ~= -1 then
			world:GetComponent(self.CurrentGlowing, "Color", 0):SetFloat3(0.05,0.05,0.05)
		end
		self.CurrentGlowing	=	tempCurrentGlow
	end
	
end
