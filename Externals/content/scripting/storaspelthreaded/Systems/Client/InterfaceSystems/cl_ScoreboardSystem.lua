ScoreboardSystem = System()
ScoreboardSystem.Name = "Scoreboard"

ScoreboardSystem.Initialize = function ( self )
	--	Set Name
	self:SetName("ScoreboardSystem")
	
	--	Toggle EntitiesAdded
	--self:UsingEntitiesAdded()
	self:UsingUpdate()

	--	Set Filter
	self:AddComponentTypeToFilter(self.Name, FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter(self.Name.."Element", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("ScoreboardPlayer", FilterType.RequiresOneOf)
	
end

ScoreboardSystem.Update = function(self, dt)
	if Input.GetKeyState(Key.Tab) == InputState.Pressed then
		self:SpawnMenu()
		print("HEEJ")
	elseif Input.GetKeyState(Key.Tab) == InputState.Released then
		print("DÅ")
		self:RemoveMenu()
	end

end

ScoreboardSystem.SpawnMenu = function(self)

	local background = self:CreateElement("gamemenubackground", "quad", 0, -0, -4.1, 3, 3)

	local players = self:GetEntities("ScoreboardPlayer")
	local name = ""
	for i = 1, #players do
		print(i)
		name = world:GetComponent(players[i], "ScoreboardPlayer", "Name"):GetText()			
		print(name)
		local text = self:CreateElement("left", "text", -0.50, 1.5-i*0.22, -3.0, 3.0, 0.16)
		self:AddTextToTexture("SCBRD"..i, name, 0, 0, 1, 1, text)
		
	end
	
	
end

ScoreboardSystem.RemoveMenu = function(self)

	local entities = self:GetEntities(self.Name.."Element")
	for i = 1, #entities do
		world:KillEntity(entities[i])
	end
	
end

ScoreboardSystem.AddConsoleCommandToButton = function(self, command, button)
	world:CreateComponentAndAddTo("MenuConsoleCommand", button)
	world:GetComponent(button, "MenuConsoleCommand", "Command"):SetText(command)
end

ScoreboardSystem.AddEntityCommandToButton = function(self, command, button)
	world:CreateComponentAndAddTo("MenuEntityCommand", button)
	world:GetComponent(button, "MenuEntityCommand", "ComponentName"):SetText(command)
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