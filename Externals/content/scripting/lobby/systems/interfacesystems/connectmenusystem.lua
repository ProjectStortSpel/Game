ConnectMenuSystem = System()
ConnectMenuSystem.Name = "ConnectMenu"
ConnectMenuSystem.ServerStartIndex = 1
ConnectMenuSystem.ServerEndIndex = 1
ConnectMenuSystem.doRefresh = true

ConnectMenuSystem.Update = function(self, dt)
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

ConnectMenuSystem.OnEntityAdded = function(self, entityId)
	if world:EntityHasComponent(entityId, self.Name) then
		self:SpawnMenu()
	end
	if world:EntityHasComponent(entityId, "ServerListEntry") then
		self.doRefresh = true
	end
end

ConnectMenuSystem.SpawnMenu = function(self)
print("CRASH")
	local background = self:CreateElement("gamemenubackground", "quad", 0, 0, -2.1, 2.07, 1.3)
	local servers = self:GetEntities("ServerListEntry")
	local button = nil

    local server = nil
    local servername = nil
    local serverip = nil
    local servernousers = nil
    local servermaxusers = nil
    local text = nil


	for i = 1, #servers do
        server = servers[i]
        servername = self:GetComponent(server, "ServerListEntry", "Name"):GetString(0)
        serverip = self:GetComponent(server, "ServerListEntry", "IpAddress"):GetString(0)
        servernousers = self:GetComponent(server, "ServerListEntry", "NoUsers"):GetInt(0)
        servermaxusers = self:GetComponent(server, "ServerListEntry", "MaxUsers"):GetInt(0)
		button = self:CreateElement("shade", "quad", 0, 0.6-i*0.11, -2, 1.8, 0.1)
		self:AddConsoleCommandToButton("connect "..serverip, button)
		self:AddHoverSize(1.005, button)
        text = self:CreateText("left", "text", -0.86, 0.64-i*0.11, -1.99999, 1, 0.08)	
		self:AddTextToTexture(servername, "C1"..i, text)
		self:AddConsoleCommandToButton("connect "..serverip, text)
		text = self:CreateText("center", "text", 0, 0.64-i*0.11, -1.99999, 1, 0.08)
		self:AddTextToTexture(serverip, "C2"..i, text)
		self:AddConsoleCommandToButton("connect "..serverip, text)
		text = self:CreateText("right", "text", 0.86, 0.64-i*0.11, -1.99999, 1, 0.08)	
		self:AddTextToTexture("["..servernousers.."/"..servermaxusers.."]", "C3"..i, text)
		self:AddConsoleCommandToButton("connect "..serverip, text)
        server = nil
        servername = nil
        serverip = nil
        servernousers = nil
        servermaxusers = nil
        text = nil
	end
background = nil
servers = nil
button = nil
print("NOCRASH");
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

ConnectMenuSystem.AddTextToTexture = function(self, text, n, button)
	world:CreateComponentAndAddTo("TextTexture", button)
	world:GetComponent(button, "TextTexture", "Name"):SetString(n) -- TODO: NAME CANT BE MORE THAN 3 CHARS? WTF?
	world:GetComponent(button, "TextTexture", "Text"):SetString(text)
	world:GetComponent(button, "TextTexture", "FontIndex"):SetInt(0)
	world:GetComponent(button, "TextTexture", "R"):SetFloat(1)
	world:GetComponent(button, "TextTexture", "G"):SetFloat(1)
	world:GetComponent(button, "TextTexture", "B"):SetFloat(1)
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