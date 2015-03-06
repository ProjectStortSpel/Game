-- ParticleComponent
local ParticleComponent = ComponentType()
ParticleComponent.Name = "Particle"
ParticleComponent.TableType = TableType.Map
ParticleComponent.SyncNetwork = true
ParticleComponent:AddVariable("aName", ByteSize.Text)
ParticleComponent:AddVariable("bTexture", ByteSize.Text)
ParticleComponent:AddVariable("cParticles", ByteSize.Int)
ParticleComponent:AddVariable("dLifetime", ByteSize.Float)
ParticleComponent:AddVariable("eScale", ByteSize.Float)
ParticleComponent:AddVariable("fSpriteSize", ByteSize.Float)
ParticleComponent:AddVariable("gId", ByteSize.Int)
worldCreator:AddComponentType(ParticleComponent)

--local ParticleComponent = ComponentType()
--ParticleComponent.Name = "Color"
--ParticleComponent.TableType = TableType.Map
--ParticleComponent:AddVariable("X", ByteSize.Float)
--ParticleComponent:AddVariable("Y", ByteSize.Float)
--ParticleComponent:AddVariable("Z", ByteSize.Float)
--worldCreator:AddComponentType(ParticleComponent)