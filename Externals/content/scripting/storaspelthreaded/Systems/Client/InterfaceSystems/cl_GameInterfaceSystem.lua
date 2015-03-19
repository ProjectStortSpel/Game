GameInterfaceSystem = System()
GameInterfaceSystem.Name = "GameInterface"
GameInterfaceSystem.RequestRelease = false

GameInterfaceSystem.Initialize = function(self)
	--	Set Name
	self:SetName(self.Name.."System")
	
	--	Toggle EntitiesAdded
	self:UsingUpdate()
	
	--	Set Filter
	self:AddComponentTypeToFilter(self.Name.."Element", FilterType.RequiresOneOf)
end


GameInterfaceSystem.PostInitialize = function(self)

	local aspectX, aspectY = GraphicDevice.GetAspectRatio()

	local menubutton = self:CreateElement("gamemenubutton", "quad", 3.9*aspectX, -1.4, -4, 1.0, 0.5)
	self:AddEntityCommandToButton("GameMenu", menubutton)
	self:AddHoverSize(1.1, menubutton)

	local socialbutton = self:CreateElement("socialmenubutton", "quad", -3.9*aspectX, -1.4, -4, 1.0, 0.5)
	self:AddEntityCommandToButton("RconMenu", socialbutton)
	self:AddHoverSize(1.1, socialbutton)

end

GameInterfaceSystem.Update = function(self, dt)
	if self.RequestRelease then
		local pressedButtons = self:GetEntities("OnPickBoxHit")
		if #pressedButtons > 0 then
			local pressedButton = pressedButtons[1]
			if world:EntityHasComponent(pressedButton, "MenuConsoleCommand") then
				local command = world:GetComponent(pressedButton, "MenuConsoleCommand", "Command"):GetText()
				Console.AddToCommandQueue(command)
			end
			if world:EntityHasComponent(pressedButton, "MenuEntityCommand") then
				local compname = world:GetComponent(pressedButton, "MenuEntityCommand", "ComponentName"):GetText()
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


GameInterfaceSystem.CreateElement = function(self, object, folder, posx, posy, posz, scalex, scaley)
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

GameInterfaceSystem.AddConsoleCommandToButton = function(self, command, button)
	world:CreateComponentAndAddTo("MenuConsoleCommand", button)
	world:GetComponent(button, "MenuConsoleCommand", "Command"):SetText(command)
end

GameInterfaceSystem.AddEntityCommandToButton = function(self, command, button)
	world:CreateComponentAndAddTo("MenuEntityCommand", button)
	world:GetComponent(button, "MenuEntityCommand", "ComponentName"):SetText(command)
end

GameInterfaceSystem.AddHoverSize = function(self, deltascale, button)
	local scale = world:GetComponent(button, "Scale", 0)
	local sx, sy, sz = scale:GetFloat3()
	world:CreateComponentAndAddTo("HoverSize", button)
	local hoversize = world:GetComponent(button, "HoverSize", 0)
	hoversize:SetFloat3(sx*deltascale, sy*deltascale, sz*deltascale)
end
