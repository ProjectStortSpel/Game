ConnectMenuSystem = System()
ConnectMenuSystem.Name = "ConnectMenu"
ConnectMenuSystem.ServerStartIndex = 1
ConnectMenuSystem.ServerEndIndex = 1
ConnectMenuSystem.doRefresh = true

ConnectMenuSystem.Update = function(self, dt, taskIndex, taskCount)
	if Input.GetTouchState(0) == InputState.Released then

		local pressedButtons = self:GetEntities("OnPickBoxHit")
		if #pressedButtons > 0 then
			local pressedButton = pressedButtons[1]
			if world:EntityHasComponent(pressedButton, "MenuConsoleCommand") then
				local command = self:GetComponent(pressedButton, "MenuConsoleCommand", "Command"):GetString()
				self:RemoveMenu()
				Console.AddToCommandQueue(command)
			end
			if world:EntityHasComponent(pressedButton, "MenuEntityCommand") then
				local compname = self:GetComponent(pressedButton, "MenuEntityCommand", "ComponentName"):GetString()
				self:RemoveMenu()
				local id = world:CreateNewEntity()
				world:CreateComponentAndAddTo(compname, id)
			end
		else
			self:RemoveMenu()
		end
		
	end
	
	local menu = self:GetEntities("ConnectMenu")
	if #menu > 0 and self.doRefresh == true then
		self.doRefresh = false
		self:RefreshMenu()
	end
end

ConnectMenuSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)
	for i = 1, #entities do
		local entityId = entities[i]
		if world:EntityHasComponent(entityId, self.Name) then
			self:SpawnMenu()
		end
		if world:EntityHasComponent(entityId, "ServerListEntry") then
			self.doRefresh = true
		end
	end
end

ConnectMenuSystem.SpawnMenu = function(self)
	local background = self:CreateElement("gamemenubackground", "quad", 0, 0, -2.1, 2.07, 1.3)
	local servers = self:GetEntities("ServerListEntry")
	local button = nil

    local server = nil
    local servername = nil
    local serverip = nil
    local serverport = nil
    local servernousers = nil
    local servermaxusers = nil
	local servergamestarted = nil
    local text = nil

	for i = 1, #servers do
		server = servers[i]
		servername = self:GetComponent(server, "ServerListEntry", "Name"):GetString(0)
		serverip = self:GetComponent(server, "ServerListEntry", "IpAddress"):GetString(0)
        serverport = self:GetComponent(server, "ServerListEntry", "Port"):GetInt(0)
		servernousers = self:GetComponent(server, "ServerListEntry", "NoUsers"):GetInt(0)
		servermaxusers = self:GetComponent(server, "ServerListEntry", "MaxUsers"):GetInt(0)
		servergamestarted = self:GetComponent(server, "ServerListEntry", "GameStarted"):GetBool(0)
		button = self:CreateElement("shade", "quad", 0, 0.6-i*0.11, -2, 1.8, 0.1)
		self:AddConsoleCommandToButton("connect "..serverip .. " " .. serverport, button)
		self:AddHoverSize(1.005, button)
		
		text = self:CreateText("center", "text", -0.85, 0.64-i*0.11, -1.99999, 0.08, 0.08)	
		if servergamestarted == true then
			self:AddTextToTexture("C0"..i, "X", 0, 1, 0, 0, text)
		else
			self:AddTextToTexture("C0"..i, "O", 0, 0, 1, 0, text)
		end
		text = self:CreateText("left", "text", -0.81, 0.64-i*0.11, -1.99999, 1.5, 0.08)	
		self:AddTextToTexture("C1"..i, servername, 0, 1, 1, 1, text)
		text = self:CreateText("center", "text", 0, 0.64-i*0.11, -1.99999, 1.78, 0.08)
self:AddTextToTexture("C2"..i, serverip .. ":" .. serverport, 0, 1, 1, 1, text)
		text = self:CreateText("right", "text", 0.89, 0.64-i*0.11, -1.99999, 0.2, 0.08)	
		self:AddTextToTexture("C3"..i, "["..servernousers.."/"..servermaxusers.."]", 0, 1, 1, 1, text)
	end
end

ConnectMenuSystem.RefreshMenu = function(self)
	local entities = self:GetEntities()
	for i = 1, #entities do
		if world:EntityHasComponent(entities[i], self.Name.."Element") then
			world:KillEntity(entities[i])
		end
	end
	self:SpawnMenu()
end

ConnectMenuSystem.RemoveMenu = function(self)
	local entities = self:GetEntities()
	for i = 1, #entities do
		if not world:EntityHasComponent(entities[i], "ServerListEntry") then
			world:KillEntity(entities[i])
		end
	end
end

ConnectMenuSystem.Initialize = function(self)
	self:SetName(self.Name.."System")
	self:UsingUpdate()
	self:UsingEntitiesAdded()
	self:AddComponentTypeToFilter(self.Name, FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter(self.Name.."Element", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("ServerListEntry", FilterType.RequiresOneOf)
end

ConnectMenuSystem.CreateText = function(self, object, folder, posx, posy, posz, scalex, scaley)
	local id = world:CreateNewEntity()
	world:CreateComponentAndAddTo("Model", id)
	world:CreateComponentAndAddTo("Position", id)
	world:CreateComponentAndAddTo("Rotation", id)
	world:CreateComponentAndAddTo("Scale", id)
	world:CreateComponentAndAddTo(self.Name.."Element", id)
	local model = self:GetComponent(id, "Model", 0)
	model:SetModel(object, folder, 2)
	local position = self:GetComponent(id, "Position", 0)
	position:SetFloat3(posx, posy, posz)
	local scale = self:GetComponent(id, "Scale", 0)
	scale:SetFloat3(scalex, scaley, 1)
	local rotation = self:GetComponent(id, "Rotation", 0)
	rotation:SetFloat3(0, 0, 0)
	return id	
end

ConnectMenuSystem.CreateElement = function(self, object, folder, posx, posy, posz, scalex, scaley)
	local id = world:CreateNewEntity()
	world:CreateComponentAndAddTo("Model", id)
	world:CreateComponentAndAddTo("Position", id)
	world:CreateComponentAndAddTo("Rotation", id)
	world:CreateComponentAndAddTo("Scale", id)
	world:CreateComponentAndAddTo("PickBox", id)
	world:CreateComponentAndAddTo(self.Name.."Element", id)
	local model = self:GetComponent(id, "Model", 0)
	model:SetModel(object, folder, 2)
	local position = self:GetComponent(id, "Position", 0)
	position:SetFloat3(posx, posy, posz)
	local scale = self:GetComponent(id, "Scale", 0)
	scale:SetFloat3(scalex, scaley, 1)
	local pickbox = self:GetComponent(id, "PickBox", 0)
	pickbox:SetFloat2(1, 1)
	local rotation = self:GetComponent(id, "Rotation", 0)
	rotation:SetFloat3(0, 0, 0)
	return id	
end

ConnectMenuSystem.AddTextToTexture = function(self, n, text, font, r, g, b, button)
	world:CreateComponentAndAddTo("TextTexture", button)
	world:GetComponent(button, "TextTexture", "Name"):SetString(n) -- TODO: NAME CANT BE MORE THAN 3 CHARS? WTF?
	world:GetComponent(button, "TextTexture", "Text"):SetString(text)
	world:GetComponent(button, "TextTexture", "FontIndex"):SetInt(font)
	world:GetComponent(button, "TextTexture", "R"):SetFloat(r)
	world:GetComponent(button, "TextTexture", "G"):SetFloat(g)
	world:GetComponent(button, "TextTexture", "B"):SetFloat(b)
end

ConnectMenuSystem.AddConsoleCommandToButton = function(self, command, button)
	world:CreateComponentAndAddTo("MenuConsoleCommand", button)
	world:GetComponent(button, "MenuConsoleCommand", "Command"):SetString(command)
end

ConnectMenuSystem.AddEntityCommandToButton = function(self, command, button)
	world:CreateComponentAndAddTo("MenuEntityCommand", button)
	world:GetComponent(button, "MenuEntityCommand", "ComponentName"):SetString(command)
end

ConnectMenuSystem.AddHoverSize = function(self, deltascale, button)
	local scale = self:GetComponent(button, "Scale", 0)
	local sx, sy, sz = scale:GetFloat3()
	world:CreateComponentAndAddTo("HoverSize", button)
	local hoversize = self:GetComponent(button, "HoverSize", 0)
	hoversize:SetFloat3(sx*deltascale, sy*deltascale, sz*deltascale)
end

ConnectMenuSystem.PostInitialize = function(self)

end