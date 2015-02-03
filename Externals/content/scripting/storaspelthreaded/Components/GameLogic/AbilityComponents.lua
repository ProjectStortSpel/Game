-- SlingShotProjectile Component
local SlingShotProjectileComponent = ComponentType()
SlingShotProjectileComponent.Name = "SlingShotProjectile"
SlingShotProjectileComponent.TableType = TableType.Map
worldCreator:AddComponentType(SlingShotProjectileComponent)

-- Stunned Component
local StunnedComponent = ComponentType()
StunnedComponent.Name = "Stunned"
StunnedComponent.TableType = TableType.Map
worldCreator:AddComponentType(StunnedComponent)

-- RemoveEffects Component
local RemoveEffectsComponent = ComponentType()
RemoveEffectsComponent.Name = "RemoveEffects"
RemoveEffectsComponent.TableType = TableType.Map
worldCreator:AddComponentType(RemoveEffectsComponent)