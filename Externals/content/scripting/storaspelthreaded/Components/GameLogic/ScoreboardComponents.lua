--	Scoreboard Component
local NewComponent = ComponentType()
NewComponent.Name = "Scoreboard"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

--	ScoreboardElement Component
NewComponent = ComponentType()
NewComponent.Name = "ScoreboardElement"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

-- ScoreboardPlayerText Component
local TextTexture = ComponentType()
TextTexture.Name = "ScoreboardPlayer"
TextTexture.TableType = TableType.Map
TextTexture.SyncNetwork = true
TextTexture:AddVariable("B", ByteSize.Float)
TextTexture:AddVariable("G", ByteSize.Float)
TextTexture:AddVariable("Name", ByteSize.Text)
TextTexture:AddVariable("R", ByteSize.Float)
TextTexture:AddVariable("IpAddress", ByteSize.Text)
TextTexture:AddVariable("Port", ByteSize.Int)
worldCreator:AddComponentType(TextTexture)