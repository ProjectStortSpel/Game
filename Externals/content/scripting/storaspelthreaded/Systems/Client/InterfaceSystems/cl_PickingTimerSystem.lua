PickingTimerSystem = System()


PickingTimerSystem.TotalSize = 25.0
PickingTimerSystem.Scale = 0.2
PickingTimerSystem.EndOffset = 0.07

PickingTimerSystem.FullChopTime = 1.0

PickingTimerSystem.FullTime = -1.0
PickingTimerSystem.CurrentTime = 0.0
PickingTimerSystem.CurrentChopTime = 0.0
    
PickingTimerSystem.BarEntity = -1
PickingTimerSystem.LeftEndEntity = -1
PickingTimerSystem.RightEndEntity = -1

PickingTimerSystem.Hidden = true
PickingTimerSystem.ChangeHidden = false

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
	local endPosition = self.TotalSize * self.Scale * 0.5 + self.EndOffset

	local id = world:CreateNewEntity()
	world:CreateComponentAndAddTo("PickingTimer", id)
	world:CreateComponentAndAddTo("Model", id)
	world:CreateComponentAndAddTo("Position", id)
	world:CreateComponentAndAddTo("Rotation", id)
	world:CreateComponentAndAddTo("Scale", id)
	world:CreateComponentAndAddTo("Hide", id)
	world:GetComponent(id, "Model", 0):SetModel("timebarshort", "quad", 3)
	world:GetComponent(id, "Position", 0):SetFloat3(0.0, 2.001, -4.0)
	world:GetComponent(id, "Scale", 0):SetFloat3(self.TotalSize * self.Scale, self.Scale, 1.0)
	world:GetComponent(id, "Rotation", 0):SetFloat3(0.0, 0.0, 0.0)
	self.BarEntity = id
	
	local id = world:CreateNewEntity()
	world:CreateComponentAndAddTo("Model", id)
	world:CreateComponentAndAddTo("Position", id)
	world:CreateComponentAndAddTo("Rotation", id)
	world:CreateComponentAndAddTo("Scale", id)
	world:CreateComponentAndAddTo("Hide", id)
	world:GetComponent(id, "Model", 0):SetModel("clock", "quad", 3)
	world:GetComponent(id, "Position", 0):SetFloat3(-endPosition, 2.0, -4.0)
	world:GetComponent(id, "Scale", 0):SetFloat3(self.Scale, self.Scale, 1.0)
	world:GetComponent(id, "Rotation", 0):SetFloat3(0.0, 0.0, 0.0)
	self.LeftEndEntity = id
	
	local id = world:CreateNewEntity()
	world:CreateComponentAndAddTo("Model", id)
	world:CreateComponentAndAddTo("Position", id)
	world:CreateComponentAndAddTo("Rotation", id)
	world:CreateComponentAndAddTo("Scale", id)
	world:CreateComponentAndAddTo("Hide", id)
	world:GetComponent(id, "Model", 0):SetModel("clock_flipped", "quad", 3)
	world:GetComponent(id, "Position", 0):SetFloat3(endPosition, 2.0, -4.0)
	world:GetComponent(id, "Scale", 0):SetFloat3(self.Scale, self.Scale, 1.0)
	world:GetComponent(id, "Rotation", 0):SetFloat3(0.0, 0.0, 0.0)
	self.RightEndEntity = id
end

PickingTimerSystem.Chop = function(self, dt)
	self.CurrentChopTime = self.CurrentChopTime + dt
	if self.CurrentChopTime >= self.FullChopTime then
		self.CurrentChopTime = self.CurrentChopTime - self.FullChopTime
		self:AlignBar()
		self:SpawnWood()
	end
end

PickingTimerSystem.AlignBar = function(self)
	local Transform = {}
	Transform.__mode = "k"
	Transform.Size = self.TotalSize * self.Scale
	Transform.HalfSize = 0.5 * Transform.Size
	Transform.ScaleX = (self.CurrentTime / self.FullTime) * Transform.Size
	Transform.HalfScaleX = 0.5 * Transform.ScaleX
	Transform.BarPosition = -Transform.HalfSize + Transform.HalfScaleX
	Transform.RightEndPosition = Transform.BarPosition + Transform.HalfScaleX + self.EndOffset
	
	world:GetComponent(self.BarEntity, "Position", 0):SetFloat3(Transform.BarPosition, 2.001, -4.0)
	world:GetComponent(self.BarEntity, "Scale", 0):SetFloat3(Transform.ScaleX, self.Scale, 1.0)
	
	world:GetComponent(self.RightEndEntity, "Position", 0):SetFloat3(Transform.RightEndPosition, 2.0, -4.0)
end

PickingTimerSystem.SpawnWood = function(self)
	local Transform = {}
	Transform.__mode = "k"
	Transform.Size = self.TotalSize * self.Scale
	Transform.HalfSize = 0.5 * Transform.Size
	Transform.PositionX = -Transform.HalfSize + ((self.CurrentTime + 0.5 * self.FullChopTime) / self.FullTime) * Transform.Size + 0.5 * self.Scale

	local sideEntity = world:CreateNewEntity()
	world:CreateComponentAndAddTo("Model", sideEntity)
	world:CreateComponentAndAddTo("Position", sideEntity)
	world:CreateComponentAndAddTo("Rotation", sideEntity)
	world:CreateComponentAndAddTo("Scale", sideEntity)
	world:CreateComponentAndAddTo("LerpPosition", sideEntity)
	world:CreateComponentAndAddTo("LerpScale", sideEntity)
	world:CreateComponentAndAddTo("KillAfterLerp", sideEntity)
	world:GetComponent(sideEntity, "Model", 0):SetModel("clock_side", "quad", 3)
	world:GetComponent(sideEntity, "Position", 0):SetFloat3(Transform.PositionX, 2.0, -3.9999)
	world:GetComponent(sideEntity, "Scale", 0):SetFloat3(self.Scale, self.Scale, 1.0)
	world:GetComponent(sideEntity, "Rotation", 0):SetFloat3(0.0, 0.0, 0.0)
	world:GetComponent(sideEntity, "LerpPosition", "Time"):SetFloat4(0.7, Transform.PositionX, 0.0, -3.9999)
	world:GetComponent(sideEntity, "LerpPosition", "Algorithm"):SetText("ExponentialLerp")
	world:GetComponent(sideEntity, "LerpScale", "Time"):SetFloat4(0.7, 0.0, 0.0, 0.0)
	world:GetComponent(sideEntity, "LerpScale", "Algorithm"):SetText("ExponentialLerp")
end

PickingTimerSystem.Update = function( self, dt )
	
	if self.ChangeHidden then
		if self.Hidden then
			world:CreateComponentAndAddTo("Hide", self.BarEntity)
			world:CreateComponentAndAddTo("Hide", self.LeftEndEntity)
			world:CreateComponentAndAddTo("Hide", self.RightEndEntity)
		else
			world:RemoveComponentFrom("Hide", self.BarEntity)
			world:RemoveComponentFrom("Hide", self.LeftEndEntity)
			world:RemoveComponentFrom("Hide", self.RightEndEntity)
		end
		self.ChangeHidden = false
	end

	if self.FullTime ~= -1.0 then
		local PrevTime = self.CurrentTime
		
		self:Chop(dt)
		
		self.CurrentTime = self.CurrentTime - dt
		if self.CurrentTime < 0.0 then
			self:Reset()
		end
		--self:AlignBar()
		
		--if #Timers > 0 then
			--local drawTimer = Timers[1]
			--local scale = world:GetComponent(drawTimer, "Scale", 0)
			--scale:SetFloat3(self.timer*self.size, 0.1, 1)
		--end
		
		-- Audio
		local ticRate = 1.0
		if self.CurrentTime < 10.0 then
			ticRate = 2.0
		end
		if self.CurrentTime > 0.0 then
			local prevTime = math.floor(PrevTime * ticRate)
			local currTime = math.floor(self.CurrentTime * ticRate)
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
end

PickingTimerSystem.EntitiesAdded = function(self, dt, entities)
	for n = 1, #entities do
		local entity = entities[n]
		if world:EntityHasComponent(entity, "SetPickingTimer") then
			local newTime = world:GetComponent(entity, "SetPickingTimer", 0):GetFloat()
			world:KillEntity(entity)
			self:SetPickingTime(newTime)
		end
	end
end

PickingTimerSystem.Show = function(self)
	if self.Hidden then
		self.Hidden = false
		self.ChangeHidden = true
	end
end
PickingTimerSystem.Hide = function(self)
	if not self.Hidden then
		self.Hidden = true
		self.ChangeHidden = true
	end
end

PickingTimerSystem.SetPickingTime = function(self, time)
	self.CurrentTime = time
	if self.FullTime == -1.0 then
		self.FullTime = time
		self:Show()
	end
end

PickingTimerSystem.Reset = function(self)
	self.CurrentTime = 0.0
	self.FullTime = -1.0
	self:Hide()
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