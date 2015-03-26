WeatherRainSystem	=	System()
WeatherRainSystem.firstStep = false

WeatherRainSystem.Initialize = function(self)
	--	Set Name
	self:SetName("WeatherRainSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter("DealCards", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("WeatherRain", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("WeatherStep", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("River", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("Unit", 		FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("DirectionalLight", 		FilterType.RequiresOneOf)
end

WeatherRainSystem.EntitiesAdded = function(self, dt, newEntities)
	for n = 1, #newEntities do
		local	tEntity	=	newEntities[n]
		
		if world:EntityHasComponent(tEntity, "WeatherRain") then
			-- Play strong wind sound!
			local audioId = Net.StartPack("Client.PlaySoundC")
			Net.WriteString(audioId, "leavingwater")
			Net.WriteString(audioId, "WeatherRainForcast")
			Net.WriteBool(audioId, false)
			Net.Broadcast(audioId)
			audioId = Net.StartPack("Client.SetSoundVolume")
			Net.WriteString(audioId, "WeatherRainForcast")
			Net.WriteInt(audioId, 20)
			Net.Broadcast(audioId)
			self.firstStep = true
			
			local	dirLight	=	self:GetEntities("DirectionalLight")
			if #dirLight ~= 0 then
				print("change back to rainy dirLight")
				local	R,G,B		=	0.5,0.5,0.6
				world:GetComponent(dirLight[1], "DirectionalLight", 0):SetDirectionalLight(math.sin(math.random(1, 360)), -1.0, math.sin(math.random(1, 360)), 0.35, 0.65, 0.65, R, G, B)
			end
			
		elseif world:EntityHasComponent(tEntity, "WeatherStep") then
			local	currentRain	=	self:GetEntities("WeatherRain")
			if #currentRain ~= 0 then
				if self.firstStep then
					self.firstStep = false
				else
					self:TickWeather()
				end
			end
		elseif world:EntityHasComponent(tEntity, "DealCards") then
			local	currentRain	=	self:GetEntities("WeatherRain")
			if #currentRain ~= 0 then
				world:KillEntity(currentRain)
				local	dirLight	=	self:GetEntities("DirectionalLight")
				if #dirLight ~= 0 then
					local	R,G,B		=	0.8,0.7,0.6
					print("change back to normal dirLight")
					world:GetComponent(dirLight[1], "DirectionalLight", 0):SetDirectionalLight(math.sin(math.random(1, 360)), -1.0, math.sin(math.random(1, 360)), 0.35, 0.65, 0.65, R, G, B)
				end
			end
		end
		
	end
end

WeatherRainSystem.TickWeather = function(self)
	local units = self:GetEntities("Unit")
	local rivers = self:GetEntities("River")
	
	for i = 1, #units do	
	
		local unitX, unitZ = world:GetComponent(units[i], "MapPosition", 0):GetInt2() 

		for j = 1, #rivers do
			
			local riverX, riverZ = world:GetComponent(rivers[j], "MapPosition", 0):GetInt2() 

			if unitX == riverX and unitZ == riverZ then

				local riverDirX, riverDirZ = world:GetComponent(rivers[j], "River", 0):GetInt2()
				local posXs = unitX + riverDirX
				local posZs = unitZ + riverDirZ

				--local id = world:CreateNewEntity()
				--world:CreateComponentAndAddTo("TestRiver", id)
				--world:SetComponent(id, "TestRiver", "Unit", units[i])
				--world:SetComponent(id, "TestRiver", "PosX", posXs)
				--world:SetComponent(id, "TestRiver", "PosZ", posZs)
				
				local id = world:CreateNewEntity()
				world:CreateComponentAndAddTo("SimultaneousMove", id)
				world:GetComponent(id, "SimultaneousMove", "Unit"):SetInt(units[i])
				world:GetComponent(id, "SimultaneousMove", "PosX"):SetInt(posXs)
				world:GetComponent(id, "SimultaneousMove", "PosZ"):SetInt(posZs)
				world:GetComponent(id, "SimultaneousMove", "DirX"):SetInt(riverDirX)
				world:GetComponent(id, "SimultaneousMove", "DirZ"):SetInt(riverDirZ)
				world:GetComponent(id, "SimultaneousMove", "Steps"):SetInt(1)
				world:GetComponent(id, "SimultaneousMove", "SlerpTime"):SetFloat(1)

				break

			end

		end

	end
end