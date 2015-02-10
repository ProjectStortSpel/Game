HostMenuSystem = System()
HostMenuSystem.Name = "HostMenu"

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
			
		end
	end

end

HostMenuSystem.CheckboxPressed = function(self, entity)

	local posX, posY, posZ = world:GetComponent(entity, "Position", 0):GetFloat3()
	local hoverX, hoverY, _ = world:GetComponent(entity, "HoverSize", 0):GetFloat3()
	
	if world:GetComponent(entity, "Checkbox", "Checked"):GetBool() then
		local cb = self:CreateCheckbox("unchecked", "checkbox", posX, posY, posZ, hoverX,hoverY, false)
		local sname = world:GetComponent(entity, "BoolSetting", "SettingsName"):GetString()
		print(sname)
		world:CreateComponentAndAddTo("BoolSetting", cb)
		world:GetComponent(cb, "BoolSetting", "SettingsName"):SetString(sname)
		world:GetComponent(cb, "BoolSetting", "Value"):SetInt(0)
	else
		local cb = self:CreateCheckbox("checked", "checkbox", posX, posY, posZ, hoverX,hoverY, true)
		local sname = world:GetComponent(entity, "BoolSetting", "SettingsName"):GetString()
		print(sname)
		world:CreateComponentAndAddTo("BoolSetting", cb)
		world:GetComponent(cb, "BoolSetting", "SettingsName"):SetString(sname)
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
	local cmp = world:GetComponent(entity, "MenuEntityCommand", "ComponentName"):GetString()
	--self:RemoveMenu()
	print(cmp)
	local id = world:CreateNewEntity()
	world:CreateComponentAndAddTo(cmp, id)
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
		
		
	end

end

HostMenuSystem.ApplySettings = function(self, entity)

	local entities = self:GetEntities()
	local e = world:CreateNewEntity()
	world:CreateComponentAndAddTo("HostSettings", e)
	
	for i = 1, #entities do
		
		if world:EntityHasComponent(entities[i], "BoolSetting") then
			local name = world:GetComponent(entities[i], "BoolSetting", "SettingsName"):GetString()
			local value = world:GetComponent(entities[i], "BoolSetting", "Value"):GetInt()
			world:GetComponent(e, "HostSettings", name):SetInt(value)
			
		elseif world:EntityHasComponent(entities[i], "IntSetting") then
			local name = world:GetComponent(entities[i], "IntSetting", "SettingsName"):GetString()
			local value = world:GetComponent(entities[i], "IntSetting", "Value"):GetInt()
			world:GetComponent(e, "HostSettings", name):SetInt(value)
			
		elseif world:EntityHasComponent(entities[i], "StringSetting") then
			local name = world:GetComponent(entities[i], "StringSetting", "SettingsName"):GetString()
			local value = world:GetComponent(entities[i], "StringSetting", "Value"):GetString()
			world:GetComponent(e, "HostSettings", name):SetString(value)
			
		end
		
	end

	local name 		= world:GetComponent(e, "HostSettings", "Name"):GetString()
	local password 	= world:GetComponent(e, "HostSettings", "Password"):GetString()
	local map 		= world:GetComponent(e, "HostSettings", "Map"):GetString()
	local gamemode 	= world:GetComponent(e, "HostSettings", "GameMode"):GetString()
	local port 		= world:GetComponent(e, "HostSettings", "Port"):GetInt()
	--local maxusers 	= world:GetComponent(e, "HostSettings", "MaxUsers"):GetInt()
	local fillai 	= world:GetComponent(e, "HostSettings", "FillAI"):GetInt()
	local allowSpec	= world:GetComponent(e, "HostSettings", "AllowSpectators"):GetInt()
	
	print("name: " .. name)
	print("password: " .. password)
	print("map: " .. map)
	print("gamemode: " .. gamemode)
	print("port: " .. port)
	print("fillai: " .. tostring(fillai))
	print("allowSpec: " .. tostring(allowSpec))
	
	self:RemoveMenu()
	local cmd = string.format("hostsettings %s %s %s %s %d %d %d", name, password, map, gamemode, port, fillai, allowSpec)
	Console.AddToCommandQueue(cmd)
	
end

HostMenuSystem.SpawnMenu = function(self)

	self:CreateElement("gamemenubackground", "quad", 0, 0, -2.5, 2.3, 2.8)
	--self:CreateElement("host", "quad", 0, 0.67, -2, 0.6, 0.3)
	
	--self:CreateCheckbox("unchecked", "checkbox", 0, 0.25, -2, 0.15, 0.15, false)
	
	-- BUTTONS
	
	local button = self:CreateButton("hostdedicated", "quad", -0.5, -0.84, -2, 0.50, 0.24)
	self:AddConsoleCommandToButton("hostlisten;gamemode", button)
	
	button = self:CreateButton("hostlisten", "quad", 0.5, -0.84, -2, 0.50, 0.24)
	self:AddEntityCommandToButton("ApplyHostSettings", button)
	
	
	-- SETTINGS TEXT
	
	local text = self:CreateText("left", "text", -0.87, 0.79, -1.99999, 2.5, 0.08)	
	self:AddTextToTexture("A"..1, "Server Name:", 0, 0, 0, 0, text)
	
	text = self:CreateText("left", "text", -0.87, 0.68, -1.99999, 2.5, 0.08)	
	self:AddTextToTexture("A"..2, "Server Password:", 0, 0, 0, 0, text)
	
	text = self:CreateText("left", "text", -0.87, 0.57, -1.99999, 2.5, 0.08)	
	self:AddTextToTexture("A"..3, "Server Game Mode:", 0, 0, 0, 0, text)
	
	text = self:CreateText("left", "text", -0.87, 0.46, -1.99999, 2.5, 0.08)	
	self:AddTextToTexture("A"..4, "Server Port:", 0, 0, 0, 0, text)
	
	text = self:CreateText("left", "text", -0.87, 0.35, -1.99999, 2.5, 0.08)	
	self:AddTextToTexture("A"..5, "Server Map:", 0, 0, 0, 0, text)
	
	text = self:CreateText("left", "text", -0.87, 0.24, -1.99999, 2.5, 0.08)	
	self:AddTextToTexture("A"..6, "Fill empty slots with AIs:", 0, 0, 0, 0, text)
	
	text = self:CreateText("left", "text", -0.87, 0.13, -1.99999, 2.5, 0.08)	
	self:AddTextToTexture("A"..7, "Allow Spectators: ", 0, 0, 0, 0, text)
	
	
	-- USER SETTINGS
	
	-- Name TEXT
	text = self:CreateText("left", "text", 0.16, 0.79, -1.99999, 2.5, 0.08)	
	self:AddTextToTexture("B"..1, "DefaultServerName", 0, 1, 1, 1, text)
	world:CreateComponentAndAddTo("StringSetting", text)
	world:GetComponent(text, "StringSetting", "SettingsName"):SetString("Name")
	world:GetComponent(text, "StringSetting", "Value"):SetString("DefaultServerName")
	
	-- Password TEXT
	text = self:CreateText("left", "text", 0.16, 0.68, -1.99999, 2.5, 0.08)	
	self:AddTextToTexture("B"..2, "EmptyPassword", 0, 1, 1, 1, text)
	world:CreateComponentAndAddTo("StringSetting", text)
	world:GetComponent(text, "StringSetting", "SettingsName"):SetString("Password")
	world:GetComponent(text, "StringSetting", "Value"):SetString("default")
	
	-- GameMode TEXT
	text = self:CreateText("left", "text", 0.16, 0.57, -1.99999, 2.5, 0.08)	
	self:AddTextToTexture("B"..3, "storaspelthreaded", 0, 1, 1, 1, text)
	world:CreateComponentAndAddTo("StringSetting", text)
	world:GetComponent(text, "StringSetting", "SettingsName"):SetString("GameMode")
	world:GetComponent(text, "StringSetting", "Value"):SetString("storaspelthreaded")
	
	-- Port INT
	text = self:CreateText("left", "text", 0.16, 0.46, -1.99999, 2.5, 0.08)	
	self:AddTextToTexture("B"..4, "6112", 0, 1, 1, 1, text)
	world:CreateComponentAndAddTo("IntSetting", text)
	world:GetComponent(text, "IntSetting", "SettingsName"):SetString("Port")
	world:GetComponent(text, "IntSetting", "Value"):SetInt(6112)
	
	-- GameMode TEXT
	text = self:CreateText("left", "text", 0.16, 0.35, -1.99999, 2.5, 0.08)	
	self:AddTextToTexture("B"..5, "map", 0, 1, 1, 1, text)
	world:CreateComponentAndAddTo("StringSetting", text)
	world:GetComponent(text, "StringSetting", "SettingsName"):SetString("Map")
	world:GetComponent(text, "StringSetting", "Value"):SetString("map")
	
	-- FillAI CHECKBOX
	local cb = self:CreateCheckbox("unchecked", "checkbox", 0.2, 0.200, -1.99999, 0.07, 0.07, false)
	world:CreateComponentAndAddTo("BoolSetting", cb)
	world:GetComponent(cb, "BoolSetting", "SettingsName"):SetString("FillAI")
	world:GetComponent(cb, "BoolSetting", "Value"):SetInt(0)
	
	-- AllowSpectators CHECKBOX
	local cb = self:CreateCheckbox("checked", "checkbox", 0.2, 0.08, -1.99999, 0.07, 0.07, true)
	world:CreateComponentAndAddTo("BoolSetting", cb)
	world:GetComponent(cb, "BoolSetting", "SettingsName"):SetString("AllowSpectators")
	world:GetComponent(cb, "BoolSetting", "Value"):SetInt(1)
	
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
	world:GetComponent(button, "TextTexture", "Name"):SetString(n) -- TODO: NAME CANT BE MORE THAN 3 CHARS? WTF?
	world:GetComponent(button, "TextTexture", "Text"):SetString(text)
	world:GetComponent(button, "TextTexture", "FontIndex"):SetInt(font)
	world:GetComponent(button, "TextTexture", "R"):SetFloat(r)
	world:GetComponent(button, "TextTexture", "G"):SetFloat(g)
	world:GetComponent(button, "TextTexture", "B"):SetFloat(b)
end

HostMenuSystem.AddConsoleCommandToButton = function(self, command, button)

	world:CreateComponentAndAddTo("MenuConsoleCommand", button)
	world:GetComponent(button, "MenuConsoleCommand", "Command"):SetString(command)
	
end

HostMenuSystem.AddEntityCommandToButton = function(self, command, button)
	world:CreateComponentAndAddTo("MenuEntityCommand", button)
	world:GetComponent(button, "MenuEntityCommand", "ComponentName"):SetString(command)
	
end

HostMenuSystem.AddHoverSize = function(self, deltascale, button)

	local sx, sy, sz = world:GetComponent(button, "Scale", 0):GetFloat3()
	world:CreateComponentAndAddTo("HoverSize", button)
	world:GetComponent(button, "HoverSize", 0):SetFloat3(sx*deltascale, sy*deltascale, sz*deltascale)
	
end