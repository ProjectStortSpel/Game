StartUpSystem = System()
local TotalTime = 0

StartUpSystem.Update = function(self, dt)

end

StartUpSystem.Initialize = function(self)
	self:SetName("Start Up System")
	
	print("Start Up System initialized!")
end

StartUpSystem.PostInitialize = function(self)

end

StartUpSystem.OnEntityAdded = function(self, entityId)

end

StartUpSystem.OnEntityRemoved = function(self, entityId)
end