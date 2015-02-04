AbilitySlingshotSystem = System()
AbilitySlingshotSystem.Range = 50

AbilitySlingshotSystem.Initialize = function(self)
	--	Set Name
	self:SetName("AbilitySlingshotSystem")
	
	--	Toggle EntitiesAdded
	--self:UsingEntitiesAdded()
	self:UsingUpdate()
	
	--	Filters
	self:AddComponentTypeToFilter("Unit",					FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("UnitSlingShot",			FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("SlingShotProjectile",	FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("NotWalkable", 			FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("MapSize", 				FilterType.RequiresOneOf)
end

AbilitySlingshotSystem.CheckUnits = function(self, mapPosX, mapPosZ, currentPosX, currentPosZ)

	-- Get all units
	local units = self:GetEntities("Unit")
	for i = 1, #units do 
		
		-- Get the position of the unit
		local targetPosX, targetPosZ = world:GetComponent(units[i], "MapPosition", 0):GetInt2()
		-- If the position is the same as the projectiles current position
		-- Add a bullet
		if targetPosX == currentPosX and targetPosZ == currentPosZ then
			self:AddBullet(mapPosX, mapPosZ, targetPosX, targetPosZ, 0.5)
			
			local newId = world:CreateNewEntity()
			world:CreateComponentAndAddTo("TakeCardStepsFromUnit", newId)
			world:GetComponent(newId, "TakeCardStepsFromUnit", "Unit"):SetInt(units[i])
			
			---- Check if the unit is already stunned, if not stun him
			--if not world:EntityHasComponent(units[i], "Stunned") then
			--	world:CreateComponentAndAddTo("Stunned", units[i])
			--end
			
			return true
		end
	
	end
	
	return false
	
end

AbilitySlingshotSystem.CheckNotWalkable = function(self, mapPosX, mapPosZ, currentPosX, currentPosZ)

	local entities = self:GetEntities("NotWalkable")
	for i = 1, #entities do
	
		local targetPosX, targetPosZ = world:GetComponent(entities[i], "MapPosition", 0):GetInt2()
		if targetPosX == currentPosX and targetPosZ == currentPosZ then
			self:AddBullet(mapPosX, mapPosZ, targetPosX, targetPosZ, 0.5)
			return true
		end
	
	end
	
	return false
	
end

AbilitySlingshotSystem.AddBullet = function(self, posX, posZ, targetPosX, targetPosZ, lerpTime)

	local bullet = world:CreateNewEntity("SlingShotProjectile")
	world:GetComponent(bullet, "Position", 0):SetFloat3(posX, 1, posZ)
	world:GetComponent(bullet, "LerpTargetPosition", 0):SetFloat3(targetPosX, 1, targetPosZ)
	world:GetComponent(bullet, "LerpTime", 0):SetFloat2(lerpTime, 0)

end

AbilitySlingshotSystem.Update = function(self, dt, taskIndex, taskCount)

	-- Get all entities which has the "UnitSlingShot" component
	local entities = self:GetEntities("UnitSlingShot")
	-- Get the size of the map
	local mapSizeEntity = self:GetEntities("MapSize")
	local mapSizeX, mapSizeZ = world:GetComponent(mapSizeEntity[1], "MapSize", 0):GetInt2()

	
	for i = 1, #entities do
		local hitSomething = false
		-- Direction and Position of the unit using the ability
		local dirX, dirZ 		= world:GetComponent(entities[i], "Direction", 0):GetInt2()
		local mapPosX, mapPosZ 	= world:GetComponent(entities[i], "MapPosition", 0):GetInt2()
		
		-- Go through all tiles the slingshot has range for and check if it collides with something
		for j = 1, self.Range do
			
			-- If we hit something we should stop checking the for more collisions
			if hitSomething then
				break
			end
			
			-- Get the current tile position
			local currentPosX = mapPosX + (dirX * j)
			local currentPosZ = mapPosZ + (dirZ * j)
			
			-- Check if the current tile is outside the map
			if currentPosX < 1 or currentPosZ < 1
			or currentPosX > mapSizeX or currentPosZ > mapSizeZ then
				-- Outside the map, create a new bullet
				self:AddBullet(mapPosX, mapPosZ, currentPosX, currentPosZ, 0.5)
				break
			else
				-- Go through all units and check if the projectile collide with something
				if self:CheckUnits(mapPosX, mapPosZ, currentPosX, currentPosZ) then
					hitSomething = true
					break
				end
				-- Go through all notwalkable tiles and check if the proj collide with something
				if self:CheckNotWalkable(mapPosX, mapPosZ, currentPosX, currentPosZ) then
					hitSomething = true
					break
				end
			end
		end
		world:RemoveComponentFrom("UnitSlingShot", entities[i])
	end

	local slingshots = self:GetEntities("SlingShotProjectile")
	for i = 1, #slingshots do
		local posX, _, posZ	 = world:GetComponent(slingshots[i], "Position", 0):GetFloat3()
		local tarX, _, tarZ = world:GetComponent(slingshots[i], "LerpTargetPosition", 0):GetFloat3()
		
		if posX == tarX and posZ == tarZ then
			world:KillEntity(slingshots[i])
		end
	end
	
end