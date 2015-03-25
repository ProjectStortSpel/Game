-- ParticleComponent
local ParticleComponent = ComponentType()
ParticleComponent.Name = "Particle"
ParticleComponent.TableType = TableType.Map
ParticleComponent.SyncNetwork = true
ParticleComponent:AddVariable("aName", ByteSize.Text)
ParticleComponent:AddVariable("bTexture", ByteSize.Text)
ParticleComponent:AddVariable("cParticles", ByteSize.Int)
ParticleComponent:AddVariable("dLifetime", ByteSize.Float)
ParticleComponent:AddVariable("eScaleX", ByteSize.Float)
ParticleComponent:AddVariable("eScaleY", ByteSize.Float)
ParticleComponent:AddVariable("eScaleZ", ByteSize.Float)
ParticleComponent:AddVariable("fVelocityX", ByteSize.Float)
ParticleComponent:AddVariable("fVelocityY", ByteSize.Float)
ParticleComponent:AddVariable("fVelocityZ", ByteSize.Float)
ParticleComponent:AddVariable("gSpriteSize", ByteSize.Float)
ParticleComponent:AddVariable("hId", ByteSize.Int)
ParticleComponent:AddVariable("iOnlyOnce", ByteSize.Int)
worldCreator:AddComponentType(ParticleComponent)

--local ParticleComponent = ComponentType()
--ParticleComponent.Name = "Color"
--ParticleComponent.TableType = TableType.Map
--ParticleComponent:AddVariable("X", ByteSize.Float)
--ParticleComponent:AddVariable("Y", ByteSize.Float)
--ParticleComponent:AddVariable("Z", ByteSize.Float)
--worldCreator:AddComponentType(ParticleComponent)