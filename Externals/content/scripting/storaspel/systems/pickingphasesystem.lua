PickingPhaseSystem = System()
PickingPhaseSystem.PickingTimer = 0.0

PickingPhaseSystem.Update = function(self, dt)

	local activeEntities = self:GetEntities()
	
	--	The system should not run when the timer is zero
	if(self.PickingTimer == 0.0 or #activeEntities == 0) then
		return
	end	
	
	--	Picking Phase active
	
	
	
	--	Decrease the timer and abort the picking phase
	--	if the timer reaches zero
	self.PickingTimer = self.PickingTimer - dt
	if(self.PickingTimer <= 0.0) then
	
		--	Remove any entities in this system
		for eId = 0, #activeEntities do
			world:KillEntity(eId)
		end
	
	
		print("TIME IS OUT! RANDOM CARDS!")
		self.PickingTimer = 0.0
	end
	
end

PickingPhaseSystem.Initialize = function(self)
	self:SetName("Picking Phase System")

	self:AddComponentTypeToFilter("PickingPhase", FilterType.Mandatory)
end

PickingPhaseSystem.PostInitialize = function(self)

end

PickingPhaseSystem.OnEntityAdded = function(self, entityId)
	
	local pickingPhase = self:GetComponent(entityId, "PickingPhase", 0)
	self.PickingTimer = pickingPhase:GetFloat()
	print("Time: ", self.PickingTimer)
end

PickingPhaseSystem.OnEntityRemoved = function(self, entityId)
	self.PickingTimer = 0.0
end