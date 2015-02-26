
-- Connected Player Component
local ClientComponent = ComponentType()
ConnectedPlayer.Name = "Client"
ConnectedPlayer.TableType = TableType.Map
ConnectedPlayer:AddVariable("ClientName", ByteSize.Text)
worldCreator:AddComponentType(ClientComponent)

