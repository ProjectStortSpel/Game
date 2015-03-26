WeatherRainSystem	=	System()
WeatherRainSystem.firstStep = false

WeatherRainSystem.Initialize = function(self)
	--	Set Name
	self:SetName("WeatherRainSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	self:UsingEntitiesRemoved()

	--	Set Filter
	self:AddComponentTypeToFilter("WeatherRain", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("FlowingWater", FilterType.RequiresOneOf)
end

WeatherRainSystem.EntitiesAdded = function(self, dt, newEntities)
	for n = 1, #newEntities do
		local	tEntity	=	newEntities[n]
		
		if world:EntityHasComponent(tEntity, "WeatherRain") then
		
			local rivers = self:GetEntities("FlowingWater")
			for i = 1, #rivers do
				-- Speed up flowing water here
			end
			
		end
		
	end
end

WeatherRainSystem.EntitiesRemoved = function(self, dt, entities)
	for n = 1, #entities do
		local	tEntity	=	entities[n]
		
		if world:EntityHasComponent(tEntity, "WeatherRain") then
		
			local rivers = self:GetEntities("FlowingWater")
			for i = 1, #rivers do
				-- Slow down flowing water here
			end
			
		end
		
	end
end