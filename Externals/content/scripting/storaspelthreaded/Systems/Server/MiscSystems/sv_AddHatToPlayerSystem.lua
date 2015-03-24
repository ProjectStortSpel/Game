AddHatToPlayerSystem = System()
AddHatToPlayerSystem.HatTemplates = {}
AddHatToPlayerSystem.HatTemplates.__mode = "k"

AddHatToPlayerSystem.Initialize = function(self)
	--	Set Name
	self:SetName("AddHatToPlayerSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	
	--	Set Filter
	self:AddComponentTypeToFilter("Hat", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("PrevHat", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("NextHat", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("ThisHat", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("UnitEntityId", FilterType.RequiresOneOf)
	
	-- Setting up
	self:PopulateHats()
end

AddHatToPlayerSystem.PopulateHats = function(self)
	self.HatTemplates[#self.HatTemplates + 1] = "totemHat"
	self.HatTemplates[#self.HatTemplates + 1] = "eggMan"
	self.HatTemplates[#self.HatTemplates + 1] = "stoneHat"
	self.HatTemplates[#self.HatTemplates + 1] = "totemtopHat"
	self.HatTemplates[#self.HatTemplates + 1] = "islandHat"
	self.HatTemplates[#self.HatTemplates + 1] = "bushHat"
	self.HatTemplates[#self.HatTemplates + 1] = "topHat"
	
end

AddHatToPlayerSystem.SwitchHatOnUnit = function(self, player, Offset)
	local thisip = world:GetComponent(player, "NetConnection", "IpAddress"):GetText()
	local thisport = world:GetComponent(player, "NetConnection", "Port"):GetInt()
	local players = self:GetEntities("UnitEntityId")
	for i = 1, #players do
		local playerId = players[i]
		local ip = world:GetComponent(playerId, "NetConnection", "IpAddress"):GetText()
		local port = world:GetComponent(playerId, "NetConnection", "Port"):GetInt()
		if ip == thisip and port == thisport then
			
			local unitId = world:GetComponent(playerId, "UnitEntityId", "Id"):GetInt()
			
			if world:EntityHasComponent(unitId, "Hat") then
				local hatNr = world:GetComponent(unitId, "Hat", "Id"):GetInt(0)
				local hatId = world:GetComponent(unitId, "Hat", "hatId"):GetInt(0)
				world:KillEntity(hatId)
				hatNr = (hatNr + Offset) % (#self.HatTemplates + 1)
				world:GetComponent(unitId, "Hat", "Id"):SetInt(hatNr)
				if hatNr > 0 then
					self:SetHatToUnit(hatNr, unitId)
				else
					world:RemoveComponentFrom("Hat", unitId)
				end
			else
				local hatNr = (Offset) % (#self.HatTemplates + 1)
				world:CreateComponentAndAddTo("Hat", unitId)
				world:GetComponent(unitId, "Hat", "Id"):SetInt(hatNr)
				self:SetHatToUnit(hatNr, unitId)
			end
			
			break
		end
	end	
end

AddHatToPlayerSystem.SetHatToUnit = function(self, hatId, unitId)

	local hatName = self.HatTemplates[hatId]
	local hatEntity = world:CreateNewEntity(hatName)

	-- SET COLOR
	local hr, hg, hb = world:GetComponent(hatEntity, "Color", "X"):GetFloat3(0)
	if not (hr == 0 and hg == 0 and hb == 0) then 
		local r, g, b = world:GetComponent(unitId, "Color", "X"):GetFloat3(0)
		world:GetComponent(hatEntity, "Color", 0):SetFloat3(r, g, b)
	end
	
	-- SET PARENT
	world:CreateComponentAndAddTo("Parent", hatEntity)
	world:GetComponent(hatEntity, "Parent", 0):SetInt(unitId)
	world:CreateComponentAndAddTo("KillWhenOrphan", hatEntity)
	
	-- SET PARENT JOINT
	if world:EntityHasComponent(hatEntity, "ParentJoint") then 
		world:GetComponent(hatEntity, "ParentJoint", 0):SetInt(5)
	end
	
	-- LERP THE HAT FOR EFFECT
	local sx, sy, sz = world:GetComponent(hatEntity, "Scale", "X"):GetFloat3(0)
	world:CreateComponentAndAddTo("LerpScale", hatEntity)
	world:GetComponent(hatEntity, "LerpScale", "Time", 0):SetFloat4(0.1, sx, sy, sz)
	world:GetComponent(hatEntity, "LerpScale", "Algorithm", 0):SetText("SmoothLerp")
	world:GetComponent(hatEntity, "Scale", "X", 0):SetFloat3(0, 0, 0)
	
	world:GetComponent(unitId, "Hat", "hatId"):SetInt(hatEntity)
end

AddHatToPlayerSystem.EntitiesAdded = function(self, dt, entities)
	for n = 1, #entities do
		local entityId = entities[n]
		if world:EntityHasComponent( entityId, "PrevHat") then
			self:SwitchHatOnUnit(entityId, -1)
			world:KillEntity(entityId)
		elseif world:EntityHasComponent( entityId, "NextHat") then
			self:SwitchHatOnUnit(entityId, 1)
			world:KillEntity(entityId)
		elseif world:EntityHasComponent( entityId, "ThisHat") then
			local hatId = world:GetComponent(entityId, "ThisHat", "hatId"):GetInt()
			local unitId = world:GetComponent(entityId, "ThisHat", "unitId"):GetInt()
			hatId = hatId % (#self.HatTemplates + 1)
			self:SetHatToUnit(hatId, unitId)
			world:KillEntity(entityId)
		end
	end
end

Net.Receive("Server.PrevHat", 
	function(id, ip, port)
		local id = world:CreateNewEntity()
		world:CreateComponentAndAddTo("PrevHat", id)
		world:CreateComponentAndAddTo("NetConnection", id)
		world:GetComponent(id, "NetConnection", "IpAddress"):SetText(ip)
		world:GetComponent(id, "NetConnection", "Port"):SetInt(port)
	end 
)

Net.Receive("Server.NextHat", 
	function(id, ip, port)
		local id = world:CreateNewEntity()
		world:CreateComponentAndAddTo("NextHat", id)
		world:CreateComponentAndAddTo("NetConnection", id)
		world:GetComponent(id, "NetConnection", "IpAddress"):SetText(ip)
		world:GetComponent(id, "NetConnection", "Port"):SetInt(port)
	end 
)