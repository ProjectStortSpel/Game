OptionMenuSystem = System()
OptionMenuSystem.Name = "OptionMenu"
OptionMenuSystem.RequestRelease = false
OptionMenuSystem.TestSliderButton = nil


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
	if self.RequestRelease then
		local pressedButtons = self:GetEntities("OnPickBoxHit")
		if #pressedButtons > 0 then
			local pressedButton = pressedButtons[1]
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
			if world:EntityHasComponent(pressedButton, "MenuSlider") then
				local command = world:GetComponent(pressedButton, "MenuSlider", "ConsoleCommand"):GetText()
				local minvalue = world:GetComponent(pressedButton, "MenuSlider", "Min"):GetFloat()
				local maxvalue = world:GetComponent(pressedButton, "MenuSlider", "Max"):GetFloat()
				local hitpos = world:GetComponent(pressedButton, "OnPickBoxHit", "X"):GetFloat()
				world:GetComponent(self.TestSliderButton, "Position", "X"):SetFloat(math.floor(hitpos*10)*0.1)
				local value = minvalue + maxvalue * (hitpos + 0.5)
				
				Console.AddToCommandQueue(command.." "..math.floor(value*10)*0.1)
			end
		else
			self:RemoveMenu()
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
	
	text = self:CreateText("left", "text", -1.3, 0.7, -2.2, 0.6, 0.1)	
	self:AddTextToTexture("graphicsettings", "Graphic Settings", 0, 1, 1, 1, text)
		
	button = self:CreateElement("graphicslow", "quad", -1, 0.3, -2.2, 0.6, 0.3)
	self:AddConsoleCommandToButton("changegraphics low", button)	
	self:AddEntityCommandToButton("GameMenu", button)
	self:AddHoverSize(1.1, button)
	
	button = self:CreateElement("graphicshigh", "quad", -1, 0.0, -2.2, 0.6, 0.3)
	self:AddConsoleCommandToButton("changegraphics high", button)	
	self:AddEntityCommandToButton("GameMenu", button)
	self:AddHoverSize(1.1, button)
	
	text = self:CreateText("left", "text", 0.7, 0.7, -2.2, 0.6, 0.1)	
	self:AddTextToTexture("soundsettings", "Sound Settings", 0, 1, 1, 1, text)
	
	text = self:CreateText("left", "text", 0.8, 0.4, -2.2, 0.6, 0.1)	
	self:AddTextToTexture("volume", "Volume:", 0, 1, 1, 1, text)
	
	button = self:CreateElement("slider", "quad", 1.2, 0.2, -2.2, 0.8, 0.2)
	self:AddSliderToButton("volume", 0.0, 1.0, button)	
	local currentvolume = Audio.GetVolume()
	self.TestSliderButton = self:CreateText("sliderbutton", "quad", currentvolume*0.5-0.5, 0, 0.01, 0.125, 1)
	world:CreateComponentAndAddTo("Parent", self.TestSliderButton)
	world:GetComponent(self.TestSliderButton, "Parent", 0):SetInt(button)
	
	button = self:CreateElement("back", "quad", 0, -0.6, -2.2, 0.6, 0.3)
	self:AddEntityCommandToButton("GameMenu", button)
	self:AddHoverSize(1.1, button)
end

OptionMenuSystem.RemoveMenu = function(self)
	local entities = self:GetEntities()
	for i = 1, #entities do
		world:KillEntity(entities[i])
	end
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

OptionMenuSystem.AddSliderToButton = function(self, command, minvalue, maxvalue, button)
	world:CreateComponentAndAddTo("MenuSlider", button)
	world:GetComponent(button, "MenuSlider", "ConsoleCommand"):SetText(command)
	world:GetComponent(button, "MenuSlider", "Min"):SetFloat(minvalue)
	world:GetComponent(button, "MenuSlider", "Max"):SetFloat(maxvalue)
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