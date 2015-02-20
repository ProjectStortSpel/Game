-- ServerListEntry Component
local ServerListEntryComponent = ComponentType()
ServerListEntryComponent.Name = "ServerListEntry"
ServerListEntryComponent.TableType = TableType.Map
ServerListEntryComponent:AddVariable("Name", ByteSize.Text)
ServerListEntryComponent:AddVariable("IpAddress", ByteSize.Text)
ServerListEntryComponent:AddVariable("Port", ByteSize.Int)
ServerListEntryComponent:AddVariable("NoUsers", ByteSize.Int)
ServerListEntryComponent:AddVariable("MaxUsers", ByteSize.Int)
ServerListEntryComponent:AddVariable("NoSpectators", ByteSize.Int)
ServerListEntryComponent:AddVariable("GameStarted", ByteSize.Int)
ServerListEntryComponent:AddVariable("PasswordProtected", ByteSize.Int)
ServerListEntryComponent:AddVariable("ButtonId", ByteSize.Int)
worldCreator:AddComponentType(ServerListEntryComponent)