-- SlingShotProjectile Component
local NewComponent = ComponentType()
NewComponent.Name = "SlingShotProjectile"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

-- AbilityStone Component
NewComponent = ComponentType()
NewComponent.Name = "AbilityStone"
NewComponent.TableType = TableType.Map
NewComponent:AddVariable("LifeSpan", ByteSize.Int)
worldCreator:AddComponentType(NewComponent)
