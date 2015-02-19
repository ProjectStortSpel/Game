NotificationBoxSystem = System()
NotificationBoxSystem.Name = "NotificationBox"
NotificationBoxSystem.IsMenuActive = false
NotificationBoxSystem.Text = ""
NotificationBoxSystem.Parentmenu = ""

NotificationBoxSystem.Initialize = function(self)
	self:SetName("NotificationBoxSystem")
	self:UsingUpdate()
	self:UsingEntitiesAdded()
	self:AddComponentTypeToFilter(self.Name, FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter(self.Name.."Element", FilterType.RequiresOneOf)
end


NotificationBoxSystem.Update = function(self, dt, taskIndex, taskCount)

	if not self.IsMenuActive then
		return
	end

	if Input.GetTouchState(0) == InputState.Released then
		local pressedButtons = self:GetEntities("OnPickBoxHit")
		if #pressedButtons > 0 then
		
			local pressedButton = pressedButtons[1]
		
			if world:EntityHasComponent(pressedButton, "MenuEntityCommand") then
				self:MenuEntityCommandPressed(pressedButton)
				self:RemoveMenu()
			end
	
		end
		
	end

end

NotificationBoxSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)
	for i = 1, #entities do
		local entityId = entities[i]
		if world:EntityHasComponent(entityId, self.Name) then
		
			self.Text = world:GetComponent(entityId, self.Name, "Text"):GetText()
			self.Parentmenu = world:GetComponent(entityId, self.Name, "ParentMenu"):GetText()

			self:SpawnMenu()
		end
	end
end

NotificationBoxSystem.MenuEntityCommandPressed = function(self, entity)

	local cmp = world:GetComponent(entity, "MenuEntityCommand", "ComponentName"):GetText()
	--self:RemoveMenu()
	local id = world:CreateNewEntity()
	world:CreateComponentAndAddTo(cmp, id)
	
end

NotificationBoxSystem.SpawnMenu = function(self)

	local background = self:CreateElement("gamemenubackground", "quad", 0, -0, -1.5, 1.0, 0.5)
		
	local text = self:CreateText("center", "text", 0.0, 0.13, -1.0, 0.5, 0.065)	
	self:AddTextToTexture("U1", self.Text, 0, 0, 0, 0, text)
		
	local button = self:CreateElement("ok", "quad", 0, -0.10, -1.0, 0.20, 0.085)
	self:AddEntityCommandToButton(self.Parentmenu .. "Activate", button)
	self:AddHoverSize(1.1, button)	

	self.IsMenuActive = true
end

NotificationBoxSystem.RemoveMenu = function(self)
	local entities = self:GetEntities()
	for i = 1, #entities do
		world:KillEntity(entities[i])
	end
	
	self.IsMenuActive = false
end



NotificationBoxSystem.CreateButton = function(self, object, folder, posX, posY, posZ, scaleX, scaleY)

	local id = world:CreateNewEntity("Button")
	world:CreateComponentAndAddTo(self.Name.."Element", id)
	world:GetComponent(id, "Model", 0):SetModel(object, folder, 2)
	world:GetComponent(id, "Position", 0):SetFloat3(posX, posY, posZ)
	world:GetComponent(id, "Scale", 0):SetFloat3(scaleX, scaleY, 1)
	world:GetComponent(id, "PickBox", 0):SetFloat2(1, 1)
	world:GetComponent(id, "Rotation", 0):SetFloat3(0, 0, 0)
	self:AddHoverSize(1.1, id)
	return id
	
end

NotificationBoxSystem.CreateElement = function(self, object, folder, posX, posY, posZ, scaleX, scaleY)

	local id = world:CreateNewEntity("Button")
	world:CreateComponentAndAddTo(self.Name.."Element", id)
	world:GetComponent(id, "Model", 0):SetModel(object, folder, 2)
	world:GetComponent(id, "Position", 0):SetFloat3(posX, posY, posZ)
	world:GetComponent(id, "Scale", 0):SetFloat3(scaleX, scaleY, 1)
	world:GetComponent(id, "PickBox", 0):SetFloat2(1, 1)
	world:GetComponent(id, "Rotation", 0):SetFloat3(0, 0, 0)
	return id	

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

NotificationBoxSystem.AddHoverSize = function(self, deltascale, button)

	local sx, sy, sz = world:GetComponent(button, "Scale", 0):GetFloat3()
	world:CreateComponentAndAddTo("HoverSize", button)
	world:GetComponent(button, "HoverSize", 0):SetFloat3(sx*deltascale, sy*deltascale, sz*deltascale)

end

NotificationBoxSystem.AddEntityCommandToButton = function(self, command, button)

	world:CreateComponentAndAddTo("MenuEntityCommand", button)
	world:GetComponent(button, "MenuEntityCommand", "ComponentName"):SetText(command)

end