RconMenuSystem = System()
RconMenuSystem.Name = "RconMenu"

RconMenuSystem.Initialize = function(self)
	--	Set Name
	self:SetName(self.Name.."System")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	self:UsingUpdate()
	
	--	Set Filter
	self:AddComponentTypeToFilter(self.Name, FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter(self.Name.."Element", FilterType.RequiresOneOf)
end

RconMenuSystem.Update = function(self, dt, taskIndex, taskCount)
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
				local compname = world:GetComponent(pressedButton, "MenuEntityCommand", "ComponentName"):GetString()
				self:RemoveMenu()
				local id = world:CreateNewEntity()
				world:CreateComponentAndAddTo(compname, id)
			end
		else
			self:RemoveMenu()
		end
	end
end

RconMenuSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)
	for n = 1, #entities do
		local entityId = entities[n]
		if world:EntityHasComponent(entityId, self.Name) then
			self:SpawnMenu()
		end
	end
end

RconMenuSystem.SpawnMenu = function(self)
	local background = self:CreateElement("gamemenubackground", "quad", 0, -0, -3.1, 1.5, 1.5)
		
	local button = nil
	button = self:CreateElement("rconreload", "quad", 0, 0.2, -3, 0.6, 0.3)
	self:AddConsoleCommandToButton("rcon reload", button)	
	self:AddHoverSize(1.1, button)	
		
	button = self:CreateElement("rconstart", "quad", 0, -0.2, -3, 0.6, 0.3)
	self:AddConsoleCommandToButton("rcon start", button)
	self:AddHoverSize(1.1, button)	
end

RconMenuSystem.RemoveMenu = function(self)
	local entities = self:GetEntities()
	for i = 1, #entities do
		world:KillEntity(entities[i])
	end
end


RconMenuSystem.CreateElement = function(self, object, folder, posx, posy, posz, scalex, scaley)
	local id = world:CreateNewEntity()
	world:CreateComponentAndAddTo("Model", id)
	world:CreateComponentAndAddTo("Position", id)
	world:CreateComponentAndAddTo("Rotation", id)
	world:CreateComponentAndAddTo("Scale", id)
	world:CreateComponentAndAddTo("PickBox", id)
	world:CreateComponentAndAddTo(self.Name.."Element", id)
	local model = world:GetComponent(id, "Model", 0)
	model:SetModel(object, folder, 2)
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

RconMenuSystem.AddConsoleCommandToButton = function(self, command, button)
	world:CreateComponentAndAddTo("MenuConsoleCommand", button)
	world:GetComponent(button, "MenuConsoleCommand", "Command"):SetString(command)
end

RconMenuSystem.AddEntityCommandToButton = function(self, command, button)
	world:CreateComponentAndAddTo("MenuEntityCommand", button)
	world:GetComponent(button, "MenuEntityCommand", "ComponentName"):SetString(command)
end

RconMenuSystem.AddHoverSize = function(self, deltascale, button)
	local scale = world:GetComponent(button, "Scale", 0)
	local sx, sy, sz = scale:GetFloat3()
	world:CreateComponentAndAddTo("HoverSize", button)
	local hoversize = world:GetComponent(button, "HoverSize", 0)
	hoversize:SetFloat3(sx*deltascale, sy*deltascale, sz*deltascale)
end