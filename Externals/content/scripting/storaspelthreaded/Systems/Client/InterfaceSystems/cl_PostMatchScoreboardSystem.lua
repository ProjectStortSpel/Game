PostMatchScoreboardSystem = System()

PostMatchScoreboardSystem.Initialize = function(self)
	--	Set Name
	self:SetName("PostMatchScoreboardSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	
	--	Set Filter
	self:AddComponentTypeToFilter("DisplayPostMatch", FilterType.RequiresOneOf)
	
	self:AddComponentTypeToFilter("ScoreboardPlayer", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("PlayedCard", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("CheckpointReached", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("UnitDied", FilterType.RequiresOneOf)
	
	
	
end
 
PostMatchScoreboardSystem.EntitiesAdded = function(self, dt, addedEntities)
	
	for n = 1, #addedEntities do
		if world:EntityHasComponent(addedEntities[n], "DisplayPostMatch") then
			self:SpawnMenu()
		end
	end
	
end

PostMatchScoreboardSystem.GetPlayerData = function(self, playerNumber)
	
	
	
	
end

PostMatchScoreboardSystem.SpawnMenu = function(self)

	local button = self:CreateElement("shade", "quad", -0.0, 1.40, -4.0, 4.0, 0.2)
	
	local text = self:CreateElement("left", "text", -1.85, 1.48, -3.9, 3.0, 0.16)
	
	local	allPlayers	=	self:GetEntities("ScoreboardPlayer")
	for i = 1, #allPlayers do
		local	name = world:GetComponent(allPlayers[i], "ScoreboardPlayer", "Name"):GetText()			
		local	r = world:GetComponent(allPlayers[i], "ScoreboardPlayer", "R"):GetFloat()
		local	g = world:GetComponent(allPlayers[i], "ScoreboardPlayer", "G"):GetFloat()
		local	b = world:GetComponent(allPlayers[i], "ScoreboardPlayer", "B"):GetFloat()
		
		
		
		self:AddTextToTexture("SCBRD"..i, name, 0, r, g, b, text)
	end
	
end

PostMatchScoreboardSystem.CreateElement = function(self, object, folder, posx, posy, posz, scalex, scaley)
	local id = world:CreateNewEntity()
	world:CreateComponentAndAddTo("SyncNetwork", id)
	world:CreateComponentAndAddTo("Model", id)
	world:CreateComponentAndAddTo("Position", id)
	world:CreateComponentAndAddTo("Rotation", id)
	world:CreateComponentAndAddTo("Scale", id)
	world:CreateComponentAndAddTo("PickBox", id)
	world:CreateComponentAndAddTo("ScoreboardElement", id)
	
	world:GetComponent(id, "Model", 0):SetModel(object, folder, 3)
	world:GetComponent(id, "Position", 0):SetFloat3(posx, posy, posz)
	world:GetComponent(id, "Scale", 0):SetFloat3(scalex, scaley, 1, false)
	world:GetComponent(id, "Rotation", 0):SetFloat3(0, 0, 0)

	return id	
end

PostMatchScoreboardSystem.AddTextToTexture = function(self, n, text, font, r, g, b, button)
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



