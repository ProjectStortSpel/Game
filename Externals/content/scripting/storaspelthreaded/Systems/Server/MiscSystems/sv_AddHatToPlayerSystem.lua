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
	self:AddComponentTypeToFilter("UnitEntityId", FilterType.RequiresOneOf)
	
	-- Setting up
	self:PopulateHats()
end

AddHatToPlayerSystem.PopulateHats = function(self)
	self.HatTemplates[#self.HatTemplates + 1] = "totem"
	self.HatTemplates[#self.HatTemplates + 1] = "smallstone"
end

AddHatToPlayerSystem.SwitchHatOnUnit = function(self, player, Offset)
	local thisip = world:GetComponent(player, "NetConnection", "IpAddress"):GetText()
	local thisport = world:GetComponent(player, "NetConnection", "Port"):GetInt()
	local entities = self:GetEntities("UnitEntityId")
	for i = 1, #entities do
		local entityId = entities[i]
		local ip = world:GetComponent(entityId, "NetConnection", "IpAddress"):GetText()
		local port = world:GetComponent(entityId, "NetConnection", "Port"):GetInt()
		if ip == thisip and port == thisport then
			
			local unitId = world:GetComponent(entityId, "UnitEntityId", "Id"):GetInt()
			
			if world:EntityHasComponent(unitId, "Hat") then
				local hatNr = world:GetComponent(unitId, "Hat", "Id"):GetInt(0)
				local hatId = world:GetComponent(unitId, "Hat", "hatId"):GetInt(0)
				world:KillEntity(hatId)
				hatNr = (hatNr % #self.HatTemplates) + Offset
				self:SetHatToUnit(hatNr, unitId)
			else
				world:CreateComponentAndAddTo("Hat", unitId)
				self:SetHatToUnit(1, unitId)
			end
		
			break
		end
	end	
end

AddHatToPlayerSystem.SetHatToUnit = function(self, hatId, unitId)
	world:GetComponent(unitId, "Hat", "Id"):SetInt(hatId)
	local hatName = self.HatTemplates[hatId]
	local hatEntity = world:CreateNewEntity(hatName.."Hat")
	world:GetComponent(hatEntity, "Model", 0):SetModel(hatName, hatName, 0)
	local r, g, b = world:GetComponent(unitId, "Color", "X"):GetFloat3(0)
	world:GetComponent(hatEntity, "Color", 0):SetFloat3(r, g, b)
	world:GetComponent(hatEntity, "Parent", 0):SetInt(unitId)
	world:GetComponent(hatEntity, "ParentJoint", 0):SetInt(5)
	world:GetComponent(unitId, "Hat", "hatId"):SetInt(hatEntity)
end

AddHatToPlayerSystem.EntitiesAdded = function(self, dt, entities)
	for n = 1, #entities do
		local entityId = entities[n]
		if world:EntityHasComponent( entityId, "PrevHat") then
			self:SwitchHatOnUnit(entityId, -1)
		elseif world:EntityHasComponent( entityId, "NextHat") then
			self:SwitchHatOnUnit(entityId, 1)
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
		print("new hat request from network")
		local id = world:CreateNewEntity()
		world:CreateComponentAndAddTo("NextHat", id)
		world:CreateComponentAndAddTo("NetConnection", id)
		world:GetComponent(id, "NetConnection", "IpAddress"):SetText(ip)
		world:GetComponent(id, "NetConnection", "Port"):SetInt(port)
		
	end 
)