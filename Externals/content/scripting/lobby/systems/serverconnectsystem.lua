ServerConnectSystem = System()

ServerConnectSystem.Update = function(self, dt)
end

ServerConnectSystem.Initialize = function(self)
	self:SetName("ServerConnectSystem System")
	print("ServerConnectSystem initialized!")
end

ServerConnectSystem.OnEntityAdded = function(self, entityId)
end

ServerConnectSystem.OnEntityRemoved = function(self, entityId)
end