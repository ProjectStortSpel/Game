NewCameraSystem = System()
NewCameraSystem.Camera = nil
NewCameraSystem.CameraUpX = 0
NewCameraSystem.CameraUpZ = -1


NewCameraSystem.Update = function(self, dt)

	local px, py, pz = self.Camera:GetPosition()
	local lx, ly, lz = self.Camera:GetLook()
	local ux, uy, uz = self.Camera:GetUp()
	local rx, ry, rz = self.Camera:GetRight()

	graphics:RenderSimpleText("C_p: "..px.." "..py.." "..pz, 0,41)
	graphics:RenderSimpleText("C_l: "..lx.." "..ly.." "..lz, 0,42)
	graphics:RenderSimpleText("C_u: "..ux.." "..uy.." "..uz, 0,43)
	graphics:RenderSimpleText("C_r: "..rx.." "..ry.." "..rz, 0,44)

	local button = self:GetEntities()
	for i = 1, #button do
		if world:EntityHasComponent(button[i], "OnPickBoxHit") and Input.GetTouchState(0) == InputState.Released then
		
			if world:EntityHasComponent(button[i], "CameraRightButton") then
				local temp = self.CameraUpX
				self.CameraUpX = self.CameraUpZ
				self.CameraUpZ = -1*temp
			end
			if world:EntityHasComponent(button[i], "CameraLeftButton") then
				local temp = self.CameraUpX
				self.CameraUpX = self.CameraUpZ*-1
				self.CameraUpZ = temp
			end

			self.Camera:MoveToAndLookAt(6.5-self.CameraUpX*7.5,10,6.5-self.CameraUpZ*7.5,self.CameraUpX,0,self.CameraUpZ,6.5,-4.5,6.5,1)
		end
	end
	
end

NewCameraSystem.OnEntityAdded = function(self, entityId)
	
end

NewCameraSystem.Initialize = function(self)
	self:SetName("Camera Update System")
	self:AddComponentTypeToFilter("CameraLeftButton", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("CameraRightButton", FilterType.RequiresOneOf)
	print("Camera Update System initialized!")
end

NewCameraSystem.PostInitialize = function(self)
	self.Camera = graphics:GetCamera()
	
	local id = world:CreateNewEntity()
	world:CreateComponentAndAddTo("Position", id)
	world:CreateComponentAndAddTo("Rotation", id)
	world:CreateComponentAndAddTo("Scale", id)
	world:CreateComponentAndAddTo("PickBox", id)
	world:CreateComponentAndAddTo("CameraRightButton", id)	
	local rposition = self:GetComponent(id, "Position", 0)
	rposition:SetFloat3(4, 0, -4)		
	local rscale = self:GetComponent(id, "Scale", 0)
	rscale:SetFloat3(1, 10, 0.5)	
	local rpickbox = self:GetComponent(id, "PickBox", 0)
	rpickbox:SetFloat2(1, 1)
	
	
	local id = world:CreateNewEntity()
	world:CreateComponentAndAddTo("Position", id)
	world:CreateComponentAndAddTo("Rotation", id)
	world:CreateComponentAndAddTo("Scale", id)
	world:CreateComponentAndAddTo("PickBox", id)
	world:CreateComponentAndAddTo("CameraLeftButton", id)		
	local lposition = self:GetComponent(id, "Position", 0)
	lposition:SetFloat3(-4, 0, -4)		
	local lscale = self:GetComponent(id, "Scale", 0)
	lscale:SetFloat3(0.5, 10, 0.5)	
	local lpickbox = self:GetComponent(id, "PickBox", 0)
	lpickbox:SetFloat2(1, 1)
			
	self.Camera:MoveToAndLookAt(6.5-self.CameraUpX*7.5,10,6.5-self.CameraUpZ*7.5,self.CameraUpX,0,self.CameraUpZ,6.5,-4.5,6.5,1)
	
end