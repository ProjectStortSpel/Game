ConnectMenuSystem = System()
ConnectMenuSystem.Name = "ConnectMenu"
ConnectMenuSystem.ServerStartIndex = 1
ConnectMenuSystem.ServerEndIndex = 1

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
	
end

ConnectMenuSystem.OnEntityAdded = function(self, entityId)
	if world:EntityHasComponent(entityId, self.Name) then
		self:SpawnMenu()
	end
end

ConnectMenuSystem.SpawnMenu = function(self)
	local background = self:CreateElement("gamemenubackground", "quad", 0, 0, -2.1, 2.07, 1.3)
	
	local servers = self:GetEntities("ServerListEntry")
	
	if #servers <= 10 then
		self.ServerStartIndex = 1
		self.ServerEndIndex = #servers
	end
	
	local button = nil
	for i = self.ServerStartIndex, self.ServerEndIndex do
		local server = servers[i]
		local ip = self:GetComponent(server, "ServerListEntry", "IpAddress"):GetString(0)
		print(ip)
	
		button = self:CreateElement("shade", "quad", 0, 0.6-i*0.11, -2, 1.8, 0.1)
		self:AddConsoleCommandToButton("connect "..ip, button)
	
	end
	
	--local button = nil
	----connect localhost
	--button = self:CreateElement("localhost", "quad", 0, 0.3, -2, 0.4, 0.2)
	--self:AddConsoleCommandToButton("connect", button)	
	--self:AddHoverSize(1.5, button)
	--
	----connect server
	--button = self:CreateElement("server", "quad", 0.6, 0.3, -2, 0.4, 0.2)
	--self:AddConsoleCommandToButton("connect 194.47.150.44", button)	
	--self:AddHoverSize(1.5, button)
	--
	----connect erik
	--button = self:CreateElement("erik", "quad", -0.6, 0, -2, 0.4, 0.2)
	--self:AddConsoleCommandToButton("connect 194.47.150.5", button)	
	--self:AddHoverSize(1.5, button)
	--
	----connect niklas
	--button = self:CreateElement("niklas", "quad", 0, 0, -2, 0.4, 0.2)
	--self:AddConsoleCommandToButton("connect 194.47.150.29", button)	
	--self:AddHoverSize(1.5, button)
	--
	----connect marcus
	--button = self:CreateElement("marcus", "quad", 0.6, 0, -2, 0.4, 0.2)
	--self:AddConsoleCommandToButton("connect 194.47.150.48", button)	
	--self:AddHoverSize(1.5, button)
	--
	----connect christian
	--button = self:CreateElement("christian", "quad", -0.6, -0.3, -2, 0.4, 0.2)
	--self:AddConsoleCommandToButton("connect 194.47.150.57", button)	
	--self:AddHoverSize(1.5, button)
	--
	----connect pontus
	--button = self:CreateElement("pontus", "quad", 0, -0.3, -2, 0.4, 0.2)
	--self:AddConsoleCommandToButton("connect 194.47.150.128", button)	
	--self:AddHoverSize(1.5, button)
	--
	----connect anders
	--button = self:CreateElement("anders", "quad", 0.6, -0.3, -2, 0.4, 0.2)
	--self:AddConsoleCommandToButton("connect 194.47.150.100", button)	
	--self:AddHoverSize(1.5, button)
	
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