AiCardPickingSystem = System()

AiCardPickingSystem.Initialize = function(self)
	self:SetName("AI card picking System")
	
	self:AddComponentTypeToFilter("AICard", FilterType.Mandatory)
	
end

AiCardPickingSystem.Update = function(self, dt)

end

AiCardPickingSystem.OnEntityAdded = function(self, entity)

	print("jo men nu fick jag ett kort")
	
	if world:EntityHasComponent(entity, "DealtCard") then
		local playerid = self:GetComponent(entity, "DealtCard", 0)
		
		local card = self:GetComponent(entity, "CardAction", 0)
		
		print ( playerid .. " gets card " .. card )
		
	else

	end

end
