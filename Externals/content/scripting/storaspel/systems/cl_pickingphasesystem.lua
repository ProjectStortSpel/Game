ClientSendCardSystem = System()
ClientSendCardSystem.PickingTimer = 0.0

ClientSendCardSystem.Update = function(self, dt)

	local activeEntities = self:GetEntities()
	
	--	The system should not run when the timer is zero
	if(self.PickingTimer == 0.0 or #activeEntities == 0) then
		return
	end	
	
	--	Recieve cards
	
	--	Decrease the timer and abort the picking phase
	--	if the timer reaches zero
	self.PickingTimer = self.PickingTimer - dt
	if(self.PickingTimer <= 0.0) then
	
		local packetId = Net.StartPack("PickedCardsFromClient")
		Net.WriteInt(packetId, 101)
		Net.WriteString(packetId, "Forward")
		Net.SendToServer(packetId)
		
		print("TIME IS OUT! RANDOM CARDS! (CLIENT)")
		self.PickingTimer = 0.0
	end
	
end

ClientSendCardSystem.Initialize = function(self)
	self:SetName("Picking Phase System")

	self:AddComponentTypeToFilter("PickingPhase", FilterType.Mandatory)
end

ClientSendCardSystem.PostInitialize = function(self)

end

ClientSendCardSystem.OnEntityAdded = function(self, entityId)
	
	local pickingPhase = self:GetComponent(entityId, "PickingPhase", 0)
	self.PickingTimer = pickingPhase:GetFloat()
	print("Time: ", self.PickingTimer)
end

ClientSendCardSystem.OnEntityRemoved = function(self, entityId)
	self.PickingTimer = 0.0
	
	--	Pick randomly
end