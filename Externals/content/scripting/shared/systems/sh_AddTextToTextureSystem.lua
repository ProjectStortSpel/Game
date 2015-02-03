AddTextToTextureSystem = System()
AddTextToTextureSystem.Name = "ConnectMenu"
AddTextToTextureSystem.ServerStartIndex = 1
AddTextToTextureSystem.ServerEndIndex = 1
AddTextToTextureSystem.doRefresh = true

AddTextToTextureSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)
	for i = 1, #entities do
		local entityId = entities[i]
		local ModelId = self:GetComponent(entityId, "Render", "ModelId"):GetInt(0)
		local N = self:GetComponent(entityId, "TextTexture", "Name"):GetString(0)
		local Text = self:GetComponent(entityId, "TextTexture", "Text"):GetString(0)
		local FontIndex = self:GetComponent(entityId, "TextTexture", "FontIndex"):GetInt(0)
		local R = self:GetComponent(entityId, "TextTexture", "R"):GetFloat(0)
		local G = self:GetComponent(entityId, "TextTexture", "G"):GetFloat(0)
		local B = self:GetComponent(entityId, "TextTexture", "B"):GetFloat(0)
		local ratio = GraphicDevice.CreateTextTexture(N, Text, FontIndex, R, G, B)
		local scalex, scaley, scalez = self:GetComponent(entityId, "Scale", 0):GetFloat3(0)
		if scaley*ratio > scalex then
			self:GetComponent(entityId, "Scale", 0):SetFloat3(scalex, scalex/ratio, scalez)
		else
			self:GetComponent(entityId, "Scale", 0):SetFloat3(scaley*ratio, scaley, scalez)
		end
		GraphicDevice.ChangeModelTexture(ModelId, N)
		world:RemoveComponentFrom("TextTexture", entityId)
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