
---------------------------- PlayerMovementSystem

PlayerMovementSystem = System()

PlayerMovementSystem.Initialize = function(self)
	self:SetName("Player Movement System")
	
	self:AddComponentTypeToFilter("Position", FilterType.Mandatory)
	self:AddComponentTypeToFilter("Rotation",FilterType.Mandatory)
	self:AddComponentTypeToFilter("Scale",FilterType.Mandatory)
	self:AddComponentTypeToFilter("Model",FilterType.Mandatory)
	self:AddComponentTypeToFilter("Direction",FilterType.Mandatory)
		
	print("PlayerMovementSystem initialized!")
end

PlayerMovementSystem.Update = function(self, dt)
	
	local entities = self:GetEntities()
	
end
