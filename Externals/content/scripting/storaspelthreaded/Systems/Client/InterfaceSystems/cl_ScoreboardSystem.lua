ScoreboardSystem = System()
ScoreboardSystem.Name = "Scoreboard"
ScoreboardSystem.GameStarted = false
ScoreboardSystem.RemoveMenuRequest = false
ScoreboardSystem.Button = nil
ScoreboardSystem.Showing = false

ScoreboardSystem.Initialize = function ( self )
	--	Set Name
	self:SetName("ScoreboardSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	self:UsingUpdate()

	--	Set Filter
	self:AddComponentTypeToFilter(self.Name, FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter(self.Name.."Element", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("ScoreboardPlayer", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("GameRunning", FilterType.RequiresOneOf)
	
end

ScoreboardSystem.PostInitialize = function(self)

	self.Button = self:CreateGameInterfaceElement("scoreboardbutton", "quad", -3.9, -0.9, -4, 1.0, 0.5)
	self:AddHoverSize(1.1, self.Button)
	
end

ScoreboardSystem.EntitiesAdded = function(self, dt, entities)

	for n = 1, #entities do
	
		if world:EntityHasComponent(entities[n], "GameRunning") then	
			self.GameStarted = true
		end
	
	end

end

ScoreboardSystem.Update = function(self, dt)

	if not self.GameStarted then
		return
	end
	
	if self.RemoveMenuRequest then
		self:RemoveMenu()
		self.RemoveMenuRequest = false
	else
		if world:EntityHasComponent(self.Button, "OnPickBoxHit") then
			if not self.Showing then
				self:SpawnMenu()
			end
		elseif Input.GetKeyState(Key.Tab) == InputState.Pressed then
			self:SpawnMenu()
		elseif Input.GetKeyState(Key.Tab) ~= InputState.Down and self.Showing then
			self.RemoveMenuRequest = true
		end
	end
end

ScoreboardSystem.SpawnMenu = function(self)
	
	self.Showing = true
	--local background = self:CreateElement("gamemenubackground", "quad", 0, -0, -3.5, 3, 3)

	local players = self:GetEntities("ScoreboardPlayer")
	local name = ""
	local r, g, b = 0
	for i = 1, #players do
		name = world:GetComponent(players[i], "ScoreboardPlayer", "Name"):GetText()			
		r = world:GetComponent(players[i], "ScoreboardPlayer", "R"):GetFloat()
		g = world:GetComponent(players[i], "ScoreboardPlayer", "G"):GetFloat()
		b = world:GetComponent(players[i], "ScoreboardPlayer", "B"):GetFloat()

		local button = self:CreateElement("shade", "quad", -0.0, 1.40-i*0.22, -4.0, 4.0, 0.2)
		
		local text = self:CreateElement("left", "text", -1.85, 1.48-i*0.22, -3.999, 3.0, 0.16)
		self:AddTextToTexture("SCBRD"..i, name, 0, r, g, b, text)
		
	end
	
	
end

ScoreboardSystem.RemoveMenu = function(self)
	self.Showing = false
	local entities = self:GetEntities(self.Name.."Element")
	for i = 1, #entities do
		world:KillEntity(entities[i])
	end
end

ScoreboardSystem.CreateElement = function(self, object, folder, posx, posy, posz, scalex, scaley)
	local id = world:CreateNewEntity()
	world:CreateComponentAndAddTo("SyncNetwork", id)
	world:CreateComponentAndAddTo("Model", id)
	world:CreateComponentAndAddTo("Position", id)
	world:CreateComponentAndAddTo("Rotation", id)
	world:CreateComponentAndAddTo("Scale", id)
	world:CreateComponentAndAddTo("PickBox", id)
	world:CreateComponentAndAddTo(self.Name.."Element", id)
	
	world:GetComponent(id, "Model", 0):SetModel(object, folder, 3)
	world:GetComponent(id, "Position", 0):SetFloat3(posx, posy, posz)
	world:GetComponent(id, "Scale", 0):SetFloat3(scalex, scaley, 1, false)
	world:GetComponent(id, "Rotation", 0):SetFloat3(0, 0, 0)

	return id	
end


ScoreboardSystem.CreateGameInterfaceElement = function(self, object, folder, posx, posy, posz, scalex, scaley)
	local id = world:CreateNewEntity()
	world:CreateComponentAndAddTo("Model", id)
	world:CreateComponentAndAddTo("Position", id)
	world:CreateComponentAndAddTo("Rotation", id)
	world:CreateComponentAndAddTo("Scale", id)
	world:CreateComponentAndAddTo("PickBox", id)
	world:CreateComponentAndAddTo("GameInterfaceElement", id)
	local model = world:GetComponent(id, "Model", 0)
	model:SetModel(object, folder, 3)
	local position = world:GetComponent(id, "Position", 0)
	position:SetFloat3(posx, posy, posz)
	local scale = world:GetComponent(id, "Scale", 0)
	scale:SetFloat3(scalex, scaley, 1)
	local pickbox = world:GetComponent(id, "PickBox", 0)
	pickbox:SetFloat2(1, 1)
	local rotation = world:GetComponent(id, "Rotation", 0)
	rotation:SetFloat3(0, 0, 0)
	return id	
end


ScoreboardSystem.AddTextToTexture = function(self, n, text, font, r, g, b, button)
	world:CreateComponentAndAddTo("TextTexture", button)
	world:GetComponent(button, "TextTexture", "Name"):SetText(n) -- TODO: NAME CANT BE MORE THAN 3 CHARS? WTF?
	world:GetComponent(button, "TextTexture", "Text"):SetText(text)
	world:GetComponent(button, "TextTexture", "FontIndex"):SetInt(font)
	world:GetComponent(button, "TextTexture", "R"):SetFloat(1)
	world:GetComponent(button, "TextTexture", "G"):SetFloat(1)
	world:GetComponent(button, "TextTexture", "B"):SetFloat(1)
	world:CreateComponentAndAddTo("Color", button)
	world:GetComponent(button, "Color", "X"):SetFloat(r)
	world:GetComponent(button, "Color", "Y"):SetFloat(g)
	world:GetComponent(button, "Color", "Z"):SetFloat(b)
end

ScoreboardSystem.AddHoverSize = function(self, deltascale, button)
	local scale = world:GetComponent(button, "Scale", 0)
	local sx, sy, sz = scale:GetFloat3()
	world:CreateComponentAndAddTo("HoverSize", button)
	local hoversize = world:GetComponent(button, "HoverSize", 0)
	hoversize:SetFloat3(sx*deltascale, sy*deltascale, sz*deltascale)
end