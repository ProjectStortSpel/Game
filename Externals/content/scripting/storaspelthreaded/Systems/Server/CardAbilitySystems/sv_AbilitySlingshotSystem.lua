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
	self:AddComponentTypeToFilter("MapSpecs", 				FilterType.RequiresOneOf)
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
			self:AddBullet(mapPosX, mapPosZ, targetPosX, targetPosZ, 0.1)
			
			if not world:EntityHasComponent(units[i], "ActionGuard") then
				local newId = world:CreateNewEntity()
				world:CreateComponentAndAddTo("TakeCardStepsFromUnit", newId)
				world:GetComponent(newId, "TakeCardStepsFromUnit", "Unit"):SetInt(units[i])
			else
				print("BLOCKED BITCH")
			end
			
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
			self:AddBullet(mapPosX, mapPosZ, targetPosX, targetPosZ, 0.1)
			return true
		end
	
	end
	
	return false
	
end

AbilitySlingshotSystem.AddBullet = function(self, posX, posZ, targetPosX, targetPosZ, lerpTime)

	local bullet = world:CreateNewEntity("SlingShotProjectile")
	world:CreateComponentAndAddTo("AddEntityAfterLerp", bullet)

	world:GetComponent(bullet, "Position", 0):SetFloat3(posX, 1, posZ)
	world:GetComponent(bullet, "LerpPosition", "X"):SetFloat(targetPosX)
	world:GetComponent(bullet, "LerpPosition", "Y"):SetFloat(1)
	world:GetComponent(bullet, "LerpPosition", "Z"):SetFloat(targetPosZ)
	world:GetComponent(bullet, "LerpPosition", "Time"):SetFloat(lerpTime*math.abs(posX-targetPosX+posZ-targetPosZ))
	world:GetComponent(bullet, "LerpPosition", "Algorithm"):SetText("NormalLerp")
	world:GetComponent(bullet, "AddEntityAfterLerp", "ComponentName"):SetText("AddBulletImpact")
	
	local audioId = Net.StartPack("Client.PlaySound")
	Net.WriteString(audioId, "Throw")
	Net.WriteBool(audioId, false)
	Net.Broadcast(audioId)

end

AbilitySlingshotSystem.Update = function(self, dt)

	-- Get all entities which has the "UnitSlingShot" component
	local entities = self:GetEntities("UnitSlingShot")
	-- Get the size of the map
	local mapSizeEntity = self:GetEntities("MapSpecs")
	if #mapSizeEntity == 0 then
		return
	end
	local mapSizeX, mapSizeZ = world:GetComponent(mapSizeEntity[1], "MapSpecs", "SizeX"):GetInt2()

	
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
				self:AddBullet(mapPosX-(dirX+dirZ)*0.4, mapPosZ+(dirX-dirZ)*0.4, currentPosX, currentPosZ, 0.1)
				break
			else
				-- Go through all units and check if the projectile collide with something
				if self:CheckUnits(mapPosX-(dirX+dirZ)*0.4, mapPosZ+(dirX-dirZ)*0.4, currentPosX, currentPosZ) then
					hitSomething = true
					break
				end
				-- Go through all notwalkable tiles and check if the proj collide with something
				if self:CheckNotWalkable(mapPosX-(dirX+dirZ)*0.4, mapPosZ+(dirX-dirZ)*0.4, currentPosX, currentPosZ) then
					hitSomething = true
					break
				end
			end
		end
		
		if not world:EntityHasComponent(entities[i], "Animation") then
			world:CreateComponentAndAddTo("Animation", entities[i])
		end
		world:GetComponent(entities[i], "Animation", "Id"):SetInt(5)
		world:GetComponent(entities[i], "Animation", "FrameTime"):SetFloat(0.01)
		world:GetComponent(entities[i], "Animation", "Time"):SetFloat(1)
		
		world:RemoveComponentFrom("UnitSlingShot", entities[i])
	end

	local slingshots = self:GetEntities("SlingShotProjectile")
	for i = 1, #slingshots do
		local posX, _, posZ	 = world:GetComponent(slingshots[i], "Position", 0):GetFloat3()
		
		local tX = world:GetComponent(slingshots[i], "LerpingPosition", "tX"):GetFloat(0)
		local tZ = world:GetComponent(slingshots[i], "LerpingPosition", "tZ"):GetFloat(0)		
		
		if posX == tX and posZ == tZ then
			world:KillEntity(slingshots[i])
		end
	end
	
end