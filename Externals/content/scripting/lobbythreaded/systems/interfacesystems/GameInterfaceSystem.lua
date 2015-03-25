GameInterfaceSystem = System()
GameInterfaceSystem.Name = "GameInterface"
GameInterfaceSystem.RequestRelease = false
GameInterfaceSystem.Buttons = {}
GameInterfaceSystem.Buttons.__mode = "k"
GameInterfaceSystem.Island = -1
GameInterfaceSystem.Rotation = 0.0


GameInterfaceSystem.Initialize = function(self)
	self:SetName("GameInterfaceSystem")
	self:AddComponentTypeToFilter(self.Name.."Element", FilterType.RequiresOneOf)
end

GameInterfaceSystem.EntitiesAdded = function(self, dt, entities)
	
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
	world:GetComponent(button, "MenuEntityCommand", "ComponentName"):SetText(command)
end

GameInterfaceSystem.AddHoverSize = function(self, deltascale, button)
	local scale = world:GetComponent(button, "Scale", 0)
	local sx, sy, sz = scale:GetFloat3()
	world:CreateComponentAndAddTo("HoverSize", button)
	local hoversize = world:GetComponent(button, "HoverSize", 0)
	hoversize:SetFloat3(sx*deltascale, sy*deltascale, sz*deltascale)
end

GameInterfaceSystem.PostInitialize = function(self)
	
	self:CreateDistantIsland()
end

GameInterfaceSystem.CreateDistantIsland = function(self)

	local	X, Y, Z	=	1, 0, 0
	local id = world:CreateNewEntity()
	world:CreateComponentAndAddTo("Model", id)
	world:CreateComponentAndAddTo("Position", id)
	world:CreateComponentAndAddTo("Rotation", id)
	world:CreateComponentAndAddTo("Scale", id)
	world:CreateComponentAndAddTo("Spin", id)
	world:GetComponent(id, "Spin", 0):SetFloat3(0, 0.25, 0)
	world:GetComponent(id, "Model", 0):SetModel("miniisland", "distantisland", 0)
	world:GetComponent(id, "Position", 0):SetFloat3(X, Y, Z)
	world:GetComponent(id, "Rotation", 0):SetFloat3(0.174532925, 0, 0)
	world:GetComponent(id, "Scale", 0):SetFloat3(2.0, 2.0, 2.0)
	self.Island = id
	
	--  Set the shadow map to look at the island
	GraphicDevice.SetShadowmapBounds(4, 4, X, Y, Z)
	
	--	Generate directional light
	local	R,G,B		=	0.9,0.8,0.7
	local 	newLight 	= 	world:CreateNewEntity()
	world:CreateComponentAndAddTo("DirectionalLight", newLight)
    world:GetComponent(newLight, "DirectionalLight", 0):SetDirectionalLight(0.5, -0.6, -0.5, 0.55, 0.65, 0.65, R, G, B)
	
	--	Move camera
	GraphicDevice.GetCamera():MoveToAndLookAt
	(
		0, 1.2, 5,
		0, 1, -1,
		0, 0, -1,
		7
	)
end

