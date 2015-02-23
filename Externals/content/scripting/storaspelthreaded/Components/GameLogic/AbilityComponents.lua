-- SlingShotProjectile Component
local SlingShotProjectileComponent = ComponentType()
SlingShotProjectileComponent.Name = "SlingShotProjectile"
SlingShotProjectileComponent.TableType = TableType.Map
worldCreator:AddComponentType(SlingShotProjectileComponent)

-- AbilityStone Component
local AbilityStoneComponent = ComponentType()
AbilityStoneComponent.Name = "AbilityStone"
AbilityStoneComponent.TableType = TableType.Map
AbilityStoneComponent:AddVariable("LifeSpan", ByteSize.Int)
worldCreator:AddComponentType(AbilityStoneComponent)
