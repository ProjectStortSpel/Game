MainMenuSystem = System()
MainMenuSystem.Name = "MainMenu"
MainMenuSystem.RequestRelease = false

MainMenuSystem.Initialize = function(self)
	--	Set Name
	self:SetName(self.Name.."System")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	self:UsingUpdate()
	
	--	Set Filter
	self:AddComponentTypeToFilter(self.Name, FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter(self.Name.."Element", FilterType.RequiresOneOf)
end
MainMenuSystem.PostInitialize = function(self)

	self:SpawnMenu()

end
MainMenuSystem.Update = function(self, dt)
	if self.RequestRelease then
		local pressedButtons = self:GetEntities("OnPickBoxHit")
		if #pressedButtons > 0 then
			local pressedButton = pressedButtons[1]
			if world:EntityHasComponent(pressedButton, "MenuConsoleCommand") then
				local command = world:GetComponent(pressedButton, "MenuConsoleCommand", "Command"):GetString()
				self:RemoveMenu()
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

MainMenuSystem.EntitiesAdded = function(self, dt, entities)
	for n = 1, #entities do
		local entityId = entities[n]
		if world:EntityHasComponent(entityId, self.Name) then
			self:SpawnMenu()
		end
	end
end
MainMenuSystem.CreateButtons = function(self)
	local menubutton = self:CreateElement("play", "quad", -2, 0.6, -5, 1, 0.5)
	--self:AddConsoleCommandToButton("host;gamemode storaspel", menubutton)	
	self:AddEntityCommandToButton("ConnectMenu", menubutton)
	self:AddHoverSize(1.5, menubutton)

	local menubutton = self:CreateElement("howto", "quad", -2, -0.0, -5, 1, 0.5)
	self:AddEntityCommandToButton("HowToMenu", menubutton)
	self:AddHoverSize(1.5, menubutton)

	local menubutton = self:CreateElement("options", "quad", -2, -0.6, -5, 1, 0.5)
	self:AddEntityCommandToButton("OptionMenu", menubutton)
	self:AddHoverSize(1.5, menubutton)

	local menubutton = self:CreateElement("quit", "quad", -2, -1.2, -5, 1, 0.5)
	self:AddConsoleCommandToButton("quit", menubutton)
	self:AddHoverSize(1.5, menubutton)
	
end

MainMenuSystem.SpawnMenu = function(self)
	self:CreateButtons()
end

MainMenuSystem.RemoveMenu = function(self)
	local entities = self:GetEntities()
	for i = 1, #entities do
		world:KillEntity(entities[i])
	end
end

MainMenuSystem.CreateElement = function(self, object, folder, posx, posy, posz, scalex, scaley)
	local id = world:CreateNewEntity("Button")
	world:CreateComponentAndAddTo(self.Name.."Element", id)
	world:GetComponent(id, "Model", 0):SetModel(object, folder, 2)
	world:GetComponent(id, "Position", 0):SetFloat3(posx, posy, posz)
	world:GetComponent(id, "Scale", 0):SetFloat3(scalex, scaley, 1)
	world:GetComponent(id, "PickBox", 0):SetFloat2(1, 1)
	world:GetComponent(id, "Rotation", 0):SetFloat3(0, 0, 0)
	return id	
end

MainMenuSystem.AddConsoleCommandToButton = function(self, command, button)
	world:CreateComponentAndAddTo("MenuConsoleCommand", button)
	world:GetComponent(button, "MenuConsoleCommand", "Command"):SetString(command)
end

MainMenuSystem.AddEntityCommandToButton = function(self, command, button)
	world:CreateComponentAndAddTo("MenuEntityCommand", button)
	world:GetComponent(button, "MenuEntityCommand", "ComponentName"):SetText(command)
end

MainMenuSystem.AddHoverSize = function(self, deltascale, button)
	local scale = world:GetComponent(button, "Scale", 0)
	local sx, sy, sz = scale:GetFloat3()
	world:CreateComponentAndAddTo("HoverSize", button)
	local hoversize = world:GetComponent(button, "HoverSize", 0)
	hoversize:SetFloat3(sx*deltascale, sy*deltascale, sz*deltascale)
end