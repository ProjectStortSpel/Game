NotificationBoxSystem = System()
NotificationBoxSystem.Name = "NotificationBox"

NotificationBoxSystem.Update = function(self, dt, taskIndex, taskCount)
	if Input.GetTouchState(0) == InputState.Released then

		local pressedButtons = self:GetEntities("OnPickBoxHit")
		if #pressedButtons > 0 then
			self:RemoveMenu()
		else
			self:RemoveMenu()
		end
		
	end

end

NotificationBoxSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)
	for i = 1, #entities do
		local entityId = entities[i]
		if world:EntityHasComponent(entityId, self.Name) then
			self:SpawnMenu()
		end
	end
end

NotificationBoxSystem.SpawnMenu = function(self)
	local background = self:CreateElement("gamemenubackground", "quad", 0, -0, -3.0, 1.5, 1)
		
	local text = self:CreateText("center", "text", 0.0, 0.2, -1.99999, 2.5, 0.065)	
	self:AddTextToTexture("pop", "Graphics settings changed.", 0, 0, 0, 0, text)
		
	local button = nil
	button = self:CreateElement("ok", "quad", 0, -0.22, -3.0, 0.6, 0.3)
	--self:AddEntityCommandToButton("OptionMenu", button)
	self:AddHoverSize(1.1, button)	

end

NotificationBoxSystem.RemoveMenu = function(self)
	local entities = self:GetEntities()
	for i = 1, #entities do
		world:KillEntity(entities[i])
	end
end

NotificationBoxSystem.Initialize = function(self)
	self:SetName("NotificationBoxSystem")
	self:UsingUpdate()
	self:UsingEntitiesAdded()
	self:AddComponentTypeToFilter(self.Name, FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter(self.Name.."Element", FilterType.RequiresOneOf)
end

NotificationBoxSystem.CreateElement = function(self, object, folder, posx, posy, posz, scalex, scaley)
	local id = world:CreateNewEntity("Button")
	world:CreateComponentAndAddTo(self.Name.."Element", id)
	world:GetComponent(id, "Model", 0):SetModel(object, folder, 2)
	world:GetComponent(id, "Position", 0):SetFloat3(posx, posy, posz)
	world:GetComponent(id, "Scale", 0):SetFloat3(scalex, scaley, 1)
	world:GetComponent(id, "PickBox", 0):SetFloat2(1, 1)
	world:GetComponent(id, "Rotation", 0):SetFloat3(0, 0, 0)
	return id	
end

--NotificationBoxSystem.AddConsoleCommandToButton = function(self, command, button)
--	world:CreateComponentAndAddTo("MenuConsoleCommand", button)
--	world:GetComponent(button, "MenuConsoleCommand", "Command"):SetString(command)
--end

--NotificationBoxSystem.AddEntityCommandToButton = function(self, command, button)
--	world:CreateComponentAndAddTo("MenuEntityCommand", button)
--	world:GetComponent(button, "MenuEntityCommand", "ComponentName"):SetText(command)
--end

NotificationBoxSystem.AddHoverSize = function(self, deltascale, button)
	local scale = world:GetComponent(button, "Scale", 0)
	local sx, sy, sz = scale:GetFloat3()
	world:CreateComponentAndAddTo("HoverSize", button)
	local hoversize = world:GetComponent(button, "HoverSize", 0)
	hoversize:SetFloat3(sx*deltascale, sy*deltascale, sz*deltascale)
end

NotificationBoxSystem.CreateText = function(self, object, folder, posx, posy, posz, scalex, scaley)

	local id = world:CreateNewEntity("Text")
	world:CreateComponentAndAddTo(self.Name.."Element", id)
	world:GetComponent(id, "Model", 0):SetModel(object, folder, 2)
	world:GetComponent(id, "Position", 0):SetFloat3(posx, posy, posz)
	world:GetComponent(id, "Scale", 0):SetFloat3(scalex, scaley, 1)
	world:GetComponent(id, "Rotation", 0):SetFloat3(0, 0, 0)
	
	return id	
end

NotificationBoxSystem.AddTextToTexture = function(self, n, text, font, r, g, b, button)
	world:CreateComponentAndAddTo("TextTexture", button)
	world:GetComponent(button, "TextTexture", "Name"):SetText(n) -- TODO: NAME CANT BE MORE THAN 3 CHARS? WTF?
	world:GetComponent(button, "TextTexture", "Text"):SetText(text)
	world:GetComponent(button, "TextTexture", "FontIndex"):SetInt(font)
	world:GetComponent(button, "TextTexture", "R"):SetFloat(r)
	world:GetComponent(button, "TextTexture", "G"):SetFloat(g)
	world:GetComponent(button, "TextTexture", "B"):SetFloat(b)
end

NotificationBoxSystem.PostInitialize = function(self)
end