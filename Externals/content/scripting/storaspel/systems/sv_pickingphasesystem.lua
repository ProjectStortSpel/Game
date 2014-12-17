RecievingPickedCards = System()
RecievingPickedCards.PickingTimer = 0.0

Net.Receive
(
	"PickedCardsFromClient",
	function(id, ip, port)
	
		print("Prio: ", Net.ReadInt(id))
		print("Card: ", Net.ReadString(id))
	
	end
)

RecievingPickedCards.Update = function(self, dt)

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
		Net.Send(packetId, "127.0.0.1", 6112)
		
		--	Remove any entities in this system
		for eId = 0, #activeEntities do
			world:KillEntity(eId)
		end
	
	
		print("TIME IS OUT! RANDOM CARDS!")
		self.PickingTimer = 0.0
	end
	
end

RecievingPickedCards.Initialize = function(self)
	self:SetName("Picking Phase System")

	self:AddComponentTypeToFilter("PickingPhase", FilterType.Mandatory)
end

RecievingPickedCards.PostInitialize = function(self)

end

RecievingPickedCards.OnEntityAdded = function(self, entityId)
	
	local pickingPhase = self:GetComponent(entityId, "PickingPhase", 0)
	self.PickingTimer = pickingPhase:GetFloat()
	print("Time: ", self.PickingTimer)
end

RecievingPickedCards.OnEntityRemoved = function(self, entityId)
	self.PickingTimer = 0.0
	
	--	Pick randomly
end