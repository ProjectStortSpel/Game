GameInterfaceSystem = System()

GameInterfaceSystem.Update = function(self, dt, taskIndex, taskCount)
	if Input.GetTouchState(0) == InputState.Released then

		local pressedButtons = self:GetEntities("OnPickBoxHit")
		if #pressedButtons > 0 then
			local pressedButton = pressedButtons[1]
			if world:EntityHasComponent(pressedButton, "MenuConsoleCommand") then
				local command = self:GetComponent(pressedButton, "MenuConsoleCommand", "Command"):GetString()
				Console.AddToCommandQueue(command)
			end
			if world:EntityHasComponent(pressedButton, "MenuEntityCommand") then
				local compname = self:GetComponent(pressedButton, "MenuEntityCommand", "ComponentName"):GetString()
				local id = world:CreateNewEntity()
				world:CreateComponentAndAddTo(compname, id)
			end
		else
			--self:RemoveMenu()
		end
		
	end
	

end

GameInterfaceSystem.Initialize = function(self)
	self:SetName("GameInterfaceSystem")
	self:UsingUpdate()
	self:AddComponentTypeToFilter("InterfaceElement", FilterType.RequiresOneOf)
end

GameInterfaceSystem.CreateElement = function(self, object, folder, posx, posy, posz, scalex, scaley)
	local id = world:CreateNewEntity()
	world:CreateComponentAndAddTo("Model", id)
	world:CreateComponentAndAddTo("Position", id)
	world:CreateComponentAndAddTo("Rotation", id)
	world:CreateComponentAndAddTo("Scale", id)
	world:CreateComponentAndAddTo("PickBox", id)
	world:CreateComponentAndAddTo("InterfaceElement", id)
	local model = self:GetComponent(id, "Model", 0)
	model:SetModel(object, folder, 3)
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

GameInterfaceSystem.AddConsoleCommandToButton = function(self, command, button)
	world:CreateComponentAndAddTo("MenuConsoleCommand", button)
	world:GetComponent(button, "MenuConsoleCommand", "Command"):SetString(command)
end

GameInterfaceSystem.AddEntityCommandToButton = function(self, command, button)
	world:CreateComponentAndAddTo("MenuEntityCommand", button)
	world:GetComponent(button, "MenuEntityCommand", "ComponentName"):SetString(command)
end

GameInterfaceSystem.AddHoverSize = function(self, deltascale, button)
	local scale = self:GetComponent(button, "Scale", 0)
	local sx, sy, sz = scale:GetFloat3()
	world:CreateComponentAndAddTo("HoverSize", button)
	local hoversize = self:GetComponent(button, "HoverSize", 0)
	hoversize:SetFloat3(sx*deltascale, sy*deltascale, sz*deltascale)
end

GameInterfaceSystem.PostInitialize = function(self)

	local menubutton = self:CreateElement("host", "quad", 0, 0.5, -5, 1, 0.5)
	--self:AddConsoleCommandToButton("host;gamemode storaspel", menubutton)	
	self:AddEntityCommandToButton("HostMenu", menubutton)
	self:AddHoverSize(1.5, menubutton)

	local menubutton = self:CreateElement("join", "quad", 0, -0.5, -5, 1, 0.5)
	self:AddEntityCommandToButton("ConnectMenu", menubutton)
	self:AddHoverSize(1.5, menubutton)

	-- GAME MENU BUTTON
	local menubutton = self:CreateElement("gamemenubutton", "quad", 3.3, -1.4, -4, 0.35, 0.35)
	self:AddEntityCommandToButton("GameMenu", menubutton)
	self:AddHoverSize(1.5, menubutton)
	
	print("ButtonPressedSystem post initialized!")
end