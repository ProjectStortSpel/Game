LobbySystem = System()
LobbySystem.Name = "LobbyMenu"
LobbySystem.UpdateRequest = false
LobbySystem.UpdateMe = false

LobbySystem.Initialize = function ( self )
	--	Set Name
	self:SetName(self.Name.."System")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	self:UsingUpdate()

	--	Set Filter
	self:AddComponentTypeToFilter(self.Name, FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter(self.Name.."Element", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("LobbyPlayerReadyMSG", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("GameRunning", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("UnitEntityId", FilterType.RequiresOneOf)
end

LobbySystem.PostInitialize = function ( self )
	self:SpawnMenu()
end

LobbySystem.Update = function(self, dt)
	if self.UpdateMe == true then
		print("update")
		self:UpdatePlayers()
		self.UpdateMe = false
	end
	if self.UpdateRequest == true then
		print("update request")
		self.UpdateMe = true
		self.UpdateRequest = false
	end
end

LobbySystem.EntitiesAdded = function(self, dt, entities)
	for n = 1, #entities do
		local entityId = entities[n]
		if world:EntityHasComponent( entityId, self.Name) then
			self:SpawnMenu()
		elseif world:EntityHasComponent( entityId, self.Name.."Element") then
		
		elseif world:EntityHasComponent( entityId, "UnitEntityId") then
			self.UpdateMe = true
		elseif world:EntityHasComponent( entityId, "GameRunning") then
			self:RemoveMenu()
		elseif world:EntityHasComponent( entityId, "LobbyPlayerReadyMSG") then
			self:ToggleReady(entityId)
		end
	end
end

LobbySystem.UpdatePlayers = function(self)
	
	local entities = self:GetEntities("LobbyMenuPlayer")
	for i = 1, #entities do
		world:KillEntity(entities[i])
	end
	
	local readyplayers = 0
	
	local entities = self:GetEntities("UnitEntityId")
	for i = 1, #entities do
		local entityId = entities[i]
		local unitId = world:GetComponent(entityId, "UnitEntityId", "Id"):GetInt()
		
		local ip = "AI";
		if world:EntityHasComponent(entityId, "NetConnection") then
			ip = world:GetComponent(entityId, "NetConnection", "IpAddress"):GetText()
			
			if world:EntityHasComponent(entityId, "LobbyPlayerReady") then
				ip = ip.." - Ready"
				readyplayers = readyplayers + 1
			end
		else
			ip = ip.." - Ready"
			readyplayers = readyplayers + 1
		end
		
		local r, g, b = world:GetComponent(unitId, "Color", "X"):GetFloat3(0)
		
		local text = self:CreateElement("left", "text", -0.4, 0.64-i*0.11, -1.999, 1.5, 0.08)
		world:CreateComponentAndAddTo("LobbyMenuPlayer", text)
		self:AddTextToTexture("LMSP"..i, ip, 0, r, g, b, text)
	end
	
	if #entities == readyplayers then
		Console.AddToCommandQueue("start")
	end
	
end

LobbySystem.ToggleReady = function(self, player)

	local thisip = world:GetComponent(player, "NetConnection", "IpAddress"):GetText()
	local thisport = world:GetComponent(player, "NetConnection", "Port"):GetInt()

	local entities = self:GetEntities("UnitEntityId")
	for i = 1, #entities do
		local entityId = entities[i]
		local ip = world:GetComponent(entityId, "NetConnection", "IpAddress"):GetText()
		local port = world:GetComponent(entityId, "NetConnection", "Port"):GetInt()
		if ip == thisip and port == thisport then
		
			if world:EntityHasComponent(entityId, "LobbyPlayerReady") then
				world:RemoveComponentFrom("LobbyPlayerReady", entityId)
			else
				world:CreateComponentAndAddTo("LobbyPlayerReady", entityId)
			end
		
			break
		end
	end	
	print("update me")
	self.UpdateRequest = true
	world:KillEntity(player)
end


LobbySystem.SpawnMenu = function(self)
	local background = self:CreateElement("gamemenubackground", "quad", 0, -0, -3.1, 1.5, 2)
end

LobbySystem.RemoveMenu = function(self)
	local entities = self:GetEntities()
	for i = 1, #entities do
		local entityId = entities[i]
		if world:EntityHasComponent( entityId, self.Name.."Element" ) then
			world:KillEntity(entities[i])
		end
	end
end

LobbySystem.AddConsoleCommandToButton = function(self, command, button)
	world:CreateComponentAndAddTo("MenuConsoleCommand", button)
	world:GetComponent(button, "MenuConsoleCommand", "Command"):SetText(command)
end

LobbySystem.AddEntityCommandToButton = function(self, command, button)
	world:CreateComponentAndAddTo("MenuEntityCommand", button)
	world:GetComponent(button, "MenuEntityCommand", "ComponentName"):SetText(command)
end

LobbySystem.CreateElement = function(self, object, folder, posx, posy, posz, scalex, scaley)
	local id = world:CreateNewEntity()
	world:CreateComponentAndAddTo("SyncNetwork", id)
	world:CreateComponentAndAddTo("Model", id)
	world:CreateComponentAndAddTo("Position", id)
	world:CreateComponentAndAddTo("Rotation", id)
	world:CreateComponentAndAddTo("Scale", id)
	world:CreateComponentAndAddTo("PickBox", id)
	world:CreateComponentAndAddTo(self.Name.."Element", id)
	local model = world:GetComponent(id, "Model", 0)
	model:SetModel(object, folder, 2)
	local position = world:GetComponent(id, "Position", 0)
	position:SetFloat3(posx, posy, posz)
	local scale = world:GetComponent(id, "Scale", 0)
	scale:SetFloat3(scalex, scaley, 1, false)
	local rotation = world:GetComponent(id, "Rotation", 0)
	rotation:SetFloat3(0, 0, 0)
	return id	
end

LobbySystem.AddTextToTexture = function(self, n, text, font, r, g, b, button)
	world:CreateComponentAndAddTo("TextTexture", button)
	world:GetComponent(button, "TextTexture", "Name"):SetText(n) -- TODO: NAME CANT BE MORE THAN 3 CHARS? WTF?
	world:GetComponent(button, "TextTexture", "Text"):SetText(text)
	world:GetComponent(button, "TextTexture", "FontIndex"):SetInt(font)
	world:GetComponent(button, "TextTexture", "R"):SetFloat(r)
	world:GetComponent(button, "TextTexture", "G"):SetFloat(g)
	world:GetComponent(button, "TextTexture", "B"):SetFloat(b)
end

Net.Receive("Server.ReadyCheck", 
	function(id, ip, port)
	
		local id = world:CreateNewEntity()
		world:CreateComponentAndAddTo("LobbyPlayerReadyMSG", id)
		world:CreateComponentAndAddTo("NetConnection", id)
		world:GetComponent(id, "NetConnection", "IpAddress"):SetText(ip)
		world:GetComponent(id, "NetConnection", "Port"):SetInt(port)
		
	end 
)