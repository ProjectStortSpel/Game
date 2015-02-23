LobbySystem = System()
LobbySystem.Name = "LobbyMenu"

LobbySystem.Initialize = function ( self )
	--	Set Name
	self:SetName(self.Name.."System")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter(self.Name, FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter(self.Name.."Element", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("GameRunning", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("UnitEntityId", FilterType.RequiresOneOf)
end

LobbySystem.PostInitialize = function ( self )
	self:SpawnMenu()
end

LobbySystem.EntitiesAdded = function(self, dt, entities)
	for n = 1, #entities do
		local entityId = entities[n]
		if world:EntityHasComponent( entityId, self.Name) then
			self:SpawnMenu()
		elseif world:EntityHasComponent( entityId, self.Name.."Element") then
		
		elseif world:EntityHasComponent( entityId, "UnitEntityId") then
			self:UpdatePlayers()	
		elseif world:EntityHasComponent( entityId, "GameRunning") then
			self:RemoveMenu()
		end
	end
end

LobbySystem.UpdatePlayers = function(self)
	
	local entities = self:GetEntities("LobbyMenuPlayer")
	for i = 1, #entities do
		world:KillEntity(entities[i])
	end
	
	local entities = self:GetEntities("UnitEntityId")
	for i = 1, #entities do
		local entityId = entities[i]
		
		
		local text = self:CreateElement("left", "text", -0.81, 0.64-i*0.11, -1.999, 1.5, 0.08)	
		self:AddTextToTexture("LobbyMenuSystemPlayer"..i, "test", 0, 1, 1, 1, text)
	end
	
end

LobbySystem.SpawnMenu = function(self)
	local background = self:CreateElement("gamemenubackground", "quad", 0, -0, -3.1, 1.5, 2)
		
	--local button = nil
	--button = self:CreateElement("options", "quad", 0, 0.6, -3, 0.6, 0.3)
	--self:AddEntityCommandToButton("OptionMenu", button)
	--self:AddHoverSize(1.1, button)	
	--
	--button = self:CreateElement("lobby", "quad", 0, 0.2, -3, 0.6, 0.3)
	--self:AddConsoleCommandToButton("disconnect;stop;gamemode lobby", button)
	--self:AddHoverSize(1.1, button)	
    --
	--button = self:CreateElement("quit", "quad", 0, -0.6, -3, 0.6, 0.3)
	--self:AddConsoleCommandToButton("quit", button)
	--self:AddHoverSize(1.1, button)	
end

LobbySystem.RemoveMenu = function(self)
	local entities = self:GetEntities()
	for i = 1, #entities do
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
	scale:SetFloat3(scalex, scaley, 1)
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

--Net.Receive("Client.ReadyCheck", 
--	function(id, ip, port)
--
--	end 
--)