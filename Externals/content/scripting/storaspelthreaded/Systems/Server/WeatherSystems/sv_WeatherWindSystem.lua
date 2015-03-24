WeatherWindSystem	=	System()
WeatherWindSystem.CurrentStep	=	0

WeatherWindSystem.Initialize = function(self)
	--	Set Name
	self:SetName("WeatherWindSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter("WeatherStep", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("WeatherWind", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("Unit", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("IsTree", FilterType.RequiresOneOf)
end

WeatherWindSystem.EntitiesAdded = function(self, dt, newEntities)



	for n = 1, #newEntities do
		local	tEntity	=	newEntities[n]
		
		if world:EntityHasComponent(tEntity, "WeatherWind") then
		
			--	Randomize which round to play it on
			local	numberOfSteps	=	5+1 - self.CurrentStep%5
			local	stepToTrigger	=	math.random(1, 5)
			
			world:GetComponent(tEntity, "Weather", "StepToHappen"):SetInt(stepToTrigger)
			world:GetComponent(tEntity, "Weather", "StageToHappen"):SetInt(0)
			world:GetComponent(tEntity, "WeatherWind", "Force"):SetInt(1)
			
			local	dirX, dirZ	=	0,0
			if math.random(1, 100) <= 50 then
				dirX	=	(-1)^math.random(1, 10)
			else
				dirZ	=	(-1)^math.random(1, 10)
			end
			
			world:CreateComponentAndAddTo("Direction", tEntity)
			world:GetComponent(tEntity, "Direction", "X"):SetInt(dirX)
			world:GetComponent(tEntity, "Direction", "Z"):SetInt(dirZ)
			
			local	allTrees	=	self:GetEntities("IsTree")
			for i = 1, #allTrees do
				local Tree = allTrees[i]
				if not world:EntityHasComponent(Tree, "LerpRotation") then
					world:CreateComponentAndAddTo("LerpRotation", Tree)
				end
				local TreeX, TreeY, TreeZ = world:GetComponent(Tree, "Rotation", "X"):GetFloat3(0)
				world:GetComponent(Tree, "LerpRotation", "X"):SetFloat(dirZ*0.1)
				world:GetComponent(Tree, "LerpRotation", "Y"):SetFloat(TreeY)
				world:GetComponent(Tree, "LerpRotation", "Z"):SetFloat(-dirX*0.1)
				world:GetComponent(Tree, "LerpRotation", "Time"):SetFloat(0.5)
				world:GetComponent(Tree, "LerpRotation", "Algorithm"):SetText("NormalLerp")
			end
			
		elseif world:EntityHasComponent(tEntity, "WeatherStep") then
			
			self.CurrentStep	=	self.CurrentStep+1
		end
		
	end
	
	--	Get current weather
	local	currentWind	=	self:GetEntities("WeatherWind")
	if #currentWind ~= 0 then
		print("NUMBER OF WEATHERS: " .. #currentWind)
		self:TickWeather(currentWind[1])
	end
	
end

WeatherWindSystem.TickWeather = function(self, weatherEntity)
	
	--	Reduce steps left
	local	stepsLeft	=	world:GetComponent(weatherEntity, "Weather", "StepToHappen"):GetInt()
	world:GetComponent(weatherEntity, "Weather", "StepToHappen"):SetInt(stepsLeft-1)
	
	if stepsLeft-1 <= 0 then
	
		local	allUnits	=	self:GetEntities("Unit")
		local	allTrees	=	self:GetEntities("IsTree")
		local	dirX, dirZ	=	world:GetComponent(weatherEntity, "Direction", "X"):GetInt2()
		local	windForce	=	world:GetComponent(weatherEntity, "WeatherWind", "Force"):GetInt()
		
		print("Wind direction " .. dirX .. ", " .. dirZ)
		
		for i = 1, #allTrees do
			local Tree = allTrees[i]
			local TreeX, TreeY, TreeZ = world:GetComponent(Tree, "Rotation", "X"):GetFloat3(0)
			
			if not world:EntityHasComponent(Tree, "LerpingRotation") then
				world:CreateComponentAndAddTo("LerpingRotation", Tree)
			end
			world:GetComponent(Tree, "LerpingRotation", "Time"):SetFloat8(0.5, 0, TreeX, TreeY, TreeZ, dirZ*0.5, 0, -dirX*0.5)
			world:GetComponent(Tree, "LerpingRotation", "Algorithm"):SetText("NormalLerp")

			if not world:EntityHasComponent(Tree, "LerpRotation") then
				world:CreateComponentAndAddTo("LerpRotation", Tree)
			end
			world:GetComponent(Tree, "LerpRotation", "X"):SetFloat(0)
			world:GetComponent(Tree, "LerpRotation", "Y"):SetFloat(TreeY)
			world:GetComponent(Tree, "LerpRotation", "Z"):SetFloat(0)
			world:GetComponent(Tree, "LerpRotation", "Time"):SetFloat(1.5)
			world:GetComponent(Tree, "LerpRotation", "Algorithm"):SetText("NormalLerp")
		end
		
		for i = 1, #allUnits do
		
			
			local	tUnit	=	allUnits[i]
			
			if world:EntityHasComponent(tUnit, "ActionGuard") then
				-- SOUND
					local audioId = Net.StartPack("Client.PlaySoundC")
					Net.WriteString(audioId, "BlockVoice" .. math.random(1, 3))
					Net.WriteString(audioId, "BlockVoice" .. tUnit)
					Net.WriteBool(audioId, false)
					Net.Broadcast(audioId)
			elseif not world:EntityHasComponent(tUnit, "UnitDead") then
			
				local	posX, posZ	=	world:GetComponent(tUnit, "MapPosition", "X"):GetInt2()
				
				local id = world:CreateNewEntity()
				world:CreateComponentAndAddTo("SimultaneousMove", id)
				world:GetComponent(id, "SimultaneousMove", "Unit"):SetInt(tUnit)
				world:GetComponent(id, "SimultaneousMove", "PosX"):SetInt(posX + dirX)
				world:GetComponent(id, "SimultaneousMove", "PosZ"):SetInt(posZ + dirZ)
				world:GetComponent(id, "SimultaneousMove", "DirX"):SetInt(dirX)
				world:GetComponent(id, "SimultaneousMove", "DirZ"):SetInt(dirZ)
				world:GetComponent(id, "SimultaneousMove", "Steps"):SetInt(windForce)
				world:GetComponent(id, "SimultaneousMove", "SlerpTime"):SetFloat(0.8)
			end
		end
		
		
		print("WIND!!!")
	
		world:KillEntity(weatherEntity)
	end
	
	
	
	
end









