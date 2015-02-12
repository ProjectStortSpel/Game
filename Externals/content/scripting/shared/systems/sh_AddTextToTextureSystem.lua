AddTextToTextureSystem = System()

AddTextToTextureSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)
	for i = 1, #entities do
		local entityId = entities[i]
		local ModelId = world:GetComponent(entityId, "Render", "ModelId"):GetInt(0)
		local N = world:GetComponent(entityId, "TextTexture", "Name"):GetText(0)
		local Text = world:GetComponent(entityId, "TextTexture", "Text"):GetText(0)
		local FontIndex = world:GetComponent(entityId, "TextTexture", "FontIndex"):GetInt(0)
		local R = world:GetComponent(entityId, "TextTexture", "R"):GetFloat(0)
		local G = world:GetComponent(entityId, "TextTexture", "G"):GetFloat(0)
		local B = world:GetComponent(entityId, "TextTexture", "B"):GetFloat(0)
		local ratio = GraphicDevice.CreateTextTexture(N, Text, FontIndex, R, G, B)
		local scalex, scaley, scalez = world:GetComponent(entityId, "Scale", 0):GetFloat3(0)
		if scaley*ratio > scalex then
			world:GetComponent(entityId, "Scale", 0):SetFloat3(scalex, scalex/ratio, scalez)
		else
			world:GetComponent(entityId, "Scale", 0):SetFloat3(scaley*ratio, scaley, scalez)
		end
		GraphicDevice.ChangeModelTexture(ModelId, N)
	end
end

AddTextToTextureSystem.PostInitialize = function(self)
	GraphicDevice.AddFont("content/fonts/verdanab.ttf", 72)
end

AddTextToTextureSystem.Initialize = function(self)
	self:SetName("AddTextToTextureSystem")
	self:UsingEntitiesAdded()
	self:AddComponentTypeToFilter("TextTexture", FilterType.Mandatory)
	self:AddComponentTypeToFilter("Render", FilterType.Mandatory)
	self:AddComponentTypeToFilter("Scale", FilterType.Mandatory)
end