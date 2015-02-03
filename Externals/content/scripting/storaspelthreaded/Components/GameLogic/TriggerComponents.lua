--	This file will contain
--	components that are needed
--	for different triggers

--	Take Cards From Player
local TakeCardsFromPlayerComponent = ComponentType()
TakeCardsFromPlayerComponent.Name = "TakeCardsFromPlayer"
TakeCardsFromPlayerComponent.TableType = TableType.Map
TakeCardsFromPlayerComponent:AddVariable("Player", ByteSize.Reference)
worldCreator:AddComponentType(TakeCardsFromPlayerComponent)

--	Take Cards Steps From Unit
local TakeCardStepsFromUnitComponent = ComponentType()
TakeCardStepsFromUnitComponent.Name = "TakeCardStepsFromUnit"
TakeCardStepsFromUnitComponent.TableType = TableType.Map
TakeCardStepsFromUnitComponent:AddVariable("Unit", ByteSize.Reference)
worldCreator:AddComponentType(TakeCardStepsFromUnitComponent)

--	Test Move
local TestMoveComponent = ComponentType()
TestMoveComponent.Name = "TestMove"
TestMoveComponent.TableType = TableType.Map
TestMoveComponent:AddVariable("Unit", ByteSize.Reference)
TestMoveComponent:AddVariable("PosX", ByteSize.Int)
TestMoveComponent:AddVariable("PosZ", ByteSize.Int)
TestMoveComponent:AddVariable("DirX", ByteSize.Int)
TestMoveComponent:AddVariable("DirZ", ByteSize.Int)
TestMoveComponent:AddVariable("Steps", ByteSize.Int)
worldCreator:AddComponentType(TestMoveComponent)

--	Test River
local TestRiverComponent = ComponentType()
TestRiverComponent.Name = "TestRiver"
TestRiverComponent.TableType = TableType.Map
TestRiverComponent:AddVariable("Unit", ByteSize.Reference)
TestRiverComponent:AddVariable("PosX", ByteSize.Int)
TestRiverComponent:AddVariable("PosZ", ByteSize.Int)
worldCreator:AddComponentType(TestRiverComponent)

--	End Round
local EndRoundComponent = ComponentType()
EndRoundComponent.Name = "EndRound"
EndRoundComponent.TableType = TableType.Map
worldCreator:AddComponentType(EndRoundComponent)