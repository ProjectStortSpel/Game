ShowNextCheckpointSystem = System()
ShowNextCheckpointSystem.TotalTime	=	0.0
ShowNextCheckpointSystem.FlameTimer	=	0.0
ShowNextCheckpointSystem.FlameLimit	=	1/10

ShowNextCheckpointSystem.Initialize = function ( self )
	--	Set Name
	self:SetName("ShowNextCheckpointSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	self:UsingUpdate()
	
	--	Filters
	self:AddComponentTypeToFilter("Particle", 			FilterType.Mandatory)
	self:AddComponentTypeToFilter("CheckpointReached",	FilterType.Mandatory)
end

ShowNextCheckpointSystem.FadeIn = function(self, lightToFade, dt)

	local	fTimer		=	world:GetComponent(lightToFade, "FadeInLight", "CurrentTime"):GetFloat() + dt
	local	fMaxTime	=	world:GetComponent(lightToFade, "FadeInLight", "FadeTime"):GetFloat()
	
	local	X, Y, Z, AMBIENT, DIFFUSE, SPECULAR, R, G, B, RANGE	=	world:GetComponent(lightToFade, "Pointlight", 0):GetPointlight()
	AMBIENT		=	0.5 * (fTimer/fMaxTime)
	DIFFUSE		=	0.8 * (fTimer/fMaxTime)
	SPECULAR	=	0.7 * (fTimer/fMaxTime)
	world:GetComponent(lightToFade, "Pointlight", 0):SetPointlight(X, Y, Z, AMBIENT, DIFFUSE, SPECULAR, R, G, B, RANGE)
	
	if fTimer >= fMaxTime then
		world:RemoveComponentFrom("FadeInLight", lightToFade)
	else
		world:GetComponent(lightToFade, "FadeInLight", "CurrentTime"):SetFloat(fTimer)
	end
end

ShowNextCheckpointSystem.Update = function(self, dt)

	local Pointlights = self:GetEntities("Pointlight")
	if #Pointlights >= 1 then
		self.TotalTime	=	self.TotalTime + dt
		self.FlameTimer	=	self.FlameTimer + dt
		for n = 1, #Pointlights do
		
			local	tPL	=	Pointlights[n]
			
			if world:EntityHasComponent(tPL, "FadeInLight") then
				self:FadeIn(tPL, dt)
			else
				if self.FlameTimer > self.FlameLimit then
					local	X, Y, Z, AMBIENT, DIFFUSE, SPECULAR, R, G, B, RANGE	=	world:GetComponent(tPL, "Pointlight", 0):GetPointlight()
					--local	newRange	=	3 + math.sin(self.TotalTime)*math.random(1, 10)*dt
					DIFFUSE		=	0.8 + 0.8*math.sin(dt*(self.TotalTime + math.random(1, 400)))*0.2
					SPECULAR	=	0.7 + 0.7*math.sin(dt*(self.TotalTime + math.random(1, 400)))*0.2
					world:GetComponent(tPL, "Pointlight", 0):SetPointlight(X, Y, Z, AMBIENT, DIFFUSE, SPECULAR, R, G, B, 5)
				end
			end
		end
		
		if self.FlameTimer > self.FlameLimit then
			self.FlameTimer	=	0.0
		end
		
	end
end

ShowNextCheckpointSystem.EntitiesAdded = function(self, dt, entities)
	
	local	highestCheckpoint	=	-1
	local	highestCheckpointId	=	-1
	for n = 1, #entities do
	
		local	tempCheckpoint	=	world:GetComponent(entities[n], "CheckpointReached", "CheckpointNumber"):GetInt()
		if tempCheckpoint > highestCheckpoint then
			highestCheckpoint	=	tempCheckpoint
			highestCheckpointId	=	entities[n]
		end
		
	end
	
	
	local	oldEffects	=	self:GetEntities()
	for n = 1, #oldEffects do
		
		if oldEffects[n] ~= highestCheckpointId then
			self:SpawnSmoke(oldEffects[n])
			world:KillEntity(oldEffects[n])
		end
	end
	
end

ShowNextCheckpointSystem.SpawnSmoke = function(self, oldParticle)
	
	local	X, Y, Z	=	world:GetComponent(oldParticle, "Position", "X"):GetFloat3()
	
	local	newParticle	=	world:CreateNewEntity()
	world:CreateComponentAndAddTo("Position", newParticle)
	world:CreateComponentAndAddTo("Color", newParticle)
	world:CreateComponentAndAddTo("Particle", newParticle)
	
	world:GetComponent(newParticle, "Position", "X"):SetFloat3(X, Y-0.05, Z)
	world:GetComponent(newParticle, "Color", "X"):SetFloat3(0.0, 0.0, 0.0)
	
	world:GetComponent(newParticle, "Particle", "aName"):SetText("smoke")
	world:GetComponent(newParticle, "Particle", "bTexture"):SetText("content/textures/smoke1.png")
	world:GetComponent(newParticle, "Particle", "cParticles"):SetInt(8)
	world:GetComponent(newParticle, "Particle", "dLifetime"):SetFloat(1.7)
	world:GetComponent(newParticle, "Particle", "eScaleX"):SetFloat(0.008)
	world:GetComponent(newParticle, "Particle", "eScaleY"):SetFloat(0.008)
	world:GetComponent(newParticle, "Particle", "eScaleZ"):SetFloat(0.008)
	world:GetComponent(newParticle, "Particle", "fVelocityX"):SetFloat(0)
	world:GetComponent(newParticle, "Particle", "fVelocityY"):SetFloat(0)
	world:GetComponent(newParticle, "Particle", "fVelocityZ"):SetFloat(0)
	world:GetComponent(newParticle, "Particle", "gSpriteSize"):SetFloat(0.4)
	world:GetComponent(newParticle, "Particle", "hId"):SetInt(-1)
	world:GetComponent(newParticle, "Particle", "iOnlyOnce"):SetInt(0)
	
end

Net.Receive("Client.NewTargetCheckpoint", 
	function( id, ip, port )

	
	local	cpID	=	Net.ReadInt(id)
	local	X		=	Net.ReadFloat(id)
	local	Z		=	Net.ReadFloat(id)
	local	C1		=	Net.ReadInt(id)
	local	C2		=	Net.ReadInt(id)
	
	if C1 ~= -1 then
		local	newEntity	=	world:CreateNewEntity()
		world:CreateComponentAndAddTo("GlowCheckpoint", newEntity)
		
		world:GetComponent(newEntity, "GlowCheckpoint", "Stage"):SetInt(C1)
		
		if C2 ~= -1 then
			newEntity	=	world:CreateNewEntity()
			world:CreateComponentAndAddTo("GlowCheckpoint", newEntity)
			
			world:GetComponent(newEntity, "GlowCheckpoint", "Stage"):SetInt(C2)
		end
	end
	

	
	
	local	newParticle	=	world:CreateNewEntity()
	world:CreateComponentAndAddTo("Position", newParticle)
	world:CreateComponentAndAddTo("Color", newParticle)
	world:CreateComponentAndAddTo("Particle", newParticle)
	world:CreateComponentAndAddTo("CheckpointReached", newParticle)
	world:CreateComponentAndAddTo("Pointlight", newParticle)
	world:CreateComponentAndAddTo("FadeInLight", newParticle)
	
	
	world:GetComponent(newParticle, "CheckpointReached", "CheckpointNumber"):SetInt(cpID)
	
	world:GetComponent(newParticle, "Position", "X"):SetFloat3(X, 0.70, Z)
	world:GetComponent(newParticle, "Color", "X"):SetFloat3(0.98, 0.17, 0.08)
	
	world:GetComponent(newParticle, "Particle", "aName"):SetText("fire")
	world:GetComponent(newParticle, "Particle", "bTexture"):SetText("content/textures/firewhite.png")
	world:GetComponent(newParticle, "Particle", "cParticles"):SetInt(80)
	world:GetComponent(newParticle, "Particle", "dLifetime"):SetFloat(1.2)
	world:GetComponent(newParticle, "Particle", "eScaleX"):SetFloat(0.016)
	world:GetComponent(newParticle, "Particle", "eScaleY"):SetFloat(0.016)
	world:GetComponent(newParticle, "Particle", "eScaleZ"):SetFloat(0.016)
	world:GetComponent(newParticle, "Particle", "fVelocityX"):SetFloat(0)
	world:GetComponent(newParticle, "Particle", "fVelocityY"):SetFloat(0)
	world:GetComponent(newParticle, "Particle", "fVelocityZ"):SetFloat(0)
	world:GetComponent(newParticle, "Particle", "gSpriteSize"):SetFloat(0.6)
	world:GetComponent(newParticle, "Particle", "hId"):SetInt(-1)
	world:GetComponent(newParticle, "Particle", "iOnlyOnce"):SetInt(0)
	
	--	Fade in timer
	world:GetComponent(newParticle, "FadeInLight", "CurrentTime"):SetFloat(0.0)
	world:GetComponent(newParticle, "FadeInLight", "FadeTime"):SetFloat(1.0)
	--	X, Y, Z Ambient, Diffuse, Specular, R, G, B, Range
	world:GetComponent(newParticle, "Pointlight", 0):SetPointlight(X, 0.7, Z, 0.0, 0.0, 0.0, 0.9, 0.65, 0.40, 5)
	
	
	
	end
)