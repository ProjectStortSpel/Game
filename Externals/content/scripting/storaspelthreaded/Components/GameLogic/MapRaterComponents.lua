
--	PlayedCard
local	PlayedCard		=	ComponentType()
PlayedCard.Name 		= 	"PlayedCard"
PlayedCard.TableType	= 	TableType.Map
PlayedCard:AddVariable("CardAction", 	ByteSize.Text)
PlayedCard:AddVariable("CardPriority", 	ByteSize.Int)
PlayedCard:AddVariable("PlayerNumber", 	ByteSize.Int)
worldCreator:AddComponentType(PlayedCard)

--	CheckpointReached
local	CheckpointReached	=	ComponentType()
CheckpointReached.Name 		= 	"CheckpointReached"
CheckpointReached.TableType	= 	TableType.Map
CheckpointReached:AddVariable("CheckpointNumber", 	ByteSize.Int)
CheckpointReached:AddVariable("PlayerNumber", 		ByteSize.Int)
worldCreator:AddComponentType(CheckpointReached)

--	UnitDied
local	UnitDied			=	ComponentType()
UnitDied.Name 		= 	"UnitDied"
UnitDied.TableType	= 	TableType.Map
UnitDied:AddVariable("PlayerNumber", 		ByteSize.Int)
UnitDied:AddVariable("X", 					ByteSize.Int)
UnitDied:AddVariable("Z", 					ByteSize.Int)
worldCreator:AddComponentType(UnitDied)