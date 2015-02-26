LobbySystem = System()
LobbySystem.Name = "LobbyMenu"

LobbySystem.Initialize = function ( self )
	--	Set Name
	self:SetName(self.Name.."System")
	
	--	Toggle EntitiesAdded
	self:UsingUpdate()
	self:UsingEntitiesAdded()
	
	--	Set Filter
	self:AddComponentTypeToFilter(self.Name.."Element", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("LobbyPlayerReady", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("LobbyPlayerStart", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("GameRunning", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("LobbyMenuActive", FilterType.RequiresOneOf)
end

LobbySystem.Update = function(self, dt)
	if Input.GetTouchState(0) == InputState.Released then
		local pressedButtons = self:GetEntities("OnPickBoxHit")
		if #pressedButtons > 0 then
			local pressedButton = pressedButtons[1]
			if world:EntityHasComponent(pressedButton, "MenuEntityCommand") then
				local compname = world:GetComponent(pressedButton, "MenuEntityCommand", "ComponentName"):GetText()
				local id = world:CreateNewEntity()
				world:CreateComponentAndAddTo(compname, id)
			end
		end
	end
end

LobbySystem.EntitiesAdded = function(self, dt, entities)
	local GameRunning = self:GetEntities("GameRunning")
	if #GameRunning > 0 then
		self:RemoveMenu()
	else
		for n = 1, #entities do
			local entityId = entities[n]
			if world:EntityHasComponent( entityId, "LobbyMenuActive" ) then
				self:SpawnMenu()
			elseif world:EntityHasComponent( entityId, self.Name.."Element") then

			elseif world:EntityHasComponent( entityId, "UnitEntityId") then
				self.UpdateMe = true
			elseif world:EntityHasComponent( entityId, "LobbyPlayerReady") then
				Net.SendToServer(Net.StartPack("Server.ReadyCheck"))
				world:KillEntity(entityId)
			elseif world:EntityHasComponent( entityId, "LobbyPlayerStart") then
				Net.SendToServer(Net.StartPack("Server.StartCheck"))
				world:KillEntity(entityId)
			end
		end
	end
end

LobbySystem.SpawnMenu = function(self)
	local button = self:CreateElement("readybutton", "quad", -1, -1.3, -4, 0.8, 0.4)
	self:AddEntityCommandToButton("LobbyPlayerReady", button)
	self:AddHoverSize(1.1, button)
	
	button = self:CreateElement("start", "quad", 1, -1.3, -4, 0.8, 0.4)
	self:AddEntityCommandToButton("LobbyPlayerStart", button)
	self:AddHoverSize(1.1, button)
end

LobbySystem.RemoveMenu = function(self)
	local entities = self:GetEntities()
	for i = 1, #entities do
		local entityId = entities[i]
		if world:EntityHasComponent( entityId, self.Name.."Element" ) then
			world:KillEntity(entities[i])
		end
	end
end

LobbySystem.AddEntityCommandToButton = function(self, command, button)
	world:CreateComponentAndAddTo("MenuEntityCommand", button)
	world:GetComponent(button, "MenuEntityCommand", "ComponentName"):SetText(command)
end

LobbySystem.AddHoverSize = function(self, deltascale, button)
	local scale = world:GetComponent(button, "Scale", 0)
	local sx, sy, sz = scale:GetFloat3()
	world:CreateComponentAndAddTo("HoverSize", button)
	local hoversize = world:GetComponent(button, "HoverSize", 0)
	hoversize:SetFloat3(sx*deltascale, sy*deltascale, sz*deltascale)
end

LobbySystem.CreateElement = function(self, object, folder, posx, posy, posz, scalex, scaley)
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