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
	self:AddComponentTypeToFilter("Particle", FilterType.Mandatory)
	self:AddComponentTypeToFilter("CheckpointReached", FilterType.Mandatory)
end

ShowNextCheckpointSystem.Update = function(self, dt)

	local Pointlights = self:GetEntities("Pointlight")
	if #Pointlights >= 1 then
		self.TotalTime	=	self.TotalTime + dt
		self.FlameTimer	=	self.FlameTimer + dt
		
		if self.FlameTimer > self.FlameLimit then
			for n = 1, #Pointlights do
			
				local	tPL	=	Pointlights[n]
				
				local	X, Y, Z, AMBIENT, DIFFUSE, SPECULAR, R, G, B, RANGE	=	world:GetComponent(tPL, "Pointlight", 0):GetPointlight()
				--local	newRange	=	3 + math.sin(self.TotalTime)*math.random(1, 10)*dt
				DIFFUSE		=	0.8 + 0.8*math.sin(dt*(self.TotalTime + math.random(1, 400)))*0.2
				SPECULAR	=	0.7 + 0.7*math.sin(dt*(self.TotalTime + math.random(1, 400)))*0.2
				world:GetComponent(tPL, "Pointlight", 0):SetPointlight(X, Y, Z, AMBIENT, DIFFUSE, SPECULAR, R, G, B, 5)
			end
			
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
	
	world:GetComponent(newParticle, "Particle", "Name"):SetText("smoke")
	world:GetComponent(newParticle, "Particle", "Texture"):SetText("content/textures/smoke1.png")
	world:GetComponent(newParticle, "Particle", "Particles"):SetInt(8)
	world:GetComponent(newParticle, "Particle", "Lifetime"):SetFloat(1700)
	world:GetComponent(newParticle, "Particle", "Scale"):SetFloat(0.008)
	world:GetComponent(newParticle, "Particle", "SpriteSize"):SetFloat(0.4)
	world:GetComponent(newParticle, "Particle", "Id"):SetInt(-1)
	
end

Net.Receive("Client.NewTargetCheckpoint", 
	function( id, ip, port )

	
	local	cpID	=	Net.ReadInt(id)
	local	X		=	Net.ReadFloat(id)
	local	Z		=	Net.ReadFloat(id)
	
	local	newParticle	=	world:CreateNewEntity()
	world:CreateComponentAndAddTo("Position", newParticle)
	world:CreateComponentAndAddTo("Color", newParticle)
	world:CreateComponentAndAddTo("Particle", newParticle)
	world:CreateComponentAndAddTo("CheckpointReached", newParticle)
	world:CreateComponentAndAddTo("Pointlight", newParticle)
	
	world:GetComponent(newParticle, "CheckpointReached", "CheckpointNumber"):SetInt(cpID)
	
	world:GetComponent(newParticle, "Position", "X"):SetFloat3(X, 0.70, Z)
	world:GetComponent(newParticle, "Color", "X"):SetFloat3(0.98, 0.17, 0.08)
	
	world:GetComponent(newParticle, "Particle", "Name"):SetText("fire")
	world:GetComponent(newParticle, "Particle", "Texture"):SetText("content/textures/firewhite.png")
	world:GetComponent(newParticle, "Particle", "Particles"):SetInt(80)
	world:GetComponent(newParticle, "Particle", "Lifetime"):SetFloat(1200)
	world:GetComponent(newParticle, "Particle", "Scale"):SetFloat(0.016)
	world:GetComponent(newParticle, "Particle", "SpriteSize"):SetFloat(0.6)
	world:GetComponent(newParticle, "Particle", "Id"):SetInt(-1)
	--	X, Y, Z Ambient, Diffuse, Specular, R, G, B, Range
	world:GetComponent(newParticle, "Pointlight", 0):SetPointlight(X, 0.7, Z, 0.5, 0.8, 0.7, 0.9, 0.65, 0.40, 2)
	
	
	
	end
)