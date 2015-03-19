HostSettingsSystem = System()
HostSettingsSystem.settingsId = -1

HostSettingsSystem.Initialize = function(self)
	--	Set Name
	self:SetName("HostSettingsSystem")
	self:UsingEntitiesAdded()

	self:AddComponentTypeToFilter("HostSettings", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("MapSpecs", FilterType.RequiresOneOf)
	self.settingsId = -1
end

HostSettingsSystem.EntitiesAdded = function(self, dt, addedEntities)

	for n = 1, #addedEntities do
		local entity = addedEntities[n]
		
		if world:EntityHasComponent(entity, "HostSettings") then
			
			self.settingsId = entity
			local name 		= world:GetComponent(entity, "HostSettings", "Name"):GetText()
			local map 		= world:GetComponent(entity, "HostSettings", "Map"):GetText()
			local addais 	= world:GetComponent(entity, "HostSettings", "AddAIs"):GetInt()
			local fillai 	= world:GetComponent(entity, "HostSettings", "FillAI"):GetInt()
			local autoStart 	= world:GetComponent(entity, "HostSettings", "AutoStart"):GetInt()
			local allowSpec	= world:GetComponent(entity, "HostSettings", "AllowSpectators"):GetInt()
		
			-- Load Map
			local id = world:CreateNewEntity()
			world:CreateComponentAndAddTo("CreateMap", id)
			world:SetComponent(id, "CreateMap", "MapName", map)
		
			print("name: " .. name)
			print("map: " .. map)
			print("addais: " .. tostring(addais))
			print("fillai: " .. tostring(fillai))
			print("autoStart: " .. tostring(autoStart))
			print("allowSpec: " .. tostring(allowSpec))
			
			
		end
		
		if world:EntityHasComponent(entity, "MapSpecs") then
			if self.settingsId == -1 then
				return
			end

			
			if world:GetComponent(self.settingsId, "HostSettings", "FillAI"):GetInt() == 1 then
				Console.AddToCommandQueue("AI")
			end
			
			local addais = world:GetComponent(self.settingsId, "HostSettings", "AddAIs"):GetInt()
			
			if 0 < addais then
				local command = "AddAI " .. tostring(addais)
				Console.AddToCommandQueue(command)
			end			
		end
		
	end 
end