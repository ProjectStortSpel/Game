AddColorToPlayerSystem = System()
AddColorToPlayerSystem.Colors = {}
AddColorToPlayerSystem.Colors.__mode = "k"

AddColorToPlayerSystem.Initialize = function(self)
	--	Set Name
	self:SetName("AddColorToPlayerSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	
	--	Set Filter
	self:AddComponentTypeToFilter("PlayerColor", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("PrevColor", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("NextColor", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("ThisColor", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("UnitEntityId", FilterType.RequiresOneOf)
	
	-- Setting up
	self:PopulateColors()
end

AddColorToPlayerSystem.PopulateColors = function(self)
	self.Colors[#self.Colors + 1] = "006401" -- GREEN
	self.Colors[#self.Colors + 1] = "EE1111" -- RED
	self.Colors[#self.Colors + 1] = "3377FF" -- BLUE
	self.Colors[#self.Colors + 1] = "FFDB33" -- YELLOW
	self.Colors[#self.Colors + 1] = "FFA6FE" -- PINK
	self.Colors[#self.Colors + 1] = "FE6600" -- ORANGE
	self.Colors[#self.Colors + 1] = "7E2DD2" -- VIOLET
	self.Colors[#self.Colors + 1] = "EEEEEE" -- WHITE
	self.Colors[#self.Colors + 1] = "111111" -- BLACK
	self.Colors[#self.Colors + 1] = "553333" -- BROWN
	self.Colors[#self.Colors + 1] = "01FFFE" -- TEAL
	self.Colors[#self.Colors + 1] = "66FF22" -- LIME GREEN
	self.Colors[#self.Colors + 1] = "010067" -- DARK BLUE
	self.Colors[#self.Colors + 1] = "670001" -- DARK RED
	self.Colors[#self.Colors + 1] = "EE33EE" -- MANGETA
end

AddColorToPlayerSystem.SwitchColorOnUnit = function(self, player, Offset)
	local thisip = world:GetComponent(player, "NetConnection", "IpAddress"):GetText()
	local thisport = world:GetComponent(player, "NetConnection", "Port"):GetInt()
	local players = self:GetEntities("UnitEntityId")
	for i = 1, #players do
		local playerId = players[i]
		local ip = world:GetComponent(playerId, "NetConnection", "IpAddress"):GetText()
		local port = world:GetComponent(playerId, "NetConnection", "Port"):GetInt()
		if ip == thisip and port == thisport then
			
			local unitId = world:GetComponent(playerId, "UnitEntityId", "Id"):GetInt()
			
			if world:EntityHasComponent(unitId, "PlayerColor") then
				local colorNr = world:GetComponent(unitId, "PlayerColor", "Id"):GetInt(0)
				colorNr = ((colorNr - 1 + Offset) % #self.Colors) + 1
				self:SetColorToUnit(colorNr, unitId, Offset)
			--else
			--	local colorNr = (Offset) % (#self.Colors + 1)
			--	world:CreateComponentAndAddTo("PlayerColor", unitId)
			--	world:GetComponent(unitId, "PlayerColor", "Id"):SetInt(colorNr)
			--	self:SetColorToUnit(colorNr, unitId, Offset)
			end
			
			break
		end
	end	
end

AddColorToPlayerSystem.SetColorToUnit = function(self, colorNr, unitId, dir)

	local BusyColors = self:GetEntities("PlayerColor")
	local colorRequest = colorNr

	for i = 1, #self.Colors do
		local colorIsBusy = false
		for j = 1, #BusyColors do
			local busyColor = world:GetComponent(BusyColors[j], "PlayerColor", "Id"):GetInt(0)
			if colorRequest == busyColor then
				colorIsBusy = true
				break
			end
		end
		if colorIsBusy == false then
			break
		end
		colorRequest = ((colorRequest + dir - 1) % #self.Colors) + 1
	end
	
	local R, G, B = self:GetPlayerColor(colorRequest)
	world:GetComponent(unitId, "Color", 0):SetFloat3(R, G, B)
	world:GetComponent(unitId, "PlayerColor", "Id"):SetInt(colorRequest)
	
	--local hatEntity = world:CreateNewEntity(hatName)
	---- SET COLOR
	--local hr, hg, hb = world:GetComponent(hatEntity, "Color", "X"):GetFloat3(0)
	--if not (hr == 0 and hg == 0 and hb == 0) then 
	--	local r, g, b = world:GetComponent(unitId, "Color", "X"):GetFloat3(0)
	--	world:GetComponent(hatEntity, "Color", 0):SetFloat3(r, g, b)
	--end
end

AddColorToPlayerSystem.GetPlayerColor = function(self, colorId)
	if colorId > #self.Colors then
		return 1, 1, 1
	end
	print("colorreq: "..colorId)
	local hex = self.Colors[colorId]
    return tonumber("0x"..hex:sub(1,2))/255, tonumber("0x"..hex:sub(3,4))/255, tonumber("0x"..hex:sub(5,6))/255
end

AddColorToPlayerSystem.EntitiesAdded = function(self, dt, entities)
	for n = 1, #entities do
		local entityId = entities[n]
		if world:EntityHasComponent( entityId, "PrevColor" ) then
			self:SwitchColorOnUnit(entityId, -1)
			world:KillEntity(entityId)
		elseif world:EntityHasComponent( entityId, "NextColor" ) then
			self:SwitchColorOnUnit(entityId, 1)
			world:KillEntity(entityId)
		elseif world:EntityHasComponent( entityId, "ThisColor" ) then
			local colorNr = world:GetComponent(entityId, "ThisColor", "colorNr"):GetInt()
			local unitId = world:GetComponent(entityId, "ThisColor", "unitId"):GetInt()
			colorNr = ((colorNr - 1) % #self.Colors) + 1
			self:SetColorToUnit(colorNr, unitId, 1)
			world:KillEntity(entityId)
		end
	end
end

Net.Receive("Server.PrevColor", 
	function(id, ip, port)
		local id = world:CreateNewEntity()
		world:CreateComponentAndAddTo("PrevColor", id)
		world:CreateComponentAndAddTo("NetConnection", id)
		world:GetComponent(id, "NetConnection", "IpAddress"):SetText(ip)
		world:GetComponent(id, "NetConnection", "Port"):SetInt(port)
	end 
)

Net.Receive("Server.NextColor", 
	function(id, ip, port)
		local id = world:CreateNewEntity()
		world:CreateComponentAndAddTo("NextColor", id)
		world:CreateComponentAndAddTo("NetConnection", id)
		world:GetComponent(id, "NetConnection", "IpAddress"):SetText(ip)
		world:GetComponent(id, "NetConnection", "Port"):SetInt(port)
	end 
)