GameMenuSystem = System()
GameMenuSystem.Name = "GameMenu"

GameMenuSystem.Update = function(self, dt, taskIndex, taskCount)
	if Input.GetTouchState(0) == InputState.Released then

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
		else
			self:RemoveMenu()
		end
		
	end

end

GameMenuSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)
	for i = 1, #entities do
		local entityId = entities[i]
		if world:EntityHasComponent(entityId, self.Name) then
			self:SpawnMenu()
		end
	end
end

GameMenuSystem.SpawnMenu = function(self)
	local background = self:CreateElement("gamemenubackground", "quad", 0, -0, -3.1, 1.5, 2)
		
	local button = nil
	button = self:CreateElement("options", "quad", 0, 0.6, -3, 0.6, 0.3)
	self:AddEntityCommandToButton("OptionMenu", button)
	self:AddHoverSize(1.1, button)	

	button = self:CreateElement("quit", "quad", 0, -0.6, -3, 0.6, 0.3)
	self:AddConsoleCommandToButton("quit", button)
	self:AddHoverSize(1.1, button)	
end

GameMenuSystem.RemoveMenu = function(self)
	local entities = self:GetEntities()
	for i = 1, #entities do
		world:KillEntity(entities[i])
	end
end

GameMenuSystem.Initialize = function(self)
	self:SetName("GameMenuSystem")
	self:UsingUpdate()
	self:UsingEntitiesAdded()
	self:AddComponentTypeToFilter(self.Name, FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter(self.Name.."Element", FilterType.RequiresOneOf)
end

GameMenuSystem.CreateElement = function(self, object, folder, posx, posy, posz, scalex, scaley)
	local id = world:CreateNewEntity("Button")
	world:CreateComponentAndAddTo(self.Name.."Element", id)
	world:GetComponent(id, "Model", 0):SetModel(object, folder, 2)
	world:GetComponent(id, "Position", 0):SetFloat3(posx, posy, posz)
	world:GetComponent(id, "Scale", 0):SetFloat3(scalex, scaley, 1)
	world:GetComponent(id, "PickBox", 0):SetFloat2(1, 1)
	world:GetComponent(id, "Rotation", 0):SetFloat3(0, 0, 0)
	return id	
end

GameMenuSystem.AddConsoleCommandToButton = function(self, command, button)
	world:CreateComponentAndAddTo("MenuConsoleCommand", button)
	world:GetComponent(button, "MenuConsoleCommand", "Command"):SetString(command)
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


GameMenuSystem.PostInitialize = function(self)
	
	local	newParticle	=	world:CreateNewEntity()
	world:CreateComponentAndAddTo("Position", newParticle)
	world:CreateComponentAndAddTo("Color", newParticle)
	world:CreateComponentAndAddTo("Particle", newParticle)
	
	world:GetComponent(newParticle, "Position", "X"):SetFloat3(10, 1, 7)
	world:GetComponent(newParticle, "Color", "X"):SetFloat3(1.0, 0.0, 0.0)
	
	world:GetComponent(newParticle, "Particle", "Name"):SetText("fire")
	world:GetComponent(newParticle, "Particle", "Texture"):SetText("content/textures/firewhite.png")
	world:GetComponent(newParticle, "Particle", "Particles"):SetInt(100)
	world:GetComponent(newParticle, "Particle", "Lifetime"):SetFloat(700)
	world:GetComponent(newParticle, "Particle", "Scale"):SetFloat(0.05)
	world:GetComponent(newParticle, "Particle", "SpriteSize"):SetFloat(0.6)
	world:GetComponent(newParticle, "Particle", "Id"):SetInt(-1)
	
	
end