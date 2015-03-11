LoadingScreenSystem	=	System()

LoadingScreenSystem.Initialize = function ( self )
	--	Set Name
	self:SetName("LoadingScreenSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	
	--	Filters
	self:AddComponentTypeToFilter("LoadingScreenDelay", FilterType.Mandatory)
	
end

LoadingScreenSystem.EntitiesAdded = function(self, dt, entities)
	for n = 1, #entities do
		local delay = world:GetComponent(entities[n], "LoadingScreenDelay", "Delay"):GetInt()
		local accessLevel = world:GetComponent(entities[n], "LoadingScreenDelay", "AccessLevel"):GetInt()
		
		if delay > 0 then
			
			delay = delay - 1
			local id = world:CreateNewEntity()
			world:CreateComponentAndAddTo("LoadingScreenDelay", id)
			world:GetComponent(id, "LoadingScreenDelay", "Delay"):SetInt(delay)
			world:GetComponent(id, "LoadingScreenDelay", "AccessLevel"):SetInt(accessLevel)
		
		else
			LoadingScreen.Remove(accessLevel)
		end
		world:KillEntity(entities[n])
	end
	
end
