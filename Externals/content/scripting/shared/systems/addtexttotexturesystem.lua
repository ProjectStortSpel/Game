AddTextToTextureSystem = System()
AddTextToTextureSystem.Name = "ConnectMenu"
AddTextToTextureSystem.ServerStartIndex = 1
AddTextToTextureSystem.ServerEndIndex = 1
AddTextToTextureSystem.doRefresh = true

AddTextToTextureSystem.OnEntityAdded = function(self, entityId)
	local ModelId = self:GetComponent(entityId, "Render", "ModelId"):GetInt(0)
	print(ModelId)
	local N = self:GetComponent(entityId, "TextTexture", "Name"):GetString(0)
	local Text = self:GetComponent(entityId, "TextTexture", "Text"):GetString(0)
	local FontIndex = self:GetComponent(entityId, "TextTexture", "FontIndex"):GetInt(0)
	local R = self:GetComponent(entityId, "TextTexture", "R"):GetFloat(0)
	local G = self:GetComponent(entityId, "TextTexture", "G"):GetFloat(0)
	local B = self:GetComponent(entityId, "TextTexture", "B"):GetFloat(0)
	local ratio = graphics:CreateTextTexture(N, Text, FontIndex, R, G, B)
	local scalex, scaley, scalez = self:GetComponent(entityId, "Scale", 0):GetFloat3(0)
	self:GetComponent(entityId, "Scale", 0):SetFloat3(scaley*ratio, scaley, scalez)
	print(ratio)
	graphics:ChangeModelTexture(ModelId, N)
	world:RemoveComponentFrom("TextTexture", entityId)
end

AddTextToTextureSystem.PostInitialize = function(self)
	graphics:AddFont("content/fonts/verdanab.ttf", 72)
end

AddTextToTextureSystem.Initialize = function(self)
	self:SetName("AddTextToTextureSystem")
	self:AddComponentTypeToFilter("TextTexture", FilterType.Mandatory)
	self:AddComponentTypeToFilter("Render", FilterType.Mandatory)
	self:AddComponentTypeToFilter("Scale", FilterType.Mandatory)
end