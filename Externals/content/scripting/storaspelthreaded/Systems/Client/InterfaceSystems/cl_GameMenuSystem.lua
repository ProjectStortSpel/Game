GameMenuSystem = System()
GameMenuSystem.Name = "GameMenu"
GameMenuSystem.RequestRelease = false

GameMenuSystem.Initialize = function(self)
	--	Set Name
	self:SetName(self.Name.."System")
	
	--	Toggle EntitiesAdded
	self:UsingUpdate()
	self:UsingEntitiesAdded()
	
	--	Set Filter
	self:AddComponentTypeToFilter(self.Name, FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter(self.Name.."Element", FilterType.RequiresOneOf)
end

GameMenuSystem.Update = function(self, dt)
	if self.RequestRelease then
		local pressedButtons = self:GetEntities("OnPickBoxHit")
		if #pressedButtons > 0 then
			local pressedButton = pressedButtons[1]
			if world:EntityHasComponent(pressedButton, "MenuConsoleCommand") then
				local command = world:GetComponent(pressedButton, "MenuConsoleCommand", "Command"):GetText()
				self:RemoveMenu()
				Console.AddToCommandQueue(command)
			end
			if world:EntityHasComponent(pressedButton, "MenuEntityCommand") then
				local compname = world:GetComponent(pressedButton, "MenuEntityCommand", "ComponentName"):GetText()
				self:RemoveMenu()
				local id = world:CreateNewEntity()
				world:CreateComponentAndAddTo(compname, id)
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

GameMenuSystem.EntitiesAdded = function(self, dt, entities)
	for n = 1, #entities do
		local entityId = entities[n]
		if world:EntityHasComponent(entityId, self.Name) then
			self:SpawnMenu()
		end
	end
end

GameMenuSystem.SpawnMenu = function(self)
	local background = self:CreateElement("transparentbackground", "quad", 0, -0, -2.22, 7, 7)
	--background = self:CreateElement("gamemenubackground", "quad", 0, -0, -3.1, 1.8, 2.3)
		
	local button = nil	
	button = self:CreateButton("options", "quad", 0, 0.5, -2.2, 0.6, 0.3)
	self:AddEntityCommandToButton("OptionMenu", button)
	self:AddHoverSize(1.1, button)	
	
	button = self:CreateButton("lobby", "quad", 0, 0.2, -2.2, 0.6, 0.3)
	self:AddConsoleCommandToButton("disconnect;stop;gamemode lobby", button)
	self:AddHoverSize(1.1, button)	

	button = self:CreateButton("quit", "quad", 0, -0.1, -2.2, 0.6, 0.3)
	self:AddConsoleCommandToButton("quit", button)
	self:AddHoverSize(1.1, button)
	
	button = self:CreateButton("resume", "quad", 0, -0.7, -2.2, 0.6, 0.3)
	self:AddConsoleCommandToButton("resume", button)
	self:AddHoverSize(1.1, button)	
end

GameMenuSystem.RemoveMenu = function(self)
	local entities = self:GetEntities()
	for i = 1, #entities do
		world:KillEntity(entities[i])
	end
end

GameMenuSystem.CreateButton = function(self, object, folder, posx, posy, posz, scalex, scaley)
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

GameMenuSystem.CreateElement = function(self, object, folder, posx, posy, posz, scalex, scaley)
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

GameMenuSystem.AddConsoleCommandToButton = function(self, command, button)
	world:CreateComponentAndAddTo("MenuConsoleCommand", button)
	world:GetComponent(button, "MenuConsoleCommand", "Command"):SetText(command)
end

GameMenuSystem.AddEntityCommandToButton = function(self, command, button)
	world:CreateComponentAndAddTo("MenuEntityCommand", button)
	world:GetComponent(button, "MenuEntityCommand", "ComponentName"):SetText(command)
end

GameMenuSystem.AddHoverSize = function(self, deltascale, button)
	local scale = world:GetComponent(button, "Scale", 0)
	local sx, sy, sz = scale:GetFloat3()
	world:CreateComponentAndAddTo("HoverSize", button)
	local hoversize = world:GetComponent(button, "HoverSize", 0)
	hoversize:SetFloat3(sx*deltascale, sy*deltascale, sz*deltascale)
end