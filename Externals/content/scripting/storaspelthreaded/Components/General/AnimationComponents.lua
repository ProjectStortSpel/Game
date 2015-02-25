-- Animation
local Animation = ComponentType()
Animation.Name = "Animation"
Animation.TableType = TableType.Map
Animation:AddVariable("Id", ByteSize.Int)
Animation:AddVariable("Time", ByteSize.Float)
worldCreator:AddComponentType(Animation)

-- Animating
local Animating = ComponentType()
Animating.Name = "Animating"
Animating.SyncNetwork = true
Animating.TableType = TableType.Map
Animating:AddVariable("Id", ByteSize.Int)
Animating:AddVariable("Time", ByteSize.Float)
Animating:AddVariable("Timer", ByteSize.Float)
worldCreator:AddComponentType(Animating)