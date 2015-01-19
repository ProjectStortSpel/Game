ActionCameraSystem = System()
ActionCameraSystem.Camera = nil

ActionCameraSystem.Update = function(self, dt)

	local entities = self:GetEntities("ActionCamera")
	for i = 1, #entities do
		
		if Input.GetKeyState(Key.W) == InputState.Down then
			self.Camera:MoveForward(dt)
		end
		if Input.GetKeyState(Key.S) == InputState.Down then
			self.Camera:MoveBackward(dt)
		end
		if Input.GetKeyState(Key.A) == InputState.Down then
			self.Camera:MoveLeft(dt)
		end
		if Input.GetKeyState(Key.D) == InputState.Down then
			self.Camera:MoveRight(dt)
		end
		
	end
		
end

ActionCameraSystem.Initialize = function(self)
	self:SetName("Camera Update System")
	self:AddComponentTypeToFilter("ActionCamera", FilterType.Mandatory)
	self:AddComponentTypeToFilter("CameraPosition", FilterType.Mandatory)
	self:AddComponentTypeToFilter("CameraLook", FilterType.Mandatory)
	self:AddComponentTypeToFilter("CameraUp", FilterType.Mandatory)
	self:AddComponentTypeToFilter("CameraRight", FilterType.Mandatory)
		
	print("Camera Update System initialized!")
end

ActionCameraSystem.PostInitialize = function(self)
	self.Camera = graphics:GetCamera()
	
	-- CREATE CAMERA FreeLookCamera
	local id = world:CreateNewEntity()
	world:CreateComponentAndAddTo("ActionCamera", id)
	world:CreateComponentAndAddTo("CameraPosition", id)
	world:CreateComponentAndAddTo("CameraLook", id)
	world:CreateComponentAndAddTo("CameraUp", id)
	world:CreateComponentAndAddTo("CameraRight", id)

	local position = self:GetComponent(id, "CameraPosition", 0)

	local px, py, pz = self.Camera:GetPosition()

	position:SetFloat3(px, py, pz)
	
	local look = self:GetComponent(id, "CameraLook", 0)
	local lx, ly, lz = self.Camera:GetLook()
	look:SetFloat3(lx, ly, lz)
	
	local up = self:GetComponent(id, "CameraUp", 0)
	local ux, uy, uz = self.Camera:GetUp()
	look:SetFloat3(ux, uy, uz)
	
	local right = self:GetComponent(id, "CameraRight", 0)
	local rx, ry, rz = self.Camera:GetRight()
	look:SetFloat3(rx, ry, rz)
end