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

-- StoneTimer Component
local StoneTimer = ComponentType()
StoneTimer.Name = "StoneTimer"
StoneTimer.TableType = TableType.Map
StoneTimer.SyncNetwork = true
StoneTimer:AddVariable("LifeSpan", ByteSize.Int)
worldCreator:AddComponentType(StoneTimer)

-- StoneTimerText Component
local StoneTimerText = ComponentType()
StoneTimerText.Name = "StoneTimerText"
StoneTimerText.TableType = TableType.Map
worldCreator:AddComponentType(StoneTimerText)

-- TempTimer Component
local TempTimer = ComponentType()
TempTimer.Name = "TempTimer"
TempTimer.TableType = TableType.Map
worldCreator:AddComponentType(TempTimer)