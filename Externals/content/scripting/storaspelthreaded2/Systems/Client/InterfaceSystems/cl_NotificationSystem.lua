NotificationSystem = System()
NotificationSystem.Name = "Notification"
NotificationSystem.RequestRelease = false

NotificationSystem.Initialize = function(self)
	--	Set Name
	self:SetName("NotificationSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	--self:UsingUpdate()
	
	--	Set Filter
	self:AddComponentTypeToFilter(self.Name, FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter(self.Name.."Popup", FilterType.RequiresOneOf)
end

NotificationSystem.EntitiesAdded = function(self, dt, entities)
	for n = 1, #entities do
		local entity = entities[n]
		
		if world:EntityHasComponent(entity, "NotificationPopup") then
		
			local text = world:GetComponent(entity, "NotificationPopup", 0):GetText()
		
			local popup = world:CreateNewEntity()
			world:CreateComponentAndAddTo("NotificationBox", popup)
			world:SetComponent(popup, "NotificationBox", "Text", text)
			world:SetComponent(popup, "NotificationBox", "ParentMenu", self.Name)
		
			world:KillEntity(entity)
		
		end
		
		
	end
end