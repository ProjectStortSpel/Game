-- ParticleComponent
local ParticleComponent = ComponentType()
ParticleComponent.Name = "Particle"
ParticleComponent.TableType = TableType.Map
ParticleComponent:AddVariable("Name", ByteSize.Text)
ParticleComponent:AddVariable("Texture", ByteSize.Text)
ParticleComponent:AddVariable("Particles", ByteSize.Int)
ParticleComponent:AddVariable("Lifetime", ByteSize.Float)
ParticleComponent:AddVariable("Scale", ByteSize.Float)
ParticleComponent:AddVariable("SpriteSize", ByteSize.Float)
ParticleComponent:AddVariable("Id", ByteSize.Int)
worldCreator:AddComponentType(ParticleComponent)

--local ParticleComponent = ComponentType()
--ParticleComponent.Name = "Color"
--ParticleComponent.TableType = TableType.Map
--ParticleComponent:AddVariable("X", ByteSize.Float)
--ParticleComponent:AddVariable("Y", ByteSize.Float)
--ParticleComponent:AddVariable("Z", ByteSize.Float)
--worldCreator:AddComponentType(ParticleComponent)