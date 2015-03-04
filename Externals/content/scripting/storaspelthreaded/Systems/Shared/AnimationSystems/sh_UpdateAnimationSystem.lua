UpdateAnimationSystem = System()

UpdateAnimationSystem.Initialize = function(self)
	--	Set Name
	self:SetName("UpdateAnimationSystem")
	
	--	Toggle EntitiesAdded
	self:UsingUpdate()
	self:UsingEntitiesAdded()
	self:UsingEntitiesRemoved()

	--	Set Filter
	self:AddComponentTypeToFilter("Model", FilterType.Mandatory)
	self:AddComponentTypeToFilter("Animating", FilterType.Mandatory)
end

UpdateAnimationSystem.EntitiesAdded = function(self, dt, entities)
	for n = 1, #entities do
		local entity = entities[n]

		-- Get Animation
		local animId = world:GetComponent(entity, "Animating", "Id"):GetInt(0)
		local frameTime = world:GetComponent(entity, "Animating", "FrameTime"):GetFloat(0)
		self:SetAnimation(entity, animId, frameTime)
	end
end

UpdateAnimationSystem.EntitiesRemoved = function(self, dt, entities)
	for n = 1, #entities do
		local entity = entities[n]

		-- Get Animation
		self:SetAnimation(entity, 0, 0.04)
	end
end

UpdateAnimationSystem.Update = function(self, dt)
	local entities = self:GetEntities()
	for i = 1, #entities do
		local entity = entities[i]
		
		local _time = world:GetComponent(entity, "Animating", "Time"):GetFloat(0)
		local _timer = world:GetComponent(entity, "Animating", "Timer"):GetFloat(0)
		
		_timer = _timer + dt
		if _time > _timer then
			world:GetComponent(entity, "Animating", "Timer"):SetFloat(_timer, false)
		else
			world:RemoveComponentFrom("Animating", entity)
		end	
	end
end

UpdateAnimationSystem.SetAnimation = function(self, entity, animId, frameTime)
	
	local modelId = 0
	if world:EntityHasComponent(entity, "Render") then
		modelId = world:GetComponent(entity, "Render", "ModelId"):GetInt(0)
	else
		return
	end

	GraphicDevice.SetAnimation(modelId, animId, frameTime)
end