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

-- StunnedIndicator Component
local StunnedIndicator = ComponentType()
StunnedIndicator.Name = "StunnedIndicator"
StunnedIndicator.TableType = TableType.Map
worldCreator:AddComponentType(StunnedIndicator)

-- AddStunnedIndicator Component
local AddStunnedIndicator = ComponentType()
AddStunnedIndicator.Name = "AddStunnedIndicator"
AddStunnedIndicator.TableType = TableType.Map
AddStunnedIndicator:AddVariable("Unit", ByteSize.Reference)
AddStunnedIndicator.SyncNetwork = true
worldCreator:AddComponentType(AddStunnedIndicator)

-- HasStunnedIndicator Component
local HasStunnedIndicator = ComponentType()
HasStunnedIndicator.Name = "HasStunnedIndicator"
HasStunnedIndicator.TableType = TableType.Map
worldCreator:AddComponentType(HasStunnedIndicator)

-- TargetUnit Component
local TargetUnit = ComponentType()
TargetUnit.Name = "TargetUnit"
TargetUnit.TableType = TableType.Map
TargetUnit:AddVariable("Unit", ByteSize.Reference)
worldCreator:AddComponentType(TargetUnit)