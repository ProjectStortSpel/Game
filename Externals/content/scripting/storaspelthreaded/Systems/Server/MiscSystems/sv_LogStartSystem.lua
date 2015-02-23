LogStartSystem = System()

LogStartSystem.Initialize = function(self)
	--	Set Name
	self:SetName("LogStartSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	
	--	Set Filter
	self:AddComponentTypeToFilter("GameRunning", FilterType.RequiresOneOf);
	self:AddComponentTypeToFilter("Unit", FilterType.RequiresOneOf);
end

LogStartSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

	for n = 1, #entities do
		local entity = entities[n]
		if world:EntityHasComponent(entity, "GameRunning") then
			
			--File.Create("gamelog.txt")

			--local units = self:GetEntities("Unit")

			--local file = File.Append("gamelog.txt")

			--for i = 1, #units do
				
			--	local plyNum = world:GetComponent(units[i], "PlayerNumber", "Number"):GetInt()
			--	local mapPosX, mapPosZ = world:GetComponent(units[i], "MapPosition", 0):GetInt2()
			--	File.WriteLine(file, "Unit " .. plyNum .. " " .. mapPosX .. " " .. mapPosZ)

			--end

			--File.Close(file)

		end
	end
end