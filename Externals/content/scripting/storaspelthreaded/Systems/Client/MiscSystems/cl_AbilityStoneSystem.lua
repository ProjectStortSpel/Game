ClientAbilityStoneSystem = System()

ClientAbilityStoneSystem.Initialize = function ( self )
	--	Set Name
	self:SetName("ClientAbilityStoneSystem")
end

Net.Receive("SERVER_STONE_IMPACT_PARTICLE",
	function(id, ip, port)
		local x = Net.ReadFloat(id)
		local y = Net.ReadFloat(id)
		local z = Net.ReadFloat(id)
		
	local	newParticle	=	world:CreateNewEntity()
	world:CreateComponentAndAddTo("Position", newParticle)
	world:CreateComponentAndAddTo("Color", newParticle)
	world:CreateComponentAndAddTo("Particle", newParticle)
	
	world:GetComponent(newParticle, "Position", "X"):SetFloat3(x, y + 0.5, z)
	world:GetComponent(newParticle, "Color", "X"):SetFloat3(0.0, 0.0, 0.0	)
	
	world:GetComponent(newParticle, "Particle", "aName"):SetText("explosion")
	world:GetComponent(newParticle, "Particle", "bTexture"):SetText("content/textures/dust.png")
	world:GetComponent(newParticle, "Particle", "cParticles"):SetInt(15)
	world:GetComponent(newParticle, "Particle", "dLifetime"):SetFloat(1.8)
	world:GetComponent(newParticle, "Particle", "eScaleX"):SetFloat(0.01)
	world:GetComponent(newParticle, "Particle", "eScaleY"):SetFloat(0.01)
	world:GetComponent(newParticle, "Particle", "eScaleZ"):SetFloat(0.01)
	world:GetComponent(newParticle, "Particle", "fVelocityX"):SetFloat(0)
	world:GetComponent(newParticle, "Particle", "fVelocityY"):SetFloat(0)
	world:GetComponent(newParticle, "Particle", "fVelocityZ"):SetFloat(0)
	world:GetComponent(newParticle, "Particle", "gSpriteSize"):SetFloat(2.5)
	world:GetComponent(newParticle, "Particle", "hId"):SetInt(-1)
	world:GetComponent(newParticle, "Particle", "iOnlyOnce"):SetInt(1)
	
	end
)