ShowNextCheckpointSystem = System()

ShowNextCheckpointSystem.Initialize = function ( self )
	--	Set Name
	self:SetName("ShowNextCheckpointSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	
	--	Filters
	self:AddComponentTypeToFilter("Particle", FilterType.Mandatory)
	self:AddComponentTypeToFilter("CheckpointReached", FilterType.Mandatory)
end

ShowNextCheckpointSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)
	
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
			print("LOLOLOLOLOLOLOLOL: ")
			self:SpawnSmoke(oldEffects[n])
			world:KillEntity(oldEffects[n])
		end
	end
	
end

ShowNextCheckpointSystem.SpawnSmoke = function(self, oldParticle)
	
	--local	X, Y, Z	=	world:GetComponent(oldParticle, "Position", "X"):GetFloat3()
	--
	--local	newParticle	=	world:CreateNewEntity()
	--world:CreateComponentAndAddTo("Position", newParticle)
	--world:CreateComponentAndAddTo("Color", newParticle)
	--world:CreateComponentAndAddTo("Particle", newParticle)
	--
	--world:GetComponent(newParticle, "Position", "X"):SetFloat3(X, Y, Z)
	--world:GetComponent(newParticle, "Color", "X"):SetFloat3(0.0, 0.0, 0.0)
	--
	--world:GetComponent(newParticle, "Particle", "Name"):SetText("smoke")
	--world:GetComponent(newParticle, "Particle", "Texture"):SetText("content/textures/smoke1.png")
	--world:GetComponent(newParticle, "Particle", "Particles"):SetInt(200)
	--world:GetComponent(newParticle, "Particle", "Lifetime"):SetFloat(3500)
	--world:GetComponent(newParticle, "Particle", "Scale"):SetFloat(0.05)
	--world:GetComponent(newParticle, "Particle", "SpriteSize"):SetFloat(1.0)
	--world:GetComponent(newParticle, "Particle", "Id"):SetInt(-1)
	--
end

Net.Receive("Client.NewTargetCheckpoint", 
	function( id, ip, port )

	local	cpID	=	Net.ReadInt(id)
	local	X		=	Net.ReadInt(id)
	local	Z		=	Net.ReadInt(id)
	
	local	newParticle	=	world:CreateNewEntity()
	world:CreateComponentAndAddTo("Position", newParticle)
	world:CreateComponentAndAddTo("Color", newParticle)
	world:CreateComponentAndAddTo("Particle", newParticle)
	world:CreateComponentAndAddTo("CheckpointReached", newParticle)
	
	world:GetComponent(newParticle, "CheckpointReached", "CheckpointNumber"):SetInt(cpID)
	
	world:GetComponent(newParticle, "Position", "X"):SetFloat3(X, 0.5, Z)
	world:GetComponent(newParticle, "Color", "X"):SetFloat3(1.0, 0.0, 0.0)
	
	world:GetComponent(newParticle, "Particle", "Name"):SetText("fire")
	world:GetComponent(newParticle, "Particle", "Texture"):SetText("content/textures/firewhite.png")
	world:GetComponent(newParticle, "Particle", "Particles"):SetInt(100)
	world:GetComponent(newParticle, "Particle", "Lifetime"):SetFloat(1500)
	world:GetComponent(newParticle, "Particle", "Scale"):SetFloat(0.05)
	world:GetComponent(newParticle, "Particle", "SpriteSize"):SetFloat(0.6)
	world:GetComponent(newParticle, "Particle", "Id"):SetInt(-1)
	
	
	end
)