HostMenuSystem = System()
HostMenuSystem.Name = "HostMenu"
HostMenuSystem.IsMenuActive = false
HostMenuSystem.IsTextBoxActive = false
HostMenuSystem.ActiveTextId = -1
HostMenuSystem.RequestRelease = false

HostMenuSystem.Initialize = function(self)
	self:SetName(self.Name.."System")
	self:UsingUpdate()
	self:UsingEntitiesAdded()
	
	self:AddComponentTypeToFilter(self.Name, FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter(self.Name.."Element", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter(self.Name.."Activate", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("ActiveTextInput", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("ApplyHostSettings", FilterType.RequiresOneOf)
	
end

HostMenuSystem.EntitiesAdded = function(self, dt, entities)

	
	for i = 1, #entities do
		local entity = entities[i]
		
		if world:EntityHasComponent(entity, self.Name) then
			self.IsMenuActive  = true
			self:SpawnMenu()
			world:KillEntity(entity)
		elseif world:EntityHasComponent(entity, "ApplyHostSettings") then
			self:Deactivate()
			self:ApplySettings(entityId)
			world:KillEntity(entity)
		elseif world:EntityHasComponent(entity, "ActiveTextInput") then
			self:Deactivate(entity)
			self:Activate(entity)
			world:KillEntity(entity)
		elseif world:EntityHasComponent(entity, self.Name .. "Activate") then
			self.IsMenuActive = true
			world:KillEntity(entity)
		end
		
	end
		
end

HostMenuSystem.Update = function(self, dt)

	if not self.IsMenuActive then
		return
	end

	if self.RequestRelease then
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
			self:Deactivate()
			self:RemoveMenu()
		end
	end
	
	self:UpdateText()
	
	if Input.GetTouchState(0) == InputState.Released then
		self.RequestRelease = true
	else
		self.RequestRelease = false
	end
end

HostMenuSystem.UpdateText = function(self)

	if not self.IsTextBoxActive or self.ActiveTextId == -1 then
		return
	end

	local textInput = Input.GetTextInput()
	local text = world:GetComponent(self.ActiveTextId, "TextTexture", "Text"):GetText()
	text = string.sub(text, 1, string.len(text) - 1 )
	
	if text ~= textInput then
		
		local textId = self.ActiveTextId
		
		local textName 		= world:GetComponent(textId, "TextTexture", "Name"):GetText()
		local boundBackdrop = world:GetComponent(textId, "BoundToEntity", "EntityId"):GetInt()
		local X, Y, Z 		= world:GetComponent(textId, "Position", 0):GetFloat3()
		
		world:KillEntity(textId) -- Kill the old text
		self.ActiveTextId = self:CreateText("left", "text", X, Y, Z, 2.5, 0.065, boundBackdrop)
		self:AddTextToTexture(textName, textInput .. "_", 0,0,0,0, self.ActiveTextId)
		world:SetComponent(boundBackdrop, "BoundToEntity", "EntityId", self.ActiveTextId)
		
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
	local id = world:CreateNewEntity()
	world:CreateComponentAndAddTo(cmp, id)
	
	if world:EntityHasComponent(entity, "BoundToEntity") then
		
		local boundTo = world:GetComponent(entity, "BoundToEntity", "EntityId"):GetInt()
		world:CreateComponentAndAddTo("BoundToEntity", id)
		world:SetComponent(id, "BoundToEntity", "EntityId", boundTo)
		
		--print("Pressed on a backdrop which has text#" .. boundTo .. " bounded to it")
		
	end
	
end

HostMenuSystem.Activate = function(self, entity)
	
	local textId = world:GetComponent(entity, "BoundToEntity", "EntityId"):GetInt() -- Get textId
	
	local text 			= world:GetComponent(textId, "TextTexture", "Text"):GetText()
	local textName 		= world:GetComponent(textId, "TextTexture", "Name"):GetText()
	local boundBackdrop = world:GetComponent(textId, "BoundToEntity", "EntityId"):GetInt()
	local X, Y, Z 		= world:GetComponent(textId, "Position", 0):GetFloat3()

	Input.StartTextInput()
	Input.SetTextInput(text)
	print("Activate")
	
	--print("Activate textInput on text with backdrop#" .. boundBackdrop .. " bound to it")
	
	world:KillEntity(textId) -- Kill the old text
	self.ActiveTextId = self:CreateText("left", "text", X, Y, Z, 2.5, 0.065, boundBackdrop)
	self:AddTextToTexture(textName, text .. "_", 0,0,0,0, self.ActiveTextId)
	world:SetComponent(boundBackdrop, "BoundToEntity", "EntityId", self.ActiveTextId)
	
	
	
	self.IsTextBoxActive = true
	
end

HostMenuSystem.Deactivate = function(self, entity)

	Input.StopTextInput()

	if self.ActiveTextId ~= -1 and entity ~= nil then
		
		local textId = self.ActiveTextId
	
		local text 			= world:GetComponent(textId, "TextTexture", "Text"):GetText()
		local textName 		= world:GetComponent(textId, "TextTexture", "Name"):GetText()
		local boundBackdrop = world:GetComponent(textId, "BoundToEntity", "EntityId"):GetInt()
		local X, Y, Z 		= world:GetComponent(textId, "Position", 0):GetFloat3()
		text = string.sub(text, 1, string.len(text) - 1)

		world:KillEntity(textId) -- Kill the old text
		self.ActiveTextId = self:CreateText("left", "text", X, Y, Z, 2.5, 0.065, boundBackdrop)
		self:AddTextToTexture(textName, text, 0,0,0,0, self.ActiveTextId)
		world:SetComponent(boundBackdrop, "BoundToEntity", "EntityId", self.ActiveTextId)
	
	end
	
	
	self.IsTextBoxActive = false
	self.ActiveTextId = -1
	
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

	--local popup = world:CreateNewEntity()
	--world:CreateComponentAndAddTo("NotificationBox", popup)
	--world:SetComponent(popup, "NotificationBox", "Text", "TEMP TEMP TEMP TEMP")
	--world:SetComponent(popup, "NotificationBox", "ParentMenu", self.Name)
	self.IsMenuActive = true

	--local background = self:CreateElement("gamemenubackground", "quad", 0, 0, -2.7, 3.0, 3.0)
	
	-- BUTTONS
	
	button = self:CreateButton("host", "quad", 0.0, -0.84, -2, 0.50, 0.24)
	self:AddEntityCommandToButton("ApplyHostSettings", button)
	
	-- SETTINGS TEXT
	
	local text = self:CreateText("left", "text", -0.87, 0.580, -1.99, 2.5, 0.065)	
	self:AddTextToTexture("A"..1, "Server Name:", 0, 0, 0, 0, text)
	--self:AddEntityCommandToButton("ActiveTextInput", text)
		
	text = self:CreateText("left", "text", -0.87, 0.475, -1.99, 2.5, 0.065)	
	self:AddTextToTexture("A"..2, "Server Password:", 0, 0, 0, 0, text)
	--self:AddEntityCommandToButton("ActiveTextInput", text)
	
	text = self:CreateText("left", "text", -0.87, 0.370, -1.99, 2.5, 0.065)	
	self:AddTextToTexture("A"..3, "Server Game Mode:", 0, 0, 0, 0, text)

	
	text = self:CreateText("left", "text", -0.87, 0.265, -1.99, 2.5, 0.065)	
	self:AddTextToTexture("A"..4, "Server Port:", 0, 0, 0, 0, text)
	
	text = self:CreateText("left", "text", -0.87, 0.160, -1.99, 2.5, 0.065)	
	self:AddTextToTexture("A"..5, "Server Map:", 0, 0, 0, 0, text)
	
	text = self:CreateText("left", "text", -0.87, -0.055, -1.99, 2.5, 0.065)	
	self:AddTextToTexture("A"..6, "Fill empty slots with AIs:", 0, 0, 0, 0, text)
	
	text = self:CreateText("left", "text", -0.87, -0.160, -1.99, 2.5, 0.065)	
	self:AddTextToTexture("A"..7, "Allow Spectators: ", 0, 0, 0, 0, text)
	
	text = self:CreateText("left", "text", -0.87, -0.255, -1.99, 2.5, 0.065)	
	self:AddTextToTexture("A"..8, "Dedicated Server: ", 0, 0, 0, 0, text)
	
	
	
	
	-- USER SETTINGS
	
	-- Name TEXT
	local backdrop = self:CreateElement("whiteshade", "quad", 0.52, 0.580, -2, 0.73, 0.09)
	text = self:CreateText("left", "text", 0.16, 0.610, -1.99, 5.5, 0.065, backdrop)	
	self:AddTextToTexture("B"..1, "DefaultName", 0, 0, 0, 0, text)
	
	world:CreateComponentAndAddTo("StringSetting", text)
	world:GetComponent(text, "StringSetting", "SettingsName"):SetText("Name")
	world:GetComponent(text, "StringSetting", "Value"):SetText("DefaultName")
	self:AddEntityCommandToButton("ActiveTextInput", backdrop, text)
		
	-- Password TEXT
	backdrop = self:CreateElement("whiteshade", "quad", 0.52, 0.475, -2, 0.73, 0.09)
	text = self:CreateText("left", "text", 0.16, 0.515, -1.99, 2.5, 0.065, backdrop)	
	self:AddTextToTexture("B"..2, "EmptyPassword", 0, 0, 0, 0, text)
	world:CreateComponentAndAddTo("StringSetting", text)
	world:GetComponent(text, "StringSetting", "SettingsName"):SetText("Password")
	world:GetComponent(text, "StringSetting", "Value"):SetText("default")
	self:AddEntityCommandToButton("ActiveTextInput", backdrop, text)
	
	-- GameMode TEXT
	backdrop = self:CreateElement("whiteshade", "quad", 0.52, 0.370, -2, 0.73, 0.09)
	text = self:CreateText("left", "text", 0.16, 0.410, -1.99, 2.5, 0.065, backdrop)	
	self:AddTextToTexture("B"..3, "storaspelthreaded", 0, 0, 0, 0, text)
	world:CreateComponentAndAddTo("StringSetting", text)
	world:GetComponent(text, "StringSetting", "SettingsName"):SetText("GameMode")
	world:GetComponent(text, "StringSetting", "Value"):SetText("storaspelthreaded")
	self:AddEntityCommandToButton("ActiveTextInput", backdrop, text)
	
	-- Port INT
	backdrop = self:CreateElement("whiteshade", "quad", 0.52, 0.265, -2, 0.73, 0.09)
	text = self:CreateText("left", "text", 0.16, 0.3, -1.99, 2.5, 0.065, backdrop)	
	self:AddTextToTexture("B"..4, "6112", 0, 0, 0, 0, text)
	world:CreateComponentAndAddTo("IntSetting", text)
	world:GetComponent(text, "IntSetting", "SettingsName"):SetText("Port")
	world:GetComponent(text, "IntSetting", "Value"):SetInt(6112)
	self:AddEntityCommandToButton("ActiveTextInput", backdrop, text)
	
	-- GameMode TEXT
	backdrop = self:CreateElement("whiteshade", "quad", 0.52, 0.160, -2, 0.73, 0.09)
	text = self:CreateText("left", "text", 0.16, 0.20, -1.99, 2.5, 0.065, backdrop)	
	self:AddTextToTexture("B"..5, "map", 0, 0, 0, 0, text)
	world:CreateComponentAndAddTo("StringSetting", text)
	world:GetComponent(text, "StringSetting", "SettingsName"):SetText("Map")
	world:GetComponent(text, "StringSetting", "Value"):SetText("map")
	self:AddEntityCommandToButton("ActiveTextInput", backdrop, text)
	
	-- FillAI CHECKBOX
	local cb = self:CreateCheckbox("unchecked", "checkbox", 0.2, -0.09, -1.99, 0.07, 0.07, false)
	world:CreateComponentAndAddTo("BoolSetting", cb)
	world:GetComponent(cb, "BoolSetting", "SettingsName"):SetText("FillAI")
	world:GetComponent(cb, "BoolSetting", "Value"):SetInt(0)
	
	-- AllowSpectators CHECKBOX
	local cb = self:CreateCheckbox("checked", "checkbox", 0.2, -0.190, -1.99, 0.07, 0.07, true)
	world:CreateComponentAndAddTo("BoolSetting", cb)
	world:GetComponent(cb, "BoolSetting", "SettingsName"):SetText("AllowSpectators")
	world:GetComponent(cb, "BoolSetting", "Value"):SetInt(1)
	
	-- ServerType CHECKBOX
	local cb = self:CreateCheckbox("unchecked", "checkbox", 0.2, -0.3, -1.99, 0.07, 0.07, false)
	world:CreateComponentAndAddTo("BoolSetting", cb)
	world:GetComponent(cb, "BoolSetting", "SettingsName"):SetText("ServerType")
	world:GetComponent(cb, "BoolSetting", "Value"):SetInt(0)
	
	
	
end

HostMenuSystem.RemoveMenu = function(self)

	local entities = self:GetEntities()
	for i = 1, #entities do
		world:KillEntity(entities[i])
	end
	
	local toconnectmenu = world:CreateNewEntity()
	world:CreateComponentAndAddTo("ConnectMenu", toconnectmenu)
	
	self.IsMenuActive = false

end

HostMenuSystem.CreateButton = function(self, object, folder, posX, posY, posZ, scaleX, scaleY)

	local id = world:CreateNewEntity("Button")
	world:CreateComponentAndAddTo(self.Name.."Element", id)
	world:GetComponent(id, "Model", 0):SetModel(object, folder, 3)
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
	world:GetComponent(id, "Model", 0):SetModel(object, folder, 3)
	world:GetComponent(id, "Position", 0):SetFloat3(posX, posY, posZ)
	world:GetComponent(id, "Scale", 0):SetFloat3(scaleX, scaleY, 1)
	world:GetComponent(id, "PickBox", 0):SetFloat2(1, 1)
	world:GetComponent(id, "Rotation", 0):SetFloat3(0, 0, 0)
	return id	

end

HostMenuSystem.CreateCheckbox = function(self, object, folder, posX, posY, posZ, scaleX, scaleY, checked)

		local id = world:CreateNewEntity("Checkbox")
		world:CreateComponentAndAddTo(self.Name.."Element", id)
		world:GetComponent(id, "Model", 0):SetModel(object, folder, 3)
		world:GetComponent(id, "Position", 0):SetFloat3(posX, posY, posZ)
		world:GetComponent(id, "Scale", 0):SetFloat3(scaleX, scaleY, 1)
		world:GetComponent(id, "PickBox", 0):SetFloat2(1, 1)
		world:GetComponent(id, "Rotation", 0):SetFloat3(0, 0, 0)
		world:GetComponent(id, "Checkbox", "Checked"):SetBool(checked)
		self:AddHoverSize(1.1, id)
		return id

end

HostMenuSystem.CreateText = function(self, object, folder, posx, posy, posz, scalex, scaley, boundTo)

	local id = world:CreateNewEntity("Text")
	world:CreateComponentAndAddTo(self.Name.."Element", id)
	world:GetComponent(id, "Model", 0):SetModel(object, folder, 3)
	world:GetComponent(id, "Position", 0):SetFloat3(posx, posy, posz)
	world:GetComponent(id, "Scale", 0):SetFloat3(scalex, scaley, 1)
	world:GetComponent(id, "Rotation", 0):SetFloat3(0, 0, 0)
	
	if boundTo ~= nil then
		--print("Bound text#" .. id .. " to entity: " .. boundTo)
		world:CreateComponentAndAddTo("BoundToEntity", id)
		world:SetComponent(id, "BoundToEntity", "EntityId", boundTo)
	end
	
	
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

HostMenuSystem.AddEntityCommandToButton = function(self, command, button, boundTo)

	world:CreateComponentAndAddTo("MenuEntityCommand", button)
	world:GetComponent(button, "MenuEntityCommand", "ComponentName"):SetText(command)
	
	
	if boundTo ~= nil then
		print("Bound backdrop#" .. button .. " to entity: " .. boundTo)
		world:CreateComponentAndAddTo("BoundToEntity", button)
		world:SetComponent(button, "BoundToEntity", "EntityId", boundTo)
	
	end
	
	
end

HostMenuSystem.AddHoverSize = function(self, deltascale, button)

	local sx, sy, sz = world:GetComponent(button, "Scale", 0):GetFloat3()
	world:CreateComponentAndAddTo("HoverSize", button)
	world:GetComponent(button, "HoverSize", 0):SetFloat3(sx*deltascale, sy*deltascale, sz*deltascale)

end