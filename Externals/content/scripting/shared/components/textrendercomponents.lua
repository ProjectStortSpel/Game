-- TextTexture Component
local TextTexture = ComponentType()
TextTexture.Name = "TextTexture"
TextTexture.TableType = TableType.Map
TextTexture.SyncNetwork = true
TextTexture:AddVariable("B", ByteSize.Float)
TextTexture:AddVariable("FontIndex", ByteSize.Int)
TextTexture:AddVariable("G", ByteSize.Float)
TextTexture:AddVariable("Name", ByteSize.Text)
TextTexture:AddVariable("R", ByteSize.Float)
TextTexture:AddVariable("Text", ByteSize.Text)
worldCreator:AddComponentType(TextTexture)