--	ButtonCommand Component
local ButtonCommandComponent = ComponentType()
ButtonCommandComponent.Name = "ButtonCommand"
ButtonCommandComponent.TableType = TableType.Map
ButtonCommandComponent:AddVariable("Command", ByteSize.Text)
ButtonCommandComponent:AddVariable("Button", ByteSize.Reference)
ButtonCommandComponent:AddVariable("Index", ByteSize.Int)
worldCreator:AddComponentType(ButtonCommandComponent)

--	ButtonCommand Component
local ButtonComponent = ComponentType()
ButtonComponent.Name = "Button"
ButtonComponent.TableType = TableType.Map
ButtonComponent:AddVariable("NumCommands", ByteSize.Int)
worldCreator:AddComponentType(ButtonComponent)