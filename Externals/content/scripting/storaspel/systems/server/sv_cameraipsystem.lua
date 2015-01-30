CameraInterestPointSystem = System()

CameraInterestPointSystem.Initialize = function ( self )
	self:SetName("CameraInterestPointSystem")
	self:AddComponentTypeToFilter("CameraOnPlayer", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("Player", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("Unit", FilterType.RequiresOneOf)
end

CameraInterestPointSystem.OnEntityAdded = function(self, entityId)
	if world:EntityHasComponent( entityId, "CameraOnPlayer") then
		local players = self:GetEntities("Player")
		for i = 1, #players do
			if not world:EntityHasComponent( players[i], "IsSpectator") then
				local ip = world:GetComponent(players[i], "NetConnection", "IpAddress"):GetString()
				local port = world:GetComponent(players[i], "NetConnection", "Port"):GetInt()
				local unitId = world:GetComponent(players[i], "UnitEntityId", "Id"):GetInt()
				local x, y, z = world:GetComponent(unitId, "Position", 0):GetFloat3(0)
				local dx, dz = world:GetComponent(unitId, "Direction", 0):GetInt2(0)
				
				print(ry)
				
				-- CAMERA INTEREST POINT
				local cipID = Net.StartPack("Client.SendCIP")
				Net.WriteFloat(cipID, x)
				Net.WriteFloat(cipID, z)
				Net.WriteFloat(cipID, dx)
				Net.WriteFloat(cipID, dz)
				Net.WriteFloat(cipID, 1)
				Net.Broadcast(cipID)
			end
		end
		world:KillEntity( entityId )
	end
	
end