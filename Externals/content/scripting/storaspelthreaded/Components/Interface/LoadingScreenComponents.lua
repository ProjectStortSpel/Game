--	This file will contain
--	components that are needed
--	for the loadingscreen

local LoadingScreenDelay = ComponentType()
LoadingScreenDelay.Name = "LoadingScreenDelay"
LoadingScreenDelay.TableType = TableType.Map
LoadingScreenDelay:AddVariable("AccessLevel", ByteSize.Int)
LoadingScreenDelay:AddVariable("Delay", ByteSize.Int)
worldCreator:AddComponentType(LoadingScreenDelay)