HowToSystem = System()
HowToSystem.Entities = {}
HowToSystem.Entities.__mode = "k"
HowToSystem.Active = false
HowToSystem.RequestRelease = false

HowToSystem.Update = function(self, dt)
	if self.RequestRelease then

		local pressedButtons = self:GetEntities("OnPickBoxHit")
		if #pressedButtons > 0 then
			local pressedButton = pressedButtons[1]
			if world:EntityHasComponent(pressedButton, "MenuConsoleCommand") then
				local command = world:GetComponent(pressedButton, "MenuConsoleCommand", "Command"):GetString()
				Console.AddToCommandQueue(command)
			end
			if world:EntityHasComponent(pressedButton, "MenuEntityCommand") then
				local compname = world:GetComponent(pressedButton, "MenuEntityCommand", "ComponentName"):GetText()
				local id = world:CreateNewEntity()
				world:CreateComponentAndAddTo(compname, id)
			end
			if self.Active then
				self:RemoveMenu()
			end
		end
		
	end
	
	if Input.GetTouchState(0) == InputState.Released then
		self.RequestRelease = true
	else
		self.RequestRelease = false
	end
end

HowToSystem.RemoveMenu = function(self)
	local entities = self:GetEntities()
	local entityCount = #entities
	for i = 1, entityCount do
		world:KillEntity(entities[i])
	end
	
	entityCount = #self.Entities
	for i = 1, entityCount do
		world:KillEntity(self.Entities[i])
	end
	self.Entities = {}
	self.Entities.__mode = "k"
	
	local tomainmenu = world:CreateNewEntity()
	world:CreateComponentAndAddTo("MainMenu", tomainmenu)
	
	self.Active = false
end

HowToSystem.Initialize = function(self)
	self:SetName("HowToSystem")
	self:UsingUpdate()
	self:UsingEntitiesAdded()
	self:AddComponentTypeToFilter("HowToMenu", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("HowToMenuElement", FilterType.RequiresOneOf)
end

HowToSystem.EntitiesAdded = function(self, dt, entities)
	for n = 1, #entities do
		local entityId = entities[n]
		if world:EntityHasComponent(entityId, "HowToMenu") then
			self:CreateEntites()
		end
	end
end

HowToSystem.CreateElement = function(self, object, folder, posx, posy, posz, scalex, scaley)
	local id = world:CreateNewEntity("Button")
	world:CreateComponentAndAddTo("HowToMenuElement", id)
	world:GetComponent(id, "Model", 0):SetModel(object, folder, 2)
	world:GetComponent(id, "Position", 0):SetFloat3(posx, posy, posz)
	world:GetComponent(id, "Scale", 0):SetFloat3(scalex, scaley, 1)
	world:GetComponent(id, "PickBox", 0):SetFloat2(1, 1)
	world:GetComponent(id, "Rotation", 0):SetFloat3(0, 0, 0)
	return id	
end

HowToSystem.AddConsoleCommandToButton = function(self, command, button)
	world:CreateComponentAndAddTo("MenuConsoleCommand", button)
	world:GetComponent(button, "MenuConsoleCommand", "Command"):SetString(command)
end

HowToSystem.AddEntityCommandToButton = function(self, command, button)
	world:CreateComponentAndAddTo("MenuEntityCommand", button)
	world:GetComponent(button, "MenuEntityCommand", "ComponentName"):SetText(command)
end

HowToSystem.AddHoverSize = function(self, deltascale, button)
	local scale = world:GetComponent(button, "Scale", 0)
	local sx, sy, sz = scale:GetFloat3()
	world:CreateComponentAndAddTo("HoverSize", button)
	local hoversize = world:GetComponent(button, "HoverSize", 0)
	hoversize:SetFloat3(sx*deltascale, sy*deltascale, sz*deltascale)
end

HowToSystem.CreateEntites = function(self)

	-- GAME MENU BUTTON
	--local menubutton = self:CreateElement("back", "quad", 4, -1.6, -5, 1, 0.5)
	--self:AddEntityCommandToButton("GameMenu", menubutton)
	--self:AddHoverSize(1.5, menubutton)
	--table.insert(self.Entities, menubutton)
	
	local specials = self:CreateElement("rules", "quad", -1.0, -0.5, -3.0, 1.4, 1.4)
	table.insert(self.Entities, specials)
	
	local cards = self:CreateElement("cards", "quad", -1.1, 0.5, -3.0, 1.1, 1.1)
	table.insert(self.Entities, cards)
	
	local tutorial = self:CreateElement("tutorial", "quad", 0.8, 1.0, -3.0, 1.1, 1.1)
	table.insert(self.Entities, tutorial)
	
	local button = self:CreateElement("back", "quad", 2, -1, -3, 0.6, 0.3)
	self:AddHoverSize(1.1, button)
	
	self.Active = true
end

