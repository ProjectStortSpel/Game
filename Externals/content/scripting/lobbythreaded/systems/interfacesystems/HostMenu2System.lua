HostMenu2System = System()
HostMenu2System.Name = "HostMenu2"
HostMenu2System.TextId = 0
HostMenu2System.OldText = ""

HostMenu2System.Initialize = function(self)
	self:SetName(self.Name.."System")
	self:UsingUpdate()
	self:UsingEntitiesAdded()
	self:AddComponentTypeToFilter(self.Name, FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter(self.Name.."Element", FilterType.RequiresOneOf)
end

HostMenu2System.Update = function(self, dt, taskIndex, taskCount)

	if Input.GetTouchState(0) == InputState.Released then
		local pressedButtons = self:GetEntities("OnPickBoxHit")
		if #pressedButtons > 0 then
			local pressedButton = pressedButtons[1]
			
			if world:EntityHasComponent(pressedButton, "Checkbox") then
				self:CheckboxPressed(pressedButton)
			end
			
		end
	end

end

HostMenu2System.CheckboxPressed = function(self, entity)

	local posX, posY, posZ = world:GetComponent(entity, "Position", 0):GetFloat3()
	local hoverX, hoverY, _ = world:GetComponent(entity, "HoverSize", 0):GetFloat3()
	
	if world:GetComponent(entity, "Checkbox", "Checked"):GetBool() then
		self:CreateCheckbox("unchecked", "checkbox", posX, posY, posZ, hoverX,hoverY, false)
	else
		self:CreateCheckbox("checked", "checkbox", posX, posY, posZ, hoverX,hoverY, true)
	end

	world:KillEntity(entity)
	
end

HostMenu2System.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

	for i = 1, #entities do
		local entityId = entities[i]
		if world:EntityHasComponent(entityId, self.Name) then
			self:SpawnMenu()
		end
	end

end

HostMenu2System.SpawnMenu = function(self)

	self:CreateElement("gamemenubackground", "quad", 0, 0, -2.5, 2.07, 1.3)
	--self:CreateElement("host", "quad", 0, 0.67, -2, 0.6, 0.3)
	
	--self:CreateCheckbox("unchecked", "checkbox", 0, 0.25, -2, 0.15, 0.15, false)
	
	local button = self:CreateButton("hostdedicated", "quad", -0.5, -0.34, -2, 0.50, 0.24)
	self:AddConsoleCommandToButton("hostdedicated;gamemode", button)
	
	button = self:CreateButton("hostlisten", "quad", 0.5, -0.34, -2, 0.50, 0.24)
	self:AddConsoleCommandToButton("hostlisten;gamemode", button)
	
	local text = self:CreateText("left", "text", -0.8, 0.45, -1.99999, 2.5, 0.08)	
	self:AddTextToTexture("A"..1, "Server Name: ", 0, 1, 1, 1, text)
	
	self.TextId = self:CreateText("left", "text", -0.8, 0.3, -1.99999, 2.5, 0.08)	
	self:AddTextToTexture("A"..2, "Server Password: ", 0, 1, 1, 1, self.TextId)
	
	text = self:CreateText("left", "text", -0.8, 0.15, -1.99999, 2.5, 0.08)	
	self:AddTextToTexture("A"..3, "Server Game Mode: ", 0, 1, 1, 1, text)
	
	text = self:CreateText("left", "text", -0.8, 0.0, -1.99999, 2.5, 0.08)	
	self:AddTextToTexture("A"..4, "Server Port: ", 0, 1, 1, 1, text)
end

HostMenu2System.RemoveMenu = function(self)

	local entities = self:GetEntities()
	for i = 1, #entities do
		world:KillEntity(entities[i])
	end
	
end


HostMenu2System.CreateButton = function(self, object, folder, posX, posY, posZ, scaleX, scaleY)

	local id = world:CreateNewEntity("Button")
	world:CreateComponentAndAddTo(self.Name.."Element", id)
	self:GetComponent(id, "Model", 0):SetModel(object, folder, 2)
	self:GetComponent(id, "Position", 0):SetFloat3(posX, posY, posZ)
	self:GetComponent(id, "Scale", 0):SetFloat3(scaleX, scaleY, 1)
	self:GetComponent(id, "PickBox", 0):SetFloat2(1, 1)
	self:GetComponent(id, "Rotation", 0):SetFloat3(0, 0, 0)
	self:AddHoverSize(1.1, id)
	return id	
	
end

HostMenu2System.CreateElement = function(self, object, folder, posX, posY, posZ, scaleX, scaleY)

	local id = world:CreateNewEntity("Button")
	world:CreateComponentAndAddTo(self.Name.."Element", id)
	self:GetComponent(id, "Model", 0):SetModel(object, folder, 2)
	self:GetComponent(id, "Position", 0):SetFloat3(posX, posY, posZ)
	self:GetComponent(id, "Scale", 0):SetFloat3(scaleX, scaleY, 1)
	self:GetComponent(id, "PickBox", 0):SetFloat2(1, 1)
	self:GetComponent(id, "Rotation", 0):SetFloat3(0, 0, 0)
	return id	

end

HostMenu2System.CreateCheckbox = function(self, object, folder, posX, posY, posZ, scaleX, scaleY, checked)
	
		local id = world:CreateNewEntity("Checkbox")
		world:CreateComponentAndAddTo(self.Name.."Element", id)
		self:GetComponent(id, "Model", 0):SetModel(object, folder, 2)
		self:GetComponent(id, "Position", 0):SetFloat3(posX, posY, posZ)
		self:GetComponent(id, "Scale", 0):SetFloat3(scaleX, scaleY, 1)
		self:GetComponent(id, "PickBox", 0):SetFloat2(1, 1)
		self:GetComponent(id, "Rotation", 0):SetFloat3(0, 0, 0)
		self:GetComponent(id, "Checkbox", "Checked"):SetBool(checked)
		self:AddHoverSize(1.1, id)
end

HostMenu2System.CreateText = function(self, object, folder, posx, posy, posz, scalex, scaley)

	local id = world:CreateNewEntity("Text")
	world:CreateComponentAndAddTo(self.Name.."Element", id)
	self:GetComponent(id, "Model", 0):SetModel(object, folder, 2)
	self:GetComponent(id, "Position", 0):SetFloat3(posx, posy, posz)
	self:GetComponent(id, "Scale", 0):SetFloat3(scalex, scaley, 1)
	self:GetComponent(id, "Rotation", 0):SetFloat3(0, 0, 0)
	
	return id	
end

HostMenu2System.AddTextToTexture = function(self, n, text, font, r, g, b, button)
	world:CreateComponentAndAddTo("TextTexture", button)
	world:GetComponent(button, "TextTexture", "Name"):SetString(n) -- TODO: NAME CANT BE MORE THAN 3 CHARS? WTF?
	world:GetComponent(button, "TextTexture", "Text"):SetString(text)
	world:GetComponent(button, "TextTexture", "FontIndex"):SetInt(font)
	world:GetComponent(button, "TextTexture", "R"):SetFloat(r)
	world:GetComponent(button, "TextTexture", "G"):SetFloat(g)
	world:GetComponent(button, "TextTexture", "B"):SetFloat(b)
end

HostMenu2System.AddConsoleCommandToButton = function(self, command, button)

	world:CreateComponentAndAddTo("MenuConsoleCommand", button)
	world:GetComponent(button, "MenuConsoleCommand", "Command"):SetString(command)
	
end

HostMenu2System.AddEntityCommandToButton = function(self, command, button)
	world:CreateComponentAndAddTo("MenuEntityCommand", button)
	world:GetComponent(button, "MenuEntityCommand", "ComponentName"):SetString(command)
	
end

HostMenu2System.AddHoverSize = function(self, deltascale, button)

	local sx, sy, sz = self:GetComponent(button, "Scale", 0):GetFloat3()
	world:CreateComponentAndAddTo("HoverSize", button)
	self:GetComponent(button, "HoverSize", 0):SetFloat3(sx*deltascale, sy*deltascale, sz*deltascale)
	
end