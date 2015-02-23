-- TextTexture Component
local TextTexture = ComponentType()
TextTexture.Name = "TextTexture"
TextTexture.TableType = TableType.Map
TextTexture.SyncNetwork = true
TextTexture:AddVariable("Name", ByteSize.Text)
TextTexture:AddVariable("Text", ByteSize.Text)
TextTexture:AddVariable("FontIndex", ByteSize.Int)
TextTexture:AddVariable("R", ByteSize.Float)
TextTexture:AddVariable("G", ByteSize.Float)
TextTexture:AddVariable("B", ByteSize.Float)
worldCreator:AddComponentType(TextTexture)