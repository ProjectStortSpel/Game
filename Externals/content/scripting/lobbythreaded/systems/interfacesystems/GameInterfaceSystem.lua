GameInterfaceSystem = System()
GameInterfaceSystem.Name = "GameInterface"

GameInterfaceSystem.Update = function(self, dt, taskIndex, taskCount)
	if Input.GetTouchState(0) == InputState.Released then

		local pressedButtons = self:GetEntities("OnPickBoxHit")
		if #pressedButtons > 0 then
			local pressedButton = pressedButtons[1]
			if world:EntityHasComponent(pressedButton, "MenuConsoleCommand") then
				local command = world:GetComponent(pressedButton, "MenuConsoleCommand", "Command"):GetString()
				Console.AddToCommandQueue(command)
			end
			if world:EntityHasComponent(pressedButton, "MenuEntityCommand") then
				local compname = world:GetComponent(pressedButton, "MenuEntityCommand", "ComponentName"):GetString()
				local id = world:CreateNewEntity()
				print(compname)
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
	self:AddComponentTypeToFilter(self.Name.."Element", FilterType.RequiresOneOf)
end

GameInterfaceSystem.CreateElement = function(self, object, folder, posx, posy, posz, scalex, scaley)
	local id = world:CreateNewEntity("Button")
	world:CreateComponentAndAddTo(self.Name.."Element", id)
	world:GetComponent(id, "Model", 0):SetModel(object, folder, 2)
	world:GetComponent(id, "Position", 0):SetFloat3(posx, posy, posz)
	world:GetComponent(id, "Scale", 0):SetFloat3(scalex, scaley, 1)
	world:GetComponent(id, "PickBox", 0):SetFloat2(1, 1)
	world:GetComponent(id, "Rotation", 0):SetFloat3(0, 0, 0)
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
	local scale = world:GetComponent(button, "Scale", 0)
	local sx, sy, sz = scale:GetFloat3()
	world:CreateComponentAndAddTo("HoverSize", button)
	local hoversize = world:GetComponent(button, "HoverSize", 0)
	hoversize:SetFloat3(sx*deltascale, sy*deltascale, sz*deltascale)
end

GameInterfaceSystem.PostInitialize = function(self)

	local menubutton = self:CreateElement("host", "quad", 0, 0.5, -5, 1, 0.5)
	--self:AddConsoleCommandToButton("host;gamemode storaspel", menubutton)	
	self:AddEntityCommandToButton("HostMenu2", menubutton)
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