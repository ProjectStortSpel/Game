ClientAbilitySlingshotSystem = System()

ClientAbilitySlingshotSystem.Initialize = function ( self )
	--	Set Name
	self:SetName("ClientAbilitySlingshotSystem")
end

Net.Receive("SERVER_BULLET_IMPACT_PARTICLE",
	function(id, ip, port)
		local x = Net.ReadFloat(id)
		local y = Net.ReadFloat(id)
		local z = Net.ReadFloat(id)
			
		local	newParticle	=	world:CreateNewEntity()
		world:CreateComponentAndAddTo("Position", newParticle)
		world:CreateComponentAndAddTo("Color", newParticle)
		world:CreateComponentAndAddTo("Particle", newParticle)
		
		world:GetComponent(newParticle, "Position", "X"):SetFloat3(x, y + 0.2, z)
		world:GetComponent(newParticle, "Color", "X"):SetFloat3(0.0, 0.0, 0.0	)
		
		world:GetComponent(newParticle, "Particle", "aName"):SetText("explosion")
		world:GetComponent(newParticle, "Particle", "bTexture"):SetText("content/textures/dust.png")
		world:GetComponent(newParticle, "Particle", "cParticles"):SetInt(15)
		world:GetComponent(newParticle, "Particle", "dLifetime"):SetFloat(1.4)
		world:GetComponent(newParticle, "Particle", "eScaleX"):SetFloat(0.01)
		world:GetComponent(newParticle, "Particle", "eScaleY"):SetFloat(0.01)
		world:GetComponent(newParticle, "Particle", "eScaleZ"):SetFloat(0.01)
		world:GetComponent(newParticle, "Particle", "fVelocityX"):SetFloat(0)
		world:GetComponent(newParticle, "Particle", "fVelocityY"):SetFloat(0)
		world:GetComponent(newParticle, "Particle", "fVelocityZ"):SetFloat(0)
		world:GetComponent(newParticle, "Particle", "gSpriteSize"):SetFloat(1.4)
		world:GetComponent(newParticle, "Particle", "hId"):SetInt(-1)
		world:GetComponent(newParticle, "Particle", "iOnlyOnce"):SetInt(1)
	
	end
)



Net.Receive("SERVER_BULLET_IMPACT_PLAYER_PARTICLE",
	function(id, ip, port)
	
		local x = Net.ReadFloat(id)
		local y = Net.ReadFloat(id)
		local z = Net.ReadFloat(id)
			
		local	newParticle	=	world:CreateNewEntity()
		world:CreateComponentAndAddTo("Position", newParticle)
		world:CreateComponentAndAddTo("Color", newParticle)
		world:CreateComponentAndAddTo("Particle", newParticle)
		
		world:GetComponent(newParticle, "Position", "X"):SetFloat3(x, y + 0.2, z)
		world:GetComponent(newParticle, "Color", "X"):SetFloat3(0.0, 0.0, 0.0	)
		
		world:GetComponent(newParticle, "Particle", "aName"):SetText("explosion")
		world:GetComponent(newParticle, "Particle", "bTexture"):SetText("content/textures/dust.png")
		world:GetComponent(newParticle, "Particle", "cParticles"):SetInt(15)
		world:GetComponent(newParticle, "Particle", "dLifetime"):SetFloat(1.4)
		world:GetComponent(newParticle, "Particle", "eScaleX"):SetFloat(0.01)
		world:GetComponent(newParticle, "Particle", "eScaleY"):SetFloat(0.01)
		world:GetComponent(newParticle, "Particle", "eScaleZ"):SetFloat(0.01)
		world:GetComponent(newParticle, "Particle", "fVelocityX"):SetFloat(0)
		world:GetComponent(newParticle, "Particle", "fVelocityY"):SetFloat(0)
		world:GetComponent(newParticle, "Particle", "fVelocityZ"):SetFloat(0)
		world:GetComponent(newParticle, "Particle", "gSpriteSize"):SetFloat(1.4)
		world:GetComponent(newParticle, "Particle", "hId"):SetInt(-1)
		world:GetComponent(newParticle, "Particle", "iOnlyOnce"):SetInt(1)
				
		--local stunnedIndicator = world:CreateNewEntity()
		--world:CreateComponentAndAddTo("Position", stunnedIndicator)
		--world:CreateComponentAndAddTo("Scale", stunnedIndicator)
		--world:CreateComponentAndAddTo("Rotation", stunnedIndicator)
		--world:CreateComponentAndAddTo("Model", stunnedIndicator)
		--world:CreateComponentAndAddTo("Spin", stunnedIndicator)
		--world:CreateComponentAndAddTo("NoShadow", stunnedIndicator)
		--world:GetComponent(stunnedIndicator, "Model", 0):SetModel("superlowpolystun", "superlowpolystun", 1)
		--world:GetComponent(stunnedIndicator, "Position", 0):SetFloat3(x, y + 0.5, z)
		--world:GetComponent(stunnedIndicator, "Rotation", 0):SetFloat3(0,0,0)
		--world:GetComponent(stunnedIndicator, "Scale", 0):SetFloat3(0.5,0.5,0.5)
		--world:GetComponent(stunnedIndicator, "Spin", 0):SetFloat3(0,-5,0)
		
	end
)