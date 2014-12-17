ClientConnectSystem = System()

ClientConnectSystem.Update = function(self, dt)
end

ClientConnectSystem.Initialize = function(self)
	self:SetName("ClientConnectSystem System")
	print("ClientConnectSystem initialized!")
end

ClientConnectSystem.OnEntityAdded = function(self, entityId)
end

ClientConnectSystem.OnEntityRemoved = function(self, entityId)
end