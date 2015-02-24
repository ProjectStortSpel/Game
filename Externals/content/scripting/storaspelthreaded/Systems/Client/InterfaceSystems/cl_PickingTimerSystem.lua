PickingTimerSystem = System()
PickingTimerSystem.size = 0.1
PickingTimerSystem.timer = 0


PickingTimerSystem.Initialize = function ( self )
	--	Set Name
	self:SetName("PickingTimerSystem")
	
	--	Toggle EntitiesAdded and Update
	self:UsingUpdate()
	self:UsingEntitiesAdded()
	
	--	Set Filter
	self:AddComponentTypeToFilter("PickingTimer", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("SetPickingTimer", FilterType.RequiresOneOf)
end

PickingTimerSystem.PostInitialize = function(self)
	local id = world:CreateNewEntity()
	world:CreateComponentAndAddTo("PickingTimer", id)
	world:CreateComponentAndAddTo("Model", id)
	world:CreateComponentAndAddTo("Position", id)
	world:CreateComponentAndAddTo("Rotation", id)
	world:CreateComponentAndAddTo("Scale", id)
	local model = world:GetComponent(id, "Model", 0)
	model:SetModel("shade", "quad", 2) 								--fix here
	local position = world:GetComponent(id, "Position", 0)
	position:SetFloat3(0, 2, -4)
	local scale = world:GetComponent(id, "Scale", 0)
	scale:SetFloat3(10, 0.1, 1)
	local rotation = world:GetComponent(id, "Rotation", 0)
	rotation:SetFloat3(0, 0, 0)
end

PickingTimerSystem.Update = function( self, dt )
	
	local prevTimer = self.timer

	local Timers = self:GetEntities("PickingTimer")
	self.timer = self.timer - dt
	if self.timer < 0 then
		self.timer = 0
	end
	
	if #Timers > 0 then
		local drawTimer = Timers[1]
		local scale = world:GetComponent(drawTimer, "Scale", 0)
		scale:SetFloat3(self.timer*self.size, 0.1, 1)
	end
	
	local ticRate = 1.0
	if self.timer < 10.0 then
		ticRate = 2.0
	end
	if self.timer > 0.0 then
		local prevTime = math.floor(prevTimer * ticRate)
		local currTime = math.floor(self.timer * ticRate)
		if prevTime ~= currTime then
			local tic = currTime % 2
			if tic == 0 then
				Audio.PlaySound("Tic1", false)
			else
				Audio.PlaySound("Tic2", false)
			end
		end
	end
end

PickingTimerSystem.EntitiesAdded = function(self, dt, entities)
	for n = 1, #entities do
		local entity = entities[n]
		if world:EntityHasComponent(entity, "SetPickingTimer") then
			local newTime = world:GetComponent(entity, "SetPickingTimer", 0):GetFloat()
			self.timer = newTime
			world:KillEntity(entity)
		end
	end
end

Net.Receive("Client.SendPickingPhaseTimer", 
	function(id)

	local entity = world:CreateNewEntity()
	world:CreateComponentAndAddTo("SetPickingTimer", entity)

	local pickingTimer = Net.ReadFloat(id)

	local setpickingtimer = world:GetComponent(entity, "SetPickingTimer", 0)
	setpickingtimer:SetFloat(pickingTimer-0.1) -- 0.1 för att göra så att tiden försvinner tidigare än korten tas från spelare
	end 
)