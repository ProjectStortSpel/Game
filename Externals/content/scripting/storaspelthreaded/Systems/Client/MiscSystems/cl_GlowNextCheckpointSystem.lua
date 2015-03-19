GlowNextCheckpointSystem = System()
GlowNextCheckpointSystem.CurrentGlowing		=	-1
GlowNextCheckpointSystem.TotalTime			=	-1
GlowNextCheckpointSystem.CurrentPointlight	=	-1
GlowNextCheckpointSystem.FlameTimer	=	0.0
GlowNextCheckpointSystem.FlameLimit	=	1/20

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

GlowNextCheckpointSystem.GlowPointlight = function(self)

	if self.CurrentPointlight == -1 then
		return
	end

	if not world:EntityHasComponent(self.CurrentPointlight, "Pointlight") then
		self.CurrentPointlight	=	-1
		local	allRunes	=	self:GetEntities("GlowingCheckpoint")
		for tRune = 1, #allRunes do
			if world:EntityHasComponent(allRunes[tRune], "Pointlight") then
				self.CurrentPointlight	=	allRunes[tRune]
			end
		end
		
		if self.CurrentPointlight == -1 then
			return
		end
	end

	
	local	X, Y, Z, AMBIENT, DIFFUSE, SPECULAR, R, G, B, RANGE	=	world:GetComponent(self.CurrentPointlight, "Pointlight", 0):GetPointlight()
	local	AMBIENT		=	0.25 + math.sin(1.5*self.TotalTime)*0.5*0.5
	local	DIFFUSE		=	0.4 + math.sin(1.5*self.TotalTime)*0.8*0.5
	local	SPECULAR	=	0.35 + math.sin(1.5*self.TotalTime)*0.7*0.5
	local	COLOR		=	0.75 + math.sin(1.5*self.TotalTime)*0.15
	
	world:GetComponent(self.CurrentPointlight, "Color", 0):SetFloat3(COLOR*R, COLOR*G, COLOR*B)
	world:GetComponent(self.CurrentPointlight, "Pointlight", 0):SetPointlight(X, Y, Z, AMBIENT, DIFFUSE, SPECULAR, R, G, B, RANGE)
	
end

GlowNextCheckpointSystem.Update = function(self, dt)
	
	if self.CurrentGlowing == -1 then
		return
	end
	
	self.TotalTime	=	self.TotalTime + dt
	self.FlameTimer	=	self.FlameTimer + dt
	
	local	R	=	0.6 + math.sin(2.25*self.TotalTime)*0.25
	local	G	=	0.6 + math.sin(2.0*self.TotalTime)*0.25
	local	B	=	0.6 + math.sin(2.0*self.TotalTime)*0.25
	world:GetComponent(self.CurrentGlowing, "Color", 0):SetFloat3(R,G,B)
	
	if self.FlameTimer >= self.FlameLimit then
		self:GlowPointlight()
		self.FlameTimer	=	0.0
	end
	
	
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
			
				world:GetComponent(tempRune, "Color", 0):SetFloat3(1.0, 1.0, 1.0)
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
			
			
			world:GetComponent(newGlow, "Model", 0):SetModel("rune" .. math.random(1, 11), "runes", 1)
			world:GetComponent(newGlow, "Position", 0):SetFloat3(X, 0.1, Z)
			world:GetComponent(newGlow, "Rotation", 0):SetFloat3(0, math.pi/4 * math.random(1, 10), 0)
			world:GetComponent(newGlow, "Scale", 0):SetFloat3(0.5, 1.0, 0.5)
			world:GetComponent(newGlow, "Color", 0):SetFloat3(0.2, 0.2, 0.2)
			world:GetComponent(newGlow, "GlowingCheckpoint", "Stage"):SetInt(cNumber)
		end
		
	end
	
	if tempCurrentGlow ~= -1 then
		if self.CurrentGlowing ~= -1 then
			world:GetComponent(self.CurrentGlowing, "Color", 0):SetFloat3(1.00,1.00,1.00)
		end
		self.CurrentGlowing	=	tempCurrentGlow
		
		local	tempRune	=	self:GetGlowingRune(tempHighest)
		local	X, Z		=	-1, -1 --world:GetComponent(tempRune, "MapPosition", "X"):GetInt2()
		
		local	allRunes	=	self:GetEntities("Checkpoint")
		for tRune = 1, #allRunes do
			if world:GetComponent(allRunes[tRune], "Checkpoint", "Number"):GetInt() == tempHighest then
				X, Z	=	world:GetComponent(allRunes[tRune], "MapPosition", "X"):GetInt2()
			end
		end
		
		local	allGlowing	=	self:GetEntities("GlowingCheckpoint")
		for tGlowing = 1, #allGlowing do
			if world:EntityHasComponent(allGlowing[tGlowing], "Pointlight") then
				world:RemoveComponentFrom("Pointlight", allGlowing[tGlowing])
				break
			end
		end
		self.CurrentPointlight	=	self:GetGlowingRune(tempHighest)
		if X >= 0 and Z >= 0 and self.CurrentPointlight ~= -1 then
			print("X/Z " .. X .. "/" .. Z)
			if not world:EntityHasComponent(self.CurrentPointlight, "Pointlight") then
				world:CreateComponentAndAddTo("Pointlight", self.CurrentPointlight)
			end
			world:GetComponent(self.CurrentPointlight, "Pointlight", 0):SetPointlight(X, 0.7, Z, 0.5, 0.8, 0.7, 0.10, 0.25, 1.0, 1.5)
		end
	end
	
end
