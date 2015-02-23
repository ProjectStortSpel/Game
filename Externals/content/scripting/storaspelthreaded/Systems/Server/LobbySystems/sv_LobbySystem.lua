LobbySystem = System()

LobbySystem.Initialize = function ( self )
	--	Set Name
	self:SetName("LobbySystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()

	--	Set Filter
end

LobbySystem.EntitiesAdded = function(self, dt, entities)
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
					
					-- CAMERA INTEREST POINT
					local cipID = Net.StartPack("Client.SendCIP")
					Net.WriteFloat(cipID, 6.5-dx*2)
					Net.WriteFloat(cipID, 6.5-dz*2)
					Net.WriteFloat(cipID, dx)
					Net.WriteFloat(cipID, dz)
					Net.WriteFloat(cipID, 1.0)
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
			Net.Broadcast(cipID)
			world:KillEntity( entityId )
		end
	end
end

Net.Receive("Client.ReadyCheck", 
	function(id, ip, port)
		local entity = world:CreateNewEntity()
		world:CreateComponentAndAddTo("CameraInterestPoint", entity)
		local AtX = Net.ReadFloat(id)
		local AtZ = Net.ReadFloat(id)
		local UpX = Net.ReadFloat(id)
		local UpZ = Net.ReadFloat(id)
		local Distance = Net.ReadFloat(id)
		world:GetComponent(entity, "CameraInterestPoint", "AtX"):SetFloat(AtX)
		world:GetComponent(entity, "CameraInterestPoint", "AtZ"):SetFloat(AtZ)
		world:GetComponent(entity, "CameraInterestPoint", "UpX"):SetFloat(UpX)
		world:GetComponent(entity, "CameraInterestPoint", "UpZ"):SetFloat(UpZ)
		world:GetComponent(entity, "CameraInterestPoint", "Distance"):SetFloat(Distance)
	end 
)