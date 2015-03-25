CameraInterestpointSystem = System()

CameraInterestpointSystem.Initialize = function ( self )
	--	Set Name
	self:SetName("CameraInterestpointSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter("CameraInterestPoint", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("CameraOnPlayer", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("Player", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("Unit", FilterType.RequiresOneOf)
end

CameraInterestpointSystem.EntitiesAdded = function(self, dt, entities)
	for n = 1, #entities do
		local entityId = entities[n]
		if world:EntityHasComponent( entityId, "CameraOnPlayer") then
			local players = self:GetEntities("Player")
			for i = 1, #players do
				if not world:EntityHasComponent( players[i], "IsSpectator") then
					local ip = world:GetComponent(players[i], "NetConnection", "IpAddress"):GetText()
					local port = world:GetComponent(players[i], "NetConnection", "Port"):GetInt()
					local unitId = world:GetComponent(players[i], "UnitEntityId", "Id"):GetInt()
					local x, y, z = world:GetComponent(unitId, "Position", 0):GetFloat3(0)
					local dx, dz = world:GetComponent(unitId, "Direction", 0):GetInt2(0)
					
					if world:EntityHasComponent(unitId, "UnitDead") then
						x, z = world:GetComponent(unitId, "Spawnpoint", "X"):GetInt2(0)
					end
					
					-- CAMERA INTEREST POINT
					local cipID = Net.StartPack("Client.SendCIP")
					Net.WriteFloat(cipID, x)
					Net.WriteFloat(cipID, z)
					Net.WriteFloat(cipID, dx)
					Net.WriteFloat(cipID, dz)
					Net.WriteFloat(cipID, 0.8)
					Net.WriteFloat(cipID, 0.5)
					Net.Send(cipID, ip, port)
				end
			end
			world:KillEntity( entityId )
		end
		if world:EntityHasComponent( entityId, "CameraInterestPoint") then
			local x = world:GetComponent(entityId, "CameraInterestPoint", "AtX"):GetFloat(0)
			local z = world:GetComponent(entityId, "CameraInterestPoint", "AtZ"):GetFloat(0)
			local dx = world:GetComponent(entityId, "CameraInterestPoint", "UpX"):GetFloat(0)
			local dz = world:GetComponent(entityId, "CameraInterestPoint", "UpZ"):GetFloat(0)
			local d = world:GetComponent(entityId, "CameraInterestPoint", "Distance"):GetFloat(0)

			-- CAMERA INTEREST POINT
			local cipID = Net.StartPack("Client.SendCIP")
			Net.WriteFloat(cipID, x)
			Net.WriteFloat(cipID, z)
			Net.WriteFloat(cipID, dx)
			Net.WriteFloat(cipID, dz)
			Net.WriteFloat(cipID, d)
			Net.WriteFloat(cipID, 0.5)
			Net.Broadcast(cipID)
			world:KillEntity( entityId )
		end
	end
end