LoadingbarSystem = System()
LoadingbarSystem.Name = "Loadingbar"
LoadingbarSystem.TextEntity = -1

Console.AddCommand("SetLoadingText", function(_command, ...)
	local args = { ... }
	if #args >= 1 then
		if type(args[1]) == "string" then
			local id = world:CreateNewEntity()
			world:CreateComponentAndAddTo("LoadingText", id)
			world:GetComponent(id, "LoadingText", "Text"):SetString(args[1])
		end
	end
end)

LoadingbarSystem.Initialize = function(self)
	self:SetName(self.Name.."System")
	--self:UsingUpdate()

	self:UsingEntitiesAdded()
	
	self:AddComponentTypeToFilter("LoadingText", FilterType.Mandatory)
	
end

LoadingbarSystem.PostInitialize = function(self)

	local id = world:CreateNewEntity()
	world:CreateComponentAndAddTo("LoadingText", id)
	world:GetComponent(id, "LoadingText", "Text"):SetString("Connecting")

end

LoadingbarSystem.EntitiesAdded = function(self, dt, entities)

	for i = 1, #entities do
	
		local text = world:GetComponent(entities[i], "LoadingText", "Text"):GetString()
		self:DeleteTextEntity()

		local id = world:CreateNewEntity()
		world:CreateComponentAndAddTo("Model", id)
		world:CreateComponentAndAddTo("Position", id)
		world:CreateComponentAndAddTo("Rotation", id)
		world:CreateComponentAndAddTo("Scale", id)
		world:CreateComponentAndAddTo("TextTexture", id)
		
		world:GetComponent(id, "Model", 0):SetModel("left", "text", 3, false)
		world:GetComponent(id, "Position", 0):SetFloat3(-0.70, 0.21, -2.0)
		world:GetComponent(id, "Scale", 0):SetFloat3(2.0, 0.095, 1.0)
		world:GetComponent(id, "Rotation", 0):SetFloat3(0.0, 0.0, 0.0)
		world:GetComponent(id, "TextTexture", "Name"):SetText("LoadingText")
		world:GetComponent(id, "TextTexture", "Text"):SetText(text)
		world:GetComponent(id, "TextTexture", "FontIndex"):SetInt(0)
		world:GetComponent(id, "TextTexture", "R"):SetFloat(0.0)
		world:GetComponent(id, "TextTexture", "G"):SetFloat(0.0)
		world:GetComponent(id, "TextTexture", "B"):SetFloat(0.0)

		self.TextEntity = id
		
		world:KillEntity(entities[i])
		
	end
end

LoadingbarSystem.DeleteTextEntity = function(self)
	if self.TextEntity ~= -1 then
		world:KillEntity(self.TextEntity)
		self.TextEntity = -1
	end
end
