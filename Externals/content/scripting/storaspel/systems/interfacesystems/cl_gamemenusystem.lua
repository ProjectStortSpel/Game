GameMenuSystem = System()

GameMenuSystem.Update = function(self, dt)
	if Input.GetTouchState(0) == InputState.Released then

		local pressedButtons = self:GetEntities("OnPickBoxHit")
		if #pressedButtons > 0 then
			local pressedButton = pressedButtons[1]
			if world:EntityHasComponent(pressedButton, "MenuConsoleCommand") then
				local command = self:GetComponent(pressedButton, "MenuConsoleCommand", "Command"):GetString()
				self:RemoveMenu()
				Console.AddToCommandQueue(command)
			end
			if world:EntityHasComponent(pressedButton, "MenuEntityCommand") then
				local compname = self:GetComponent(pressedButton, "MenuEntityCommand", "ComponentName"):GetString()
				self:RemoveMenu()
				local id = world:CreateNewEntity()
				world:CreateComponentAndAddTo(compname, id)
			end
		else
			self:RemoveMenu()
		end
		
	end
	

end

GameMenuSystem.OnEntityAdded = function(self, entityId)
	if world:EntityHasComponent(entityId, "GameMenu") then
		self:SpawnMenu()
	end
end

GameMenuSystem.SpawnMenu = function(self)
	print("create game menu")
	local background = self:CreateElement("gamemenubackground", "quad", 0, 0, -4, 2, 2)
		
	local startbutton = self:CreateElement("rconreload", "quad", 0, 0.4, -3, 0.6, 0.3)
	self:AddConsoleCommandToButton("rcon reload", startbutton)	
		
	local startbutton = self:CreateElement("rconstart", "quad", 0, 0, -3, 0.6, 0.3)
	self:AddConsoleCommandToButton("rcon start", startbutton)

	local quitbutton = self:CreateElement("quit", "quad", 0, -0.4, -3, 0.6, 0.3)
	self:AddConsoleCommandToButton("quit", quitbutton)
end

GameMenuSystem.RemoveMenu = function(self)
	print("removed game menu")
	local entities = self:GetEntities()
	for i = 1, #entities do
		world:KillEntity(entities[i])
	end
end

GameMenuSystem.Initialize = function(self)
	self:SetName("GameMenuSystem")
	self:AddComponentTypeToFilter("GameMenu", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("GameMenuElement", FilterType.RequiresOneOf)
	print("GameMenuSystem initialized!")
end

GameMenuSystem.CreateElement = function(self, object, folder, posx, posy, posz, scalex, scaley)
	local id = world:CreateNewEntity()
	world:CreateComponentAndAddTo("Model", id)
	world:CreateComponentAndAddTo("Position", id)
	world:CreateComponentAndAddTo("Rotation", id)
	world:CreateComponentAndAddTo("Scale", id)
	world:CreateComponentAndAddTo("PickBox", id)
	world:CreateComponentAndAddTo("GameMenuElement", id)
	local model = self:GetComponent(id, "Model", 0)
	model:SetModel(object, folder, 2)
	local position = self:GetComponent(id, "Position", 0)
	position:SetFloat3(posx, posy, posz)
	local scale = self:GetComponent(id, "Scale", 0)
	scale:SetFloat3(scalex, scaley, 1)
	local pickbox = self:GetComponent(id, "PickBox", 0)
	pickbox:SetFloat2(1, 1)
	local rotation = self:GetComponent(id, "Rotation", 0)
	rotation:SetFloat3(0, 0, 0)
	return id	
end

GameMenuSystem.AddConsoleCommandToButton = function(self, command, button)
	world:CreateComponentAndAddTo("MenuConsoleCommand", button)
	world:GetComponent(button, "MenuConsoleCommand", "Command"):SetString(command)
end

GameMenuSystem.AddEntityCommandToButton = function(self, command, button)
	world:CreateComponentAndAddTo("MenuEntityCommand", button)
	world:GetComponent(button, "MenuEntityCommand", "ComponentName"):SetString(command)
end

GameMenuSystem.PostInitialize = function(self)

end