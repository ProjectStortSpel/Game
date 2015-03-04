DirectionalLightSystem = System()
DirectionalLightSystem.TotalTime = 1.0

DirectionalLightSystem.Initialize = function(self)
	--	Set Name
	self:SetName("DirectionalLightSystem")
	
	--	Toggle EntitiesAdded
	--self:UsingUpdate()

	--	Set Filter
	self:AddComponentTypeToFilter("DirectionalLight", FilterType.Mandatory)
end

DirectionalLightSystem.PostInitialize = function(self)
	
	local entity = world:CreateNewEntity()
	world:CreateComponentAndAddTo("DirectionalLight", entity)
	world:CreateComponentAndAddTo("SyncNetwork", entity)
    local directionalLight = world:GetComponent(entity, "DirectionalLight", 0)
	directionalLight:SetDirectionalLight(-0.38, -1.0, 0.7, 0.3, 0.7, 0.7, 0.7, 0.75, 0.85)
	
end

--DirectionalLightSystem.Update = function(self, dt)
--	
--	self.TotalTime = self.TotalTime + dt
--	local directionalLight = self:GetEntities()
--	if #directionalLight == 0 then
--		return
--	end
--		
--	local dirLight = world:GetComponent(directionalLight[1], "DirectionalLight", 0) 
--	local dX, dY, dZ, inta, intd, ints, colorR, colorG, colorB = dirLight:GetDirectionalLight()
--	dX = math.sin( 0.05 * self.TotalTime ) 
--	dY = -1.0
--	dZ = math.cos( 0.05 * self.TotalTime )
--	
--	dirLight:SetDirectionalLight(dX, dY, dZ, inta, intd, ints, colorR, colorG, colorB)
--end
