--	This file will contain
--	components that are needed
--	for different triggers

--	Take Cards From Player
local NewComponent = ComponentType()
NewComponent.Name = "TakeCardsFromPlayer"
NewComponent.TableType = TableType.Map
NewComponent:AddVariable("Player", ByteSize.Reference)
worldCreator:AddComponentType(NewComponent)

--	Take Cards Steps From Unit
NewComponent = ComponentType()
NewComponent.Name = "TakeCardStepsFromUnit"
NewComponent.TableType = TableType.Map
NewComponent:AddVariable("Unit", ByteSize.Reference)
worldCreator:AddComponentType(NewComponent)

--	Test Move
NewComponent = ComponentType()
NewComponent.Name = "TestMove"
NewComponent.TableType = TableType.Map
NewComponent:AddVariable("Unit", ByteSize.Reference)
NewComponent:AddVariable("PosX", ByteSize.Int)
NewComponent:AddVariable("PosZ", ByteSize.Int)
NewComponent:AddVariable("DirX", ByteSize.Int)
NewComponent:AddVariable("DirZ", ByteSize.Int)
NewComponent:AddVariable("Steps", ByteSize.Int)
worldCreator:AddComponentType(NewComponent)

NewComponent = ComponentType()
NewComponent.Name = "SimultaneousMove"
NewComponent.TableType = TableType.Map
NewComponent:AddVariable("Unit", ByteSize.Reference)
NewComponent:AddVariable("PosX", ByteSize.Int)
NewComponent:AddVariable("PosZ", ByteSize.Int)
NewComponent:AddVariable("DirX", ByteSize.Int)
NewComponent:AddVariable("DirZ", ByteSize.Int)
NewComponent:AddVariable("Steps", ByteSize.Int)
NewComponent:AddVariable("SlerpTime", ByteSize.Float)
worldCreator:AddComponentType(NewComponent)


--	Test River
NewComponent = ComponentType()
NewComponent.Name = "TestRiver"
NewComponent.TableType = TableType.Map
NewComponent:AddVariable("Unit", ByteSize.Reference)
NewComponent:AddVariable("PosX", ByteSize.Int)
NewComponent:AddVariable("PosZ", ByteSize.Int)
worldCreator:AddComponentType(NewComponent)

NewComponent = ComponentType()
NewComponent.Name = "TestMoveSuccess"
NewComponent.TableType = TableType.Map
NewComponent:AddVariable("Unit", ByteSize.Reference)
NewComponent:AddVariable("Steps", ByteSize.Int)
worldCreator:AddComponentType(NewComponent)

--	End Round
NewComponent = ComponentType()
NewComponent.Name = "EndRound"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)