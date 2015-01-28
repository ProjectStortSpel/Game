-- TextTexture Component
local TextTexture = ComponentType()
TextTexture.Name = "TextTexture"
TextTexture.TableType = TableType.Map
TextTexture:AddVariable("Name", ByteSize.String)
TextTexture:AddVariable("Text", ByteSize.String)
TextTexture:AddVariable("FontIndex", ByteSize.Int)
TextTexture:AddVariable("R", ByteSize.Float)
TextTexture:AddVariable("G", ByteSize.Float)
TextTexture:AddVariable("B", ByteSize.Float)
worldCreator:AddComponentType(TextTexture)