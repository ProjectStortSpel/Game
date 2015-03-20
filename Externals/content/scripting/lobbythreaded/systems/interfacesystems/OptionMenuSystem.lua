OptionMenuSystem = System()
OptionMenuSystem.Name = "OptionMenu"
OptionMenuSystem.RequestRelease = false
OptionMenuSystem.Active = false

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
				local command = world:GetComponent(pressedButton, "MenuConsoleCommand", "Command"):GetString()
				Console.AddToCommandQueue(command)
			end
			if world:EntityHasComponent(pressedButton, "MenuEntityCommand") then
				local compname = world:GetComponent(pressedButton, "MenuEntityCommand", "ComponentName"):GetText()
				local id = world:CreateNewEntity()
				world:CreateComponentAndAddTo(compname, id)
			end
			if self.Active then
				self:RemoveMenuToMain()
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
	--local background = self:CreateElement("gamemenubackground", "quad", 0, -0, -3.1, 1.5, 2.0)
	
	local button = nil
	button = self:CreateElement("graphicslow", "quad", 0, 0.4, -3, 0.6, 0.3)
	self:AddConsoleCommandToButton("changegraphics low", button)
	--self:AddEntityCommandToButton("NotificationBox", button)
	self:AddHoverSize(1.1, button)
	
	button = self:CreateElement("graphicshigh", "quad", 0, -0.4, -3, 0.6, 0.3)
	self:AddConsoleCommandToButton("changegraphics high", button)	
	--self:AddEntityCommandToButton("NotificationBox", button)	
	self:AddHoverSize(1.1, button)
	
	button = self:CreateElement("back", "quad", 2, -1, -3, 0.6, 0.3)
	self:AddHoverSize(1.1, button)
	
	self.Active = true
end

OptionMenuSystem.RemoveMenu = function(self)
	local entities = self:GetEntities()
	for i = 1, #entities do
		world:KillEntity(entities[i])
	end
	
	self.Active = false
end

OptionMenuSystem.RemoveMenuToMain = function(self)
	local entities = self:GetEntities()
	for i = 1, #entities do
		world:KillEntity(entities[i])
	end
	
	local tomainmenu = world:CreateNewEntity()
	world:CreateComponentAndAddTo("MainMenu", tomainmenu)
	
	self.Active = false
end

OptionMenuSystem.CreateElement = function(self, object, folder, posx, posy, posz, scalex, scaley)
	local id = world:CreateNewEntity("Button")
	world:CreateComponentAndAddTo(self.Name.."Element", id)
	world:GetComponent(id, "Model", 0):SetModel(object, folder, 2)
	world:GetComponent(id, "Position", 0):SetFloat3(posx, posy, posz)
	world:GetComponent(id, "Scale", 0):SetFloat3(scalex, scaley, 1)
	world:GetComponent(id, "PickBox", 0):SetFloat2(1, 1)
	world:GetComponent(id, "Rotation", 0):SetFloat3(0, 0, 0)
	return id	
end

OptionMenuSystem.AddConsoleCommandToButton = function(self, command, button)
	world:CreateComponentAndAddTo("MenuConsoleCommand", button)
	world:GetComponent(button, "MenuConsoleCommand", "Command"):SetString(command)
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