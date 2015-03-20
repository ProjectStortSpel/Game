WinScreenSystem = System()

WinScreenSystem.Initialize = function(self)
	--	Set Name
	self:SetName("WinScreenSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	self:UsingEntitiesRemoved()
	
	--	Set Filter
	self:AddComponentTypeToFilter("WinScreen", FilterType.Mandatory)
	self:AddComponentTypeToFilter("Null", FilterType.Excluded)
end
 
WinScreenSystem.EntitiesAdded = function(self, dt, addedEntities)
	
	for n = 1, #addedEntities do
		
		local	timeLeft	=	world:GetComponent(addedEntities[n], "WinScreen", "TimeToDelay"):GetFloat() - dt
		world:GetComponent(addedEntities[n], "WinScreen", "TimeToDelay"):SetFloat(timeLeft)
		
		if timeLeft > 0 then
			world:CreateComponentAndAddTo("Null", addedEntities[n])
		else
			world:KillEntity(addedEntities[n])
		end
	end
	
end

WinScreenSystem.EntitiesRemoved = function(self, dt, addedEntities)
	
	for n = 1, #addedEntities do
		
		if world:EntityHasComponent(addedEntities[n], "Null") then
		
			world:RemoveComponentFrom("Null", addedEntities[n])
		end
		
	end
	
end

Net.Receive("Client.PrintMessage", 
	function( id, ip, port )
		
	local newEnt = world:CreateNewEntity("Button")
	world:CreateComponentAndAddTo("WinScreen", newEnt)
	world:GetComponent(newEnt, "Model", 0):SetModel("winscreen", "quad", 2)
	world:GetComponent(newEnt, "Position", 0):SetFloat3(0, 0.25, -5.0)
	world:GetComponent(newEnt, "Scale", 0):SetFloat3(2, 2, 1)
	world:GetComponent(newEnt, "PickBox", 0):SetFloat2(1, 1)
	world:GetComponent(newEnt, "Rotation", 0):SetFloat3(0, 0, 0)
	world:GetComponent(newEnt, "WinScreen", "TimeToDelay"):SetFloat(2)
	
	newEnt = world:CreateNewEntity()
	world:CreateComponentAndAddTo("DisplayPostMatch", newEnt)
		
	end 
)