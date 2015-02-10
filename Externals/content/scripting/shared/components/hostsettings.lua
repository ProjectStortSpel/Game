-- HostSettings Component
local HostSettingsComponent = ComponentType()
HostSettingsComponent.Name = "HostSettings"
HostSettingsComponent.TableType = TableType.Map
HostSettingsComponent:AddVariable("Name", ByteSize.Text)
HostSettingsComponent:AddVariable("Password", ByteSize.Text)
HostSettingsComponent:AddVariable("Password", ByteSize.Text)
HostSettingsComponent:AddVariable("Map", ByteSize.Text)
HostSettingsComponent:AddVariable("GameMode", ByteSize.Text)
HostSettingsComponent:AddVariable("Port", ByteSize.Int)
HostSettingsComponent:AddVariable("MaxUsers", ByteSize.Int)
HostSettingsComponent:AddVariable("FillAI", ByteSize.Int)
worldCreator:AddComponentType(HostSettingsComponent)

-- BoolSetting Component
local BoolSettingComponent = ComponentType()
BoolSettingComponent.Name = "BoolSetting"
BoolSettingComponent.TableType = TableType.Map
BoolSettingComponent:AddVariable("SettingsName", ByteSize.Text)
BoolSettingComponent:AddVariable("Value", ByteSize.Bool)
worldCreator:AddComponentType(BoolSettingComponent)

-- IntSetting Component
local IntSettingComponent = ComponentType()
IntSettingComponent.Name = "IntSetting"
IntSettingComponent.TableType = TableType.Map
IntSettingComponent:AddVariable("SettingsName", ByteSize.Text)
IntSettingComponent:AddVariable("Value", ByteSize.Int)
worldCreator:AddComponentType(IntSettingComponent)

-- StringSetting Component
local StringSettingComponent = ComponentType()
StringSettingComponent.Name = "StringSetting"
StringSettingComponent.TableType = TableType.Map
StringSettingComponent:AddVariable("SettingsName", ByteSize.Text)
StringSettingComponent:AddVariable("Value", ByteSize.Text)
worldCreator:AddComponentType(StringSettingComponent)


-- ApplyHostSettings Component
local ApplyHostSettingsComponent = ComponentType()
ApplyHostSettingsComponent.Name = "ApplyHostSettings"
ApplyHostSettingsComponent.TableType = TableType.Map
worldCreator:AddComponentType(ApplyHostSettingsComponent)