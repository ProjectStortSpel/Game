GlowNextCheckpointSystem = System()
GlowNextCheckpointSystem.TotalTime		=	0.0
GlowNextCheckpointSystem.GlowTimer		=	0.0
GlowNextCheckpointSystem.GlowInterval	=	1/20

GlowNextCheckpointSystem.GlowingRuneEnt		=	-1
GlowNextCheckpointSystem.CurrentGlowStage	=	0

GlowNextCheckpointSystem.TargetRune	=	-1


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

GlowNextCheckpointSystem.EntitiesAdded = function(self, dt, newEntities)
	
	local	targetRune	=	2147483647
	
	for nEntity = 1, #newEntities do
		
		local	newEntity	=	newEntities[nEntity]
		
		if world:EntityHasComponent(newEntity, "GlowCheckpoint") then
			
			local	cNumber		=	world:GetComponent(newEntity, "GlowCheckpoint", "Stage"):GetInt()
			
			print("cNumber: " .. cNumber)
			if cNumber == -1 then
				local	allGlowing	=	self:GetEntities("GlowingCheckpoint")
				for tGlowing = 1, #allGlowing do
					if world:EntityHasComponent(allGlowing[tGlowing], "Pointlight") then
						world:RemoveComponentFrom("Pointlight", allGlowing[tGlowing])
						break
					end
				end
				
				self.TargetRune	=	self:GetGlowingRune(self.CurrentGlowStage)
				self.CurrentGlowStage	=	-1
				self.GlowingRuneEnt		=	-1
				print("TargetRune: " .. self.TargetRune)
				if self.TargetRune ~= -1 then
					world:GetComponent(self.TargetRune, "Color", 0):SetFloat3(1.00,1.00,1.00)
				end
			else
				if cNumber > self.CurrentGlowStage then
					self.CurrentGlowStage	=	cNumber
				end
				
				if cNumber < targetRune then
					self.TargetRune	=	self:GetGlowingRune(cNumber)
					targetRune		=	cNumber
				end
			end
			
			--	Kill the entity
			world:KillEntity(newEntity)
		end
		
		--	New checkpoint, add Rune object
		if world:EntityHasComponent(newEntity, "Checkpoint") then
			self:CreateRune(newEntity)
		end
	
	end
	
	if self.CurrentGlowStage > 0 then
		self.GlowingRuneEnt	=	self:GetGlowingRune(self.CurrentGlowStage)
		
		if self.GlowingRuneEnt ~= -1 then
			
			local	X, Z	=	-1, -1
			local	allRunes	=	self:GetEntities("Checkpoint")
			for tRune = 1, #allRunes do
				if world:GetComponent(allRunes[tRune], "Checkpoint", "Number"):GetInt() == self.CurrentGlowStage then
					X, Z	=	world:GetComponent(allRunes[tRune], "MapPosition", "X"):GetInt2()
					break
				end
			end
			
			if not world:EntityHasComponent(self.GlowingRuneEnt, "Pointlight") then
				world:CreateComponentAndAddTo("Pointlight", self.GlowingRuneEnt)
			end
			world:GetComponent(self.GlowingRuneEnt, "Pointlight", 0):SetPointlight(X, 0.7, Z, 0.5, 0.8, 0.7, 0.10, 0.25, 1.0, 1.5)
		end
	end
	
end

GlowNextCheckpointSystem.Update = function(self, dt)
	
	if self.TargetRune == -1 then
		return
	end
	
	--	Update timers
	self.TotalTime	=	self.TotalTime + dt
	self.GlowTimer	=	self.GlowTimer + dt
	
	local	R	=	0.6 + math.sin(0.10*self.TotalTime)*0.25
	local	G	=	0.6 + math.sin(0.10*self.TotalTime*self.TotalTime)*0.25
	local	B	=	0.6 + math.sin(0.10*self.TotalTime*self.TotalTime*self.TotalTime)*0.25
	world:GetComponent(self.TargetRune, "Color", 0):SetFloat3(R,G,B)
	
	--	Update the glowing light
	if self.GlowTimer > self.GlowInterval then
		self:GlowPointlightLogic()
		self.GlowTimer	=	0.0
	end
end

GlowNextCheckpointSystem.CreateRune = function(self, CheckpointEntity)

	local	X, Z	=	world:GetComponent(CheckpointEntity, "MapPosition", "X"):GetInt2()
	local	cNumber	=	world:GetComponent(CheckpointEntity, "Checkpoint", "Number"):GetInt()
	
	local	newGlow	=	world:CreateNewEntity()
	world:CreateComponentAndAddTo("Position", newGlow)
	world:CreateComponentAndAddTo("Rotation", newGlow)
	world:CreateComponentAndAddTo("Scale", newGlow)
	world:CreateComponentAndAddTo("Model", newGlow)
	world:CreateComponentAndAddTo("Color", newGlow)
	world:CreateComponentAndAddTo("GlowingCheckpoint", newGlow)
	
	
	world:GetComponent(newGlow, "Model", 0):SetModel("rune" .. math.random(1, 11), "runes", 1)
	world:GetComponent(newGlow, "Position", 0):SetFloat3(X, 0.1, Z)
	world:GetComponent(newGlow, "Rotation", 0):SetFloat3(0, math.pi/4 * math.random(1, 36), 0)
	world:GetComponent(newGlow, "Scale", 0):SetFloat3(0.5, 1.0, 0.5)
	world:GetComponent(newGlow, "Color", 0):SetFloat3(0.2, 0.2, 0.2)
	world:GetComponent(newGlow, "GlowingCheckpoint", "Stage"):SetInt(cNumber)
	world:CreateComponentAndAddTo("NoShadow", newGlow)
	
end

GlowNextCheckpointSystem.GlowPointlightLogic = function(self)

	if self.GlowingRuneEnt == -1 then
		return
	end
	
	local	X, Y, Z, AMBIENT, DIFFUSE, SPECULAR, R, G, B, RANGE	=	world:GetComponent(self.GlowingRuneEnt, "Pointlight", 0):GetPointlight()
	local	AMBIENT		=	0.25 + math.sin(1.5*self.TotalTime)*0.5*0.5
	local	DIFFUSE		=	0.4 + math.sin(1.5*self.TotalTime)*0.8*0.5
	local	SPECULAR	=	0.35 + math.sin(1.5*self.TotalTime)*0.7*0.5
	local	COLOR		=	0.75 + math.sin(1.5*self.TotalTime)*0.15
	
	world:GetComponent(self.GlowingRuneEnt, "Color", 0):SetFloat3(COLOR*R, COLOR*G, COLOR*B)
	world:GetComponent(self.GlowingRuneEnt, "Pointlight", 0):SetPointlight(X, Y, Z, AMBIENT, DIFFUSE, SPECULAR, R, G, B, RANGE)
end

GlowNextCheckpointSystem.GetGlowingRune = function(self, StageNumber)

	local	allRunes	=	self:GetEntities("GlowingCheckpoint")
	for tRune = 1, #allRunes do
		if world:GetComponent(allRunes[tRune], "GlowingCheckpoint", "Stage"):GetInt() == StageNumber then
			return allRunes[tRune]
		end
	end
	
	return	-1
end
