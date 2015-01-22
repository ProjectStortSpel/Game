NewCameraSystem = System()
NewCameraSystem.Camera = nil
NewCameraSystem.CameraUpX = 0
NewCameraSystem.CameraUpZ = -1
NewCameraSystem.CameraLookAtX = 6.5
NewCameraSystem.CameraLookAtZ = 6.5
NewCameraSystem.CameraDistance = 1
NewCameraSystem.mouseX = 0
NewCameraSystem.mouseY = 0


NewCameraSystem.Update = function(self, dt)

	--local button = self:GetEntities()
	--for i = 1, #button do
	--	if world:EntityHasComponent(button[i], "OnPickBoxHit") and Input.GetTouchState(0) == InputState.Released then
	--	
	--		if world:EntityHasComponent(button[i], "CameraRightButton") then
	--			local temp = self.CameraUpX
	--			self.CameraUpX = self.CameraUpZ
	--			self.CameraUpZ = -1*temp
	--		end
	--		if world:EntityHasComponent(button[i], "CameraLeftButton") then
	--			local temp = self.CameraUpX
	--			self.CameraUpX = self.CameraUpZ*-1
	--			self.CameraUpZ = temp
	--		end
    --
	--		self.Camera:MoveToAndLookAt(	self.CameraLookAtX-self.CameraUpX*self.CameraDistance*7.5,self.CameraDistance*10,self.CameraLookAtZ-self.CameraUpZ*self.CameraDistance*7.5,
	--										self.CameraUpX,0,self.CameraUpZ,
	--										self.CameraLookAtX,-4.5,self.CameraLookAtZ,
	--										1)
	--	end
	--end
	
	if Input.GetTouchState(0) == InputState.Pressed then
		self.mouseX, self.mouseY = graphics:GetTouchPosition()
	end
	if Input.GetTouchState(0) == InputState.Down then
		local mX, mY = graphics:GetTouchPosition()
		local x, y, z = self.Camera:GetPosition()
		local move = false
		if mX > 0.45 then
			self.CameraLookAtX = self.CameraLookAtX - dt * 5 * self.CameraUpZ
			self.CameraLookAtZ = self.CameraLookAtZ + dt * 5 * self.CameraUpX
			move = true
		end
		if mX < -0.45 then
			self.CameraLookAtX = self.CameraLookAtX + dt * 5 * self.CameraUpZ
			self.CameraLookAtZ = self.CameraLookAtZ - dt * 5 * self.CameraUpX
			move = true
		end
		if mY > 0.45 then
			self.CameraLookAtX = self.CameraLookAtX + dt * 5 * self.CameraUpX
			self.CameraLookAtZ = self.CameraLookAtZ + dt * 5 * self.CameraUpZ
			move = true
		end
		if mY < -0.45 then
			self.CameraLookAtX = self.CameraLookAtX - dt * 5 * self.CameraUpX
			self.CameraLookAtZ = self.CameraLookAtZ - dt * 5 * self.CameraUpZ
			move = true
		end
		
		if move == true then
			print (self.CameraLookAtX.." "..self.CameraLookAtZ)
			self.Camera:SetPosition(self.CameraLookAtX-self.CameraUpX*self.CameraDistance*7.5, y, self.CameraLookAtZ-self.CameraUpZ*self.CameraDistance*7.5)
		end
	end
	if Input.GetTouchState(0) == InputState.Released then
		local offsetMouseX, offsetMouseY = graphics:GetTouchPosition()
		local dX = offsetMouseX - self.mouseX
		local dZ = offsetMouseY - self.mouseY
		local move = false

		if dX > 0.2 then
			local temp = self.CameraUpX
			self.CameraUpX = self.CameraUpZ
			self.CameraUpZ = -1*temp
			move = true
		end
		if dX < -0.2 then
			local temp = self.CameraUpX
			self.CameraUpX = self.CameraUpZ*-1
			self.CameraUpZ = temp
			move = true
		end
		if dZ > 0.2 then
			self.CameraDistance = self.CameraDistance - 0.2
			move = true
		end
		if dZ < -0.2 then
			self.CameraDistance = self.CameraDistance + 0.2
			move = true
		end
		
		if move == true then
			self.Camera:MoveToAndLookAt(	self.CameraLookAtX-self.CameraUpX*self.CameraDistance*7.5,self.CameraDistance*10,self.CameraLookAtZ-self.CameraUpZ*self.CameraDistance*7.5,
											self.CameraUpX,0,self.CameraUpZ,
											self.CameraLookAtX,-4.5,self.CameraLookAtZ,
											1)
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
	--world:CreateComponentAndAddTo("Model", id)
	--local model = self:GetComponent(id, "Model", 0)
	--model:SetModel("readybutton", "quad", 2)
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
	--world:CreateComponentAndAddTo("Model", id)
	--local model = self:GetComponent(id, "Model", 0)
	--model:SetModel("readybutton", "quad", 2)
	world:CreateComponentAndAddTo("Position", id)
	world:CreateComponentAndAddTo("Rotation", id)
	world:CreateComponentAndAddTo("Scale", id)
	world:CreateComponentAndAddTo("PickBox", id)
	world:CreateComponentAndAddTo("CameraLeftButton", id)		
	local lposition = self:GetComponent(id, "Position", 0)
	lposition:SetFloat3(-4, 0, -4)		
	local lscale = self:GetComponent(id, "Scale", 0)
	lscale:SetFloat3(1, 10, 0.5)	
	local lpickbox = self:GetComponent(id, "PickBox", 0)
	lpickbox:SetFloat2(1, 1)
			
	self.Camera:MoveToAndLookAt(	self.CameraLookAtX-self.CameraUpX*self.CameraDistance*7.5,self.CameraDistance*10,self.CameraLookAtZ-self.CameraUpZ*self.CameraDistance*7.5,
									self.CameraUpX,0,self.CameraUpZ,
									self.CameraLookAtX,-4.5,self.CameraLookAtZ,
									1)
	
end