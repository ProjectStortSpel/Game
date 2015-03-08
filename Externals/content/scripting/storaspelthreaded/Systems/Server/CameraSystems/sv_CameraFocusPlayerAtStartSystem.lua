CameraFocusPlayerAtStartSystem = System()

CameraFocusPlayerAtStartSystem.Initialize = function ( self )
	--	Set Name
	self:SetName("CameraFocusPlayerAtStartSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter("PlayerEntityId", FilterType.Mandatory)
	self:AddComponentTypeToFilter("Spawnpoint", FilterType.Mandatory)
	self:AddComponentTypeToFilter("Direction", FilterType.Mandatory)
end

CameraFocusPlayerAtStartSystem.EntitiesAdded = function(self, dt, entities)
	for n = 1, #entities do
		local unitId = entities[n]
		local playerId = world:GetComponent(unitId, "PlayerEntityId", 0):GetInt(0)

		if world:EntityHasComponent(playerId, "NetConnection") then
			local ip = world:GetComponent(playerId, "NetConnection", "IpAddress"):GetText()
			local port = world:GetComponent(playerId, "NetConnection", "Port"):GetInt()

			local unitDirX, unitDirZ = world:GetComponent(unitId, "Direction", "X"):GetInt2(0)
			local unitSpawnX, unitSpawnZ = world:GetComponent(unitId, "Spawnpoint", "X"):GetInt2(0)

			local cipID = Net.StartPack("Client.SendCIP")
			Net.WriteFloat(cipID, unitSpawnX-unitDirX)
			Net.WriteFloat(cipID, unitSpawnZ-unitDirZ)
			Net.WriteFloat(cipID, -unitDirX)
			Net.WriteFloat(cipID, -unitDirZ)
			Net.WriteFloat(cipID, 0.4)
			Net.Send(cipID, ip, port)
		end
	end
end