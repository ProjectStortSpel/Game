OptionMenuSystem = System()
OptionMenuSystem.Name = "OptionMenu"
OptionMenuSystem.RequestRelease = false

OptionMenuSystem.Initialize = function(self)
	--	Set Name
	self:SetName(self.Name.."System")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	self:UsingUpdate()
	
	--	Set Filter
	self:AddComponentTypeToFilter(self.Name, FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter(self.Name.."Element", FilterType.RequiresOneOf)
end

OptionMenuSystem.Update = function(self, dt)

	local pressedButtons = self:GetEntities("OnPickBoxHit")
	if #pressedButtons > 0 then
		local pressedButton = pressedButtons[1]
		if self.RequestRelease then
			if world:EntityHasComponent(pressedButton, "MenuConsoleCommand") then
				local command = world:GetComponent(pressedButton, "MenuConsoleCommand", "Command"):GetText()
				Console.AddToCommandQueue(command)
			end
			if world:EntityHasComponent(pressedButton, "MenuEntityCommand") then
				local compname = world:GetComponent(pressedButton, "MenuEntityCommand", "ComponentName"):GetText()
				self:RemoveMenu()
				local id = world:CreateNewEntity()
				world:CreateComponentAndAddTo(compname, id)
			end
		end
	end
	
	if Input.GetTouchState(0) == InputState.Released then
		self.RequestRelease = true
	else
		self.RequestRelease = false
	end
end

OptionMenuSystem.EntitiesAdded = function(self, dt, entities)
	for n = 1, #entities do
		local entityId = entities[n]
		if world:EntityHasComponent(entityId, self.Name) then
			self:SpawnMenu()
		end
	end
end

OptionMenuSystem.SpawnMenu = function(self)
	local background = self:CreateElement("transparentbackground", "quad", 0, -0, -2.22, 7, 7)
	
	local button = nil
	local text = nil
	
	-- GRAPHIC SETTINGS
	text = self:CreateText("center", "text", -0.7, 0.8, -2.2, 0.6, 0.1)	
	self:AddTextToTexture("graphicsettings", "Graphic Settings", 0, 1, 1, 1, text)
		
	button = self:CreateButton("graphicslow", "quad", -0.7, 0.4, -2.2, 0.6, 0.3)
	self:AddConsoleCommandToButton("changegraphics low", button)	
	self:AddEntityCommandToButton("GameMenu", button)
	self:AddHoverSize(1.1, button)
	
	button = self:CreateButton("graphicshigh", "quad", -0.7, 0.1, -2.2, 0.6, 0.3)
	self:AddConsoleCommandToButton("changegraphics high", button)	
	self:AddEntityCommandToButton("GameMenu", button)
	self:AddHoverSize(1.1, button)
	
	
	-- SOUND SETTINGS
	text = self:CreateText("center", "text", 0.7, 0.8, -2.2, 0.6, 0.1)	
	self:AddTextToTexture("soundsettings", "Sound Settings", 0, 1, 1, 1, text)
	
	text = self:CreateText("left", "text", 0.4, 0.58, -2.2, 0.6, 0.08)	
	self:AddTextToTexture("mastervolume", "Master volume:", 0, 1, 1, 1, text)
	text = self:CreateText("left", "text", 0.4, 0.28, -2.2, 0.6, 0.08)	
	self:AddTextToTexture("sfxvolume", "SFX:", 0, 1, 1, 1, text)
	text = self:CreateText("left", "text", 0.4, -0.02, -2.2, 0.6, 0.08)	
	self:AddTextToTexture("musicvolume", "Music:", 0, 1, 1, 1, text)
	
	button = self:CreateElement("slider", "quad", 0.8, 0.4, -2.2, 0.8, 0.2)
	local currentvolume = Audio.GetMasterVolume()
	self:AddSliderToButton("volume", 0.0, 1.0, currentvolume, 0.05, button)	
	
	button = self:CreateElement("slider", "quad", 0.8, 0.1, -2.2, 0.8, 0.2)
	local currentvolume = Audio.GetVolume()
	self:AddSliderToButton("soundvolume", 0.0, 1.0, currentvolume, 0.05, button)	

	button = self:CreateElement("slider", "quad", 0.8, -0.2, -2.2, 0.8, 0.2)
	local currentvolume = Audio.GetMusicVolume()
	self:AddSliderToButton("musicvolume", 0.0, 1.0, currentvolume, 0.05, button)
	
	-- BACK
	button = self:CreateButton("back", "quad", 0, -0.7, -2.2, 0.6, 0.3)
	self:AddEntityCommandToButton("GameMenu", button)
	self:AddHoverSize(1.1, button)
end

OptionMenuSystem.RemoveMenu = function(self)
	local entities = self:GetEntities()
	for i = 1, #entities do
		world:KillEntity(entities[i])
	end
end
OptionMenuSystem.CreateButton = function(self, object, folder, posx, posy, posz, scalex, scaley)
	local id = world:CreateNewEntity("Button")
	world:CreateComponentAndAddTo(self.Name.."Element", id)
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

OptionMenuSystem.CreateElement = function(self, object, folder, posx, posy, posz, scalex, scaley)
	local id = world:CreateNewEntity()
	world:CreateComponentAndAddTo("Model", id)
	world:CreateComponentAndAddTo("Position", id)
	world:CreateComponentAndAddTo("Rotation", id)
	world:CreateComponentAndAddTo("Scale", id)
	world:CreateComponentAndAddTo("PickBox", id)
	world:CreateComponentAndAddTo(self.Name.."Element", id)
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

OptionMenuSystem.CreateText = function(self, object, folder, posx, posy, posz, scalex, scaley)
	local id = world:CreateNewEntity("Text")
	world:CreateComponentAndAddTo(self.Name.."Element", id)
	world:GetComponent(id, "Model", 0):SetModel(object, folder, 3)
	world:GetComponent(id, "Position", 0):SetFloat3(posx, posy, posz)
	world:GetComponent(id, "Scale", 0):SetFloat3(scalex, scaley, 1)
	world:GetComponent(id, "Rotation", 0):SetFloat3(0, 0, 0)
	return id		
end

OptionMenuSystem.AddTextToTexture = function(self, n, text, font, r, g, b, button)
	world:CreateComponentAndAddTo("TextTexture", button)
	world:GetComponent(button, "TextTexture", "Name"):SetText(n) -- TODO: NAME CANT BE MORE THAN 3 CHARS? WTF?
	world:GetComponent(button, "TextTexture", "Text"):SetText(text)
	world:GetComponent(button, "TextTexture", "FontIndex"):SetInt(font)
	world:GetComponent(button, "TextTexture", "R"):SetFloat(r)
	world:GetComponent(button, "TextTexture", "G"):SetFloat(g)
	world:GetComponent(button, "TextTexture", "B"):SetFloat(b)
end

OptionMenuSystem.AddSliderToButton = function(self, command, minvalue, maxvalue, current, step, button)
	world:CreateComponentAndAddTo("MenuSlider", button)
	world:GetComponent(button, "MenuSlider", "ConsoleCommand"):SetText(command)
	world:GetComponent(button, "MenuSlider", "Min"):SetFloat(minvalue)
	world:GetComponent(button, "MenuSlider", "Max"):SetFloat(maxvalue)
	world:GetComponent(button, "MenuSlider", "Current"):SetFloat(current)
	world:GetComponent(button, "MenuSlider", "Step"):SetFloat(step)
end

OptionMenuSystem.AddConsoleCommandToButton = function(self, command, button)
	world:CreateComponentAndAddTo("MenuConsoleCommand", button)
	world:GetComponent(button, "MenuConsoleCommand", "Command"):SetText(command)
end

OptionMenuSystem.AddEntityCommandToButton = function(self, command, button)
	world:CreateComponentAndAddTo("MenuEntityCommand", button)
	world:GetComponent(button, "MenuEntityCommand", "ComponentName"):SetText(command)
end

OptionMenuSystem.AddHoverSize = function(self, deltascale, button)
	local scale = world:GetComponent(button, "Scale", 0)
	local sx, sy, sz = scale:GetFloat3()
	world:CreateComponentAndAddTo("HoverSize", button)
	local hoversize = world:GetComponent(button, "HoverSize", 0)
	hoversize:SetFloat3(sx*deltascale, sy*deltascale, sz*deltascale)
end