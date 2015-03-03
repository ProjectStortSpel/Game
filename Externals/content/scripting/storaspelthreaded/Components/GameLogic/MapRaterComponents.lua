
--	PlayedCard
local	NewComponent		=	ComponentType()
NewComponent.Name 		= 	"PlayedCard"
NewComponent.TableType	= 	TableType.Map
NewComponent:AddVariable("CardAction", 	ByteSize.Text)
NewComponent:AddVariable("CardPriority", 	ByteSize.Int)
NewComponent:AddVariable("PlayerNumber", 	ByteSize.Int)
worldCreator:AddComponentType(NewComponent)

--	CheckpointReached
NewComponent			=	ComponentType()
NewComponent.Name 		= 	"CheckpointReached"
NewComponent.TableType	= 	TableType.Map
NewComponent:AddVariable("CheckpointNumber", 	ByteSize.Int)
NewComponent:AddVariable("PlayerNumber", 		ByteSize.Int)
worldCreator:AddComponentType(NewComponent)

--	UnitDied
NewComponent			=	ComponentType()
NewComponent.Name 		= 	"UnitDied"
NewComponent.TableType	= 	TableType.Map
NewComponent:AddVariable("PlayerNumber", 		ByteSize.Int)
NewComponent:AddVariable("X", 					ByteSize.Int)
NewComponent:AddVariable("Z", 					ByteSize.Int)
worldCreator:AddComponentType(NewComponent)