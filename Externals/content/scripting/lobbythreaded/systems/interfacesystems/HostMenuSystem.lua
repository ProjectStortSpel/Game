HostMenuSystem = System()
HostMenuSystem.Name = "HostMenu"
HostMenuSystem.IsActive = false
HostMenuSystem.TextInput = ""
HostMenuSystem.ActiveTextId = -1

HostMenuSystem.Initialize = function(self)
	self:SetName(self.Name.."System")
	self:UsingUpdate()
	self:UsingEntitiesAdded()
	self:AddComponentTypeToFilter(self.Name, FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter(self.Name.."Element", FilterType.RequiresOneOf)
	
	self:AddComponentTypeToFilter("BoolSetting", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("IntSetting", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("StringSetting", FilterType.RequiresOneOf)
	
	self:AddComponentTypeToFilter("ApplyHostSettings", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("ActiveTextInput", FilterType.RequiresOneOf)
	self.ActiveTextId = -1
end

HostMenuSystem.Update = function(self, dt, taskIndex, taskCount)

	if Input.GetTouchState(0) == InputState.Released then
		local pressedButtons = self:GetEntities("OnPickBoxHit")
		if #pressedButtons > 0 then
			local pressedButton = pressedButtons[1]
			
			if world:EntityHasComponent(pressedButton, "Checkbox") then
				self:CheckboxPressed(pressedButton)
			end
			
			if world:EntityHasComponent(pressedButton, "MenuConsoleCommand") then
				self:MenuConsoleCommandPressed(pressedButton)
			end
			
			if world:EntityHasComponent(pressedButton, "MenuEntityCommand") then
				self:MenuEntityCommandPressed(pressedButton)
			end
		else
			Input.SetTextInput("")
			--Input.StopTextInput()

			self.IsActive = false
			self.TextInput = ""
			self.ActiveTextId  = -1
			self:RemoveMenu()
		end

	end

	self:UpdateText()
	
	
end

HostMenuSystem.UpdateText = function(self)

	if not self.IsActive or self.ActiveTextId == -1 then
		return
	end
	--	
	if Input.GetTextInput() ~= self.TextInput then
		local textSelf = world:GetComponent(self.ActiveTextId, "TextTexture", "Text"):GetText()
	
		local textInput 		= Input.GetTextInput()
		print("textInput: " .. textInput)
		local posX, posY, posZ 	= world:GetComponent(self.ActiveTextId, "Position", 0):GetFloat3()
		local textName			= world:GetComponent(self.ActiveTextId, "TextTexture", "Name"):GetText()
		local settingsName		= world:GetComponent(self.ActiveTextId, "StringSetting", "SettingsName"):GetText()

		world:KillEntity(self.ActiveTextId)
		self.ActiveTextId = self:CreateText("left", "text", posX, posY, posZ, 2.5, 0.065)
		self:AddTextToTexture(textName, textInput .. "_", 0, 0, 0, 0, self.ActiveTextId)
		world:CreateComponentAndAddTo("StringSetting", self.ActiveTextId)
		world:GetComponent(self.ActiveTextId, "StringSetting", "SettingsName"):SetText(settingsName)
		world:GetComponent(self.ActiveTextId, "StringSetting", "Value"):SetText(textInput)
		
		self.TextInput = Input.GetTextInput()
		
	end

end

HostMenuSystem.CheckboxPressed = function(self, entity)

	local posX, posY, posZ = world:GetComponent(entity, "Position", 0):GetFloat3()
	local hoverX, hoverY, _ = world:GetComponent(entity, "HoverSize", 0):GetFloat3()
	
	if world:GetComponent(entity, "Checkbox", "Checked"):GetBool() then
		local cb = self:CreateCheckbox("unchecked", "checkbox", posX, posY, posZ, hoverX,hoverY, false)
		local sname = world:GetComponent(entity, "BoolSetting", "SettingsName"):GetText()
		world:CreateComponentAndAddTo("BoolSetting", cb)
		world:GetComponent(cb, "BoolSetting", "SettingsName"):SetText(sname)
		world:GetComponent(cb, "BoolSetting", "Value"):SetInt(0)
	else
		local cb = self:CreateCheckbox("checked", "checkbox", posX, posY, posZ, hoverX,hoverY, true)
		local sname = world:GetComponent(entity, "BoolSetting", "SettingsName"):GetText()
		world:CreateComponentAndAddTo("BoolSetting", cb)
		world:GetComponent(cb, "BoolSetting", "SettingsName"):SetText(sname)
		world:GetComponent(cb, "BoolSetting", "Value"):SetInt(1)
	end

	world:KillEntity(entity)
	
end

HostMenuSystem.MenuConsoleCommandPressed = function(self, entity)
	local cmd = world:GetComponent(entity, "MenuConsoleCommand", "Command"):GetString()
	self:RemoveMenu()
	Console.AddToCommandQueue(cmd)
end

HostMenuSystem.MenuEntityCommandPressed = function(self, entity)
	local cmp = world:GetComponent(entity, "MenuEntityCommand", "ComponentName"):GetText()
	--self:RemoveMenu()
	print(cmp)
	local id = world:CreateNewEntity()
	world:CreateComponentAndAddTo(cmp, id)
	if world:EntityHasComponent(entity, "BoundToEntity") then
		
		local boundTo = world:GetComponent(entity, "BoundToEntity", "EntityId"):GetInt()
		print("BoundTo: " .. boundTo)
		world:CreateComponentAndAddTo("BoundToEntity", id)
		world:SetComponent(id, "BoundToEntity", "EntityId", boundTo)
	end
end

HostMenuSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

	for i = 1, #entities do
		local entityId = entities[i]
		if world:EntityHasComponent(entityId, self.Name) then
			self:SpawnMenu()
		end
		
		if world:EntityHasComponent(entityId, "ApplyHostSettings") then
			self:ApplySettings(entityId)
		end
		
		if world:EntityHasComponent(entityId, "ActiveTextInput") then
			self:Deactivate()
			self.ActiveTextId = world:GetComponent(entityId, "BoundToEntity", "EntityId"):GetInt()
			self:Activate()
			
			
		end
		
	end

end

HostMenuSystem.Activate = function(self)
	local text	= world:GetComponent(self.ActiveTextId, "TextTexture", "Text"):GetText()

	Input.StartTextInput()
	Input.SetTextInput(text)
	
	self.IsActive = true
	self.TextInput = ""
	self.tmp = true
end

HostMenuSystem.Deactivate = function(self)
	Input.SetTextInput("")
	Input.StopTextInput()

	self.IsActive = false
	self.TextInput = ""
	self.ActiveTextId  = -1
	
	if self.ActiveTextId ~= -1 then
	
		print("textInput: " .. "HEJ")
		local posX, posY, posZ 	= world:GetComponent(self.ActiveTextId, "Position", 0):GetFloat3()
		local textName			= world:GetComponent(self.ActiveTextId, "TextTexture", "Name"):GetText()
		local text				= world:GetComponent(self.ActiveTextId, "TextTexture", "Text"):GetText()
		local settingsName		= world:GetComponent(self.ActiveTextId, "StringSetting", "SettingsName"):GetText()
		text = string.sub(text, 0, string.len(text)-1)
		print(text)
		
		world:KillEntity(self.ActiveTextId)
		self.ActiveTextId = self:CreateText("left", "text", posX, posY, posZ, 2.5, 0.065)
		self:AddTextToTexture(textName, text, 0, 0, 0, 0, self.ActiveTextId)
		world:CreateComponentAndAddTo("StringSetting", self.ActiveTextId)
		world:GetComponent(self.ActiveTextId, "StringSetting", "SettingsName"):SetText(settingsName)
		world:GetComponent(self.ActiveTextId, "StringSetting", "Value"):SetText(text)
	
	end
	
	
	
	
	self.ActiveTextId  = -1
	
end

HostMenuSystem.ApplySettings = function(self, entity)

	local entities = self:GetEntities()
	local e = world:CreateNewEntity()
	world:CreateComponentAndAddTo("HostSettings", e)
	
	for i = 1, #entities do
		
		if world:EntityHasComponent(entities[i], "BoolSetting") then
			local name = world:GetComponent(entities[i], "BoolSetting", "SettingsName"):GetText()
			local value = world:GetComponent(entities[i], "BoolSetting", "Value"):GetInt()
			world:GetComponent(e, "HostSettings", name):SetInt(value)
			
		elseif world:EntityHasComponent(entities[i], "IntSetting") then
			local name = world:GetComponent(entities[i], "IntSetting", "SettingsName"):GetText()
			local value = world:GetComponent(entities[i], "IntSetting", "Value"):GetInt()
			world:GetComponent(e, "HostSettings", name):SetInt(value)
			
		elseif world:EntityHasComponent(entities[i], "StringSetting") then
			local name = world:GetComponent(entities[i], "StringSetting", "SettingsName"):GetText()
			local value = world:GetComponent(entities[i], "StringSetting", "Value"):GetText()
			world:GetComponent(e, "HostSettings", name):SetText(value)
			
		end
		
	end

	local name 			= world:GetComponent(e, "HostSettings", "Name"):GetText()
	local password 		= world:GetComponent(e, "HostSettings", "Password"):GetText()
	local map 			= world:GetComponent(e, "HostSettings", "Map"):GetText()
	local gamemode 		= world:GetComponent(e, "HostSettings", "GameMode"):GetText()
	local port 			= world:GetComponent(e, "HostSettings", "Port"):GetInt()
	--local maxusers 	= world:GetComponent(e, "HostSettings", "MaxUsers"):GetInt()
	local fillai 		= world:GetComponent(e, "HostSettings", "FillAI"):GetInt()
	local allowSpec		= world:GetComponent(e, "HostSettings", "AllowSpectators"):GetInt()
	local serverType	= world:GetComponent(e, "HostSettings", "ServerType"):GetInt()
	
	print("name: " .. name)
	print("password: " .. password)
	print("map: " .. map)
	print("gamemode: " .. gamemode)
	print("port: " .. port)
	print("fillai: " .. tostring(fillai))
	print("allowSpec: " .. tostring(allowSpec))
	print("serverType: " .. tostring(serverType))
	
	self:RemoveMenu()
	local cmd = string.format("hostsettings %s %s %s %s %d %d %d %d", name, password, map, gamemode, port, fillai, allowSpec, serverType)
	Console.AddToCommandQueue(cmd)
	
end

HostMenuSystem.SpawnMenu = function(self)

	self:CreateElement("gamemenubackground", "quad", 0, 0, -2.5, 2.3, 2.8)
	--self:CreateElement("host", "quad", 0, 0.67, -2, 0.6, 0.3)
	
	--self:CreateCheckbox("unchecked", "checkbox", 0, 0.25, -2, 0.15, 0.15, false)
	
	-- BUTTONS
	
	button = self:CreateButton("host", "quad", 0.0, -0.84, -2, 0.50, 0.24)
	self:AddEntityCommandToButton("ApplyHostSettings", button)
	
	
	-- SETTINGS TEXT
	
	local text = self:CreateText("left", "text", -0.87, 0.790, -1.99999, 2.5, 0.065)	
	self:AddTextToTexture("A"..1, "Server Name:", 0, 0, 0, 0, text)
	self:AddEntityCommandToButton("ActiveTextInput", text)
		
	text = self:CreateText("left", "text", -0.87, 0.685, -1.99999, 2.5, 0.065)	
	self:AddTextToTexture("A"..2, "Server Password:", 0, 0, 0, 0, text)
	--self:AddEntityCommandToButton("ActiveTextInput", text)
	
	text = self:CreateText("left", "text", -0.87, 0.580, -1.99999, 2.5, 0.065)	
	self:AddTextToTexture("A"..3, "Server Game Mode:", 0, 0, 0, 0, text)

	
	text = self:CreateText("left", "text", -0.87, 0.475, -1.99999, 2.5, 0.065)	
	self:AddTextToTexture("A"..4, "Server Port:", 0, 0, 0, 0, text)
	
	text = self:CreateText("left", "text", -0.87, 0.370, -1.99999, 2.5, 0.065)	
	self:AddTextToTexture("A"..5, "Server Map:", 0, 0, 0, 0, text)
	
	text = self:CreateText("left", "text", -0.87, 0.265, -1.99999, 2.5, 0.065)	
	self:AddTextToTexture("A"..6, "Fill empty slots with AIs:", 0, 0, 0, 0, text)
	
	text = self:CreateText("left", "text", -0.87, 0.160, -1.99999, 2.5, 0.065)	
	self:AddTextToTexture("A"..7, "Allow Spectators: ", 0, 0, 0, 0, text)
	
	text = self:CreateText("left", "text", -0.87, 0.055, -1.99999, 2.5, 0.065)	
	self:AddTextToTexture("A"..8, "Dedicated Server: ", 0, 0, 0, 0, text)
	
	
	-- USER SETTINGS
	
	-- Name TEXT
	text = self:CreateText("left", "text", 0.16, 0.790, -1.99999, 5.5, 0.065)	
	self:AddTextToTexture("B"..1, "DefaultName", 0, 0, 0, 0, text)
	world:CreateComponentAndAddTo("StringSetting", text)
	world:GetComponent(text, "StringSetting", "SettingsName"):SetText("Name")
	world:GetComponent(text, "StringSetting", "Value"):SetText("DefaultName")
	local backdrop = self:CreateElement("whiteshade", "quad", 0.52, 0.760, -2, 0.73, 0.09)
	self:AddEntityCommandToButton("ActiveTextInput", backdrop, text)
	
	-- Password TEXT
	text = self:CreateText("left", "text", 0.16, 0.685, -1.99999, 2.5, 0.065)	
	self:AddTextToTexture("B"..2, "EmptyPassword", 0, 0, 0, 0, text)
	world:CreateComponentAndAddTo("StringSetting", text)
	world:GetComponent(text, "StringSetting", "SettingsName"):SetText("Password")
	world:GetComponent(text, "StringSetting", "Value"):SetText("default")
	backdrop = self:CreateElement("whiteshade", "quad", 0.52, 0.655, -2, 0.73, 0.09)
	self:AddEntityCommandToButton("ActiveTextInput", backdrop, text)
	
	-- GameMode TEXT
	text = self:CreateText("left", "text", 0.16, 0.580, -1.99999, 2.5, 0.065)	
	self:AddTextToTexture("B"..3, "storaspelthreaded", 0, 0, 0, 0, text)
	world:CreateComponentAndAddTo("StringSetting", text)
	world:GetComponent(text, "StringSetting", "SettingsName"):SetText("GameMode")
	world:GetComponent(text, "StringSetting", "Value"):SetText("storaspelthreaded")
	backdrop = self:CreateElement("whiteshade", "quad", 0.52, 0.550, -2, 0.73, 0.09)
	self:AddEntityCommandToButton("ActiveTextInput", backdrop, text)
	
	-- Port INT
	text = self:CreateText("left", "text", 0.16, 0.475, -1.99999, 2.5, 0.065)	
	self:AddTextToTexture("B"..4, "6112", 0, 0, 0, 0, text)
	world:CreateComponentAndAddTo("IntSetting", text)
	world:GetComponent(text, "IntSetting", "SettingsName"):SetText("Port")
	world:GetComponent(text, "IntSetting", "Value"):SetInt(6112)
	backdrop = self:CreateElement("whiteshade", "quad", 0.52, 0.445, -2, 0.73, 0.09)
	self:AddEntityCommandToButton("ActiveTextInput", backdrop, text)
	
	-- GameMode TEXT
	text = self:CreateText("left", "text", 0.16, 0.370, -1.99999, 2.5, 0.065)	
	self:AddTextToTexture("B"..5, "map", 0, 0, 0, 0, text)
	world:CreateComponentAndAddTo("StringSetting", text)
	world:GetComponent(text, "StringSetting", "SettingsName"):SetText("Map")
	world:GetComponent(text, "StringSetting", "Value"):SetText("map")
	backdrop = self:CreateElement("whiteshade", "quad", 0.52, 0.340, -2, 0.73, 0.09)
	self:AddEntityCommandToButton("ActiveTextInput", backdrop, text)
	
	-- FillAI CHECKBOX
	local cb = self:CreateCheckbox("unchecked", "checkbox", 0.2, 0.225, -1.99999, 0.07, 0.07, false)
	world:CreateComponentAndAddTo("BoolSetting", cb)
	world:GetComponent(cb, "BoolSetting", "SettingsName"):SetText("FillAI")
	world:GetComponent(cb, "BoolSetting", "Value"):SetInt(0)
	
	-- AllowSpectators CHECKBOX
	local cb = self:CreateCheckbox("checked", "checkbox", 0.2, 0.120, -1.99999, 0.07, 0.07, true)
	world:CreateComponentAndAddTo("BoolSetting", cb)
	world:GetComponent(cb, "BoolSetting", "SettingsName"):SetText("AllowSpectators")
	world:GetComponent(cb, "BoolSetting", "Value"):SetInt(1)
	
	-- ServerType CHECKBOX
	local cb = self:CreateCheckbox("unchecked", "checkbox", 0.2, 0.015, -1.99999, 0.07, 0.07, false)
	world:CreateComponentAndAddTo("BoolSetting", cb)
	world:GetComponent(cb, "BoolSetting", "SettingsName"):SetText("ServerType")
	world:GetComponent(cb, "BoolSetting", "Value"):SetInt(0)
	
end

HostMenuSystem.RemoveMenu = function(self)

	local entities = self:GetEntities()
	for i = 1, #entities do
		world:KillEntity(entities[i])
	end

end

HostMenuSystem.CreateButton = function(self, object, folder, posX, posY, posZ, scaleX, scaleY)

	local id = world:CreateNewEntity("Button")
	world:CreateComponentAndAddTo(self.Name.."Element", id)
	world:GetComponent(id, "Model", 0):SetModel(object, folder, 2)
	world:GetComponent(id, "Position", 0):SetFloat3(posX, posY, posZ)
	world:GetComponent(id, "Scale", 0):SetFloat3(scaleX, scaleY, 1)
	world:GetComponent(id, "PickBox", 0):SetFloat2(1, 1)
	world:GetComponent(id, "Rotation", 0):SetFloat3(0, 0, 0)
	self:AddHoverSize(1.1, id)
	return id	
	
end

HostMenuSystem.CreateElement = function(self, object, folder, posX, posY, posZ, scaleX, scaleY)

	local id = world:CreateNewEntity("Button")
	world:CreateComponentAndAddTo(self.Name.."Element", id)
	world:GetComponent(id, "Model", 0):SetModel(object, folder, 2)
	world:GetComponent(id, "Position", 0):SetFloat3(posX, posY, posZ)
	world:GetComponent(id, "Scale", 0):SetFloat3(scaleX, scaleY, 1)
	world:GetComponent(id, "PickBox", 0):SetFloat2(1, 1)
	world:GetComponent(id, "Rotation", 0):SetFloat3(0, 0, 0)
	return id	

end

HostMenuSystem.CreateCheckbox = function(self, object, folder, posX, posY, posZ, scaleX, scaleY, checked)
	
		local id = world:CreateNewEntity("Checkbox")
		world:CreateComponentAndAddTo(self.Name.."Element", id)
		world:GetComponent(id, "Model", 0):SetModel(object, folder, 2)
		world:GetComponent(id, "Position", 0):SetFloat3(posX, posY, posZ)
		world:GetComponent(id, "Scale", 0):SetFloat3(scaleX, scaleY, 1)
		world:GetComponent(id, "PickBox", 0):SetFloat2(1, 1)
		world:GetComponent(id, "Rotation", 0):SetFloat3(0, 0, 0)
		world:GetComponent(id, "Checkbox", "Checked"):SetBool(checked)
		self:AddHoverSize(1.1, id)
		return id
end

HostMenuSystem.CreateText = function(self, object, folder, posx, posy, posz, scalex, scaley)

	local id = world:CreateNewEntity("Text")
	world:CreateComponentAndAddTo(self.Name.."Element", id)
	world:GetComponent(id, "Model", 0):SetModel(object, folder, 2)
	world:GetComponent(id, "Position", 0):SetFloat3(posx, posy, posz)
	world:GetComponent(id, "Scale", 0):SetFloat3(scalex, scaley, 1)
	world:GetComponent(id, "Rotation", 0):SetFloat3(0, 0, 0)
	
	return id	
end

HostMenuSystem.AddTextToTexture = function(self, n, text, font, r, g, b, button)
	world:CreateComponentAndAddTo("TextTexture", button)
	world:GetComponent(button, "TextTexture", "Name"):SetText(n) -- TODO: NAME CANT BE MORE THAN 3 CHARS? WTF?
	world:GetComponent(button, "TextTexture", "Text"):SetText(text)
	world:GetComponent(button, "TextTexture", "FontIndex"):SetInt(font)
	world:GetComponent(button, "TextTexture", "R"):SetFloat(r)
	world:GetComponent(button, "TextTexture", "G"):SetFloat(g)
	world:GetComponent(button, "TextTexture", "B"):SetFloat(b)
end

HostMenuSystem.AddConsoleCommandToButton = function(self, command, button)

	world:CreateComponentAndAddTo("MenuConsoleCommand", button)
	world:GetComponent(button, "MenuConsoleCommand", "Command"):SetString(command)
	
end

HostMenuSystem.AddEntityCommandToButton = function(self, command, button, bindedTo)
	world:CreateComponentAndAddTo("MenuEntityCommand", button)
	world:GetComponent(button, "MenuEntityCommand", "ComponentName"):SetText(command)
	
	if bindedTo ~= nil then
		print("AddEntityCommandToButton: " .. bindedTo)
		world:CreateComponentAndAddTo("BoundToEntity", button)
		world:SetComponent(button, "BoundToEntity", "EntityId", bindedTo)
	
	end
	
end

HostMenuSystem.AddHoverSize = function(self, deltascale, button)

	local sx, sy, sz = world:GetComponent(button, "Scale", 0):GetFloat3()
	world:CreateComponentAndAddTo("HoverSize", button)
	world:GetComponent(button, "HoverSize", 0):SetFloat3(sx*deltascale, sy*deltascale, sz*deltascale)
	
end