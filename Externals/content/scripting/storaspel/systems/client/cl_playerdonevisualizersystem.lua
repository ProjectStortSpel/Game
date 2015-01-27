PlayerDoneVisualizer = System()

PlayerDoneVisualizer.Initialize = function(self)
	self:SetName("Card position System")
	self:AddComponentTypeToFilter("PickingDone", FilterType.Mandatory)	
	self:AddComponentTypeToFilter("IsSpectator", FilterType.Excluded)	
end

PlayerDoneVisualizer.Update = function(self, dt)

	local players = self:GetEntities()
	local ready = "not ready"
	for i = 1, #players do
	
		local done = world:GetComponent(players[i], "PickingDone", 0):GetInt()
	
		if done == 1 then
			ready = "ready"
		end

		graphics:RenderSimpleText("Player" .. i .. " is " .. ready, 20, (1 * i) + 3)
	
	end
	

end

PlayerDoneVisualizer.OnEntityAdded = function(self, entityId)
	print("PlayerDoneVisualizer.OnEntityAdded")
	
	local players = self:GetEntities()
	print("NoPlayers: " .. #players)
end

Net.Receive("Client.RemotePickingDone", 
	function( id, ip, port )

	print("Client.RemotePickingDone")
	
	local pId = Net.ToClientID(Net.ReadInt(id))
	world:SetComponent(pId, "PickingDone", "Done", 1)
	
	end
)

Net.Receive("Client.RemotePickingStarted", 
	function( id, ip, port )

	local pId = Net.ToClientID(Net.ReadInt(id))
	world:SetComponent(pId, "PickingDone", "Done", 0)

	end
)