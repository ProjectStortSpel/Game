NewCameraSystem = System()
NewCameraSystem.Camera = nil
NewCameraSystem.CameraUpX = 0
NewCameraSystem.CameraUpZ = -1
NewCameraSystem.CameraLookAtX = 6.5
NewCameraSystem.CameraLookAtZ = 6.5
NewCameraSystem.CameraDistance = 1
NewCameraSystem.mouseX = 0
NewCameraSystem.mouseY = 0
NewCameraSystem.TouchSprite1 = nil
NewCameraSystem.TouchSprite2 = nil
NewCameraSystem.TouchScreen = nil
NewCameraSystem.Pressed = false
NewCameraSystem.Moved = false

NewCameraSystem.Update = function(self, dt)
	if world:EntityHasComponent(self.TouchScreen, "OnPickBoxHit") then
		local move = false
		local mX, mY = graphics:GetTouchPosition()
		local aspectX, aspectY = graphics:GetAspectRatio()
		local rX = mX * aspectX * 2
		local rY = mY * aspectY * 2
		
		aspectX = aspectY / aspectX
		
		if Input.GetTouchState(0) == InputState.Pressed  then
			self.Pressed = true
			self.Moved = false
			self.mouseX, self.mouseY = graphics:GetTouchPosition()	
			local rposition = self:GetComponent(self.TouchSprite1, "Position", 0)
			rposition:SetFloat3(rX, rY, -1)	
			local rposition = self:GetComponent(self.TouchScreen, "Position", 0)
			rposition:SetFloat3(0, 0, -0.10001)					
		end
		if Input.GetTouchState(0) == InputState.Down and self.Pressed == true then
			local dX = mX - self.mouseX
			local dZ = mY - self.mouseY
			local x, y, z = self.Camera:GetPosition()
			if dX > 0.02 * aspectX then
				self.CameraLookAtX = self.CameraLookAtX - dt * dX * 25 * self.CameraUpZ
				self.CameraLookAtZ = self.CameraLookAtZ + dt * dX * 25 * self.CameraUpX
				move = true                                    
			end                                                
			if dX < -0.02 * aspectX then                       
				self.CameraLookAtX = self.CameraLookAtX - dt * dX * 25 * self.CameraUpZ
				self.CameraLookAtZ = self.CameraLookAtZ + dt * dX * 25 * self.CameraUpX
				move = true                                    
			end                                                
			if dZ > 0.02 then                                  
				self.CameraLookAtX = self.CameraLookAtX + dt * dZ * 25 * self.CameraUpX
				self.CameraLookAtZ = self.CameraLookAtZ + dt * dZ * 25 * self.CameraUpZ
				move = true                                    
			end                                                
			if dZ < -0.02 then                                 
				self.CameraLookAtX = self.CameraLookAtX + dt * dZ * 25 * self.CameraUpX
				self.CameraLookAtZ = self.CameraLookAtZ + dt * dZ * 25 * self.CameraUpZ
				move = true
			end

			if move == true then
				self.Moved = true
				self.Camera:SetPosition(self.CameraLookAtX-self.CameraUpX*self.CameraDistance*7.5, y, self.CameraLookAtZ-self.CameraUpZ*self.CameraDistance*7.5)
			end
			local rposition = self:GetComponent(self.TouchSprite2, "Position", 0)
			rposition:SetFloat3(rX, rY, -1)	
		end
		if Input.GetTouchState(0) == InputState.Released and self.Pressed == true then
			self.Pressed = false
			local rposition = self:GetComponent(self.TouchScreen, "Position", 0)
			rposition:SetFloat3(0, 0, -100)
			if self.Moved == false then
				local x, y, z = self.Camera:GetPosition()
				
				if mX > 0.5 - 0.1 * aspectX then
					local temp = self.CameraUpX
					self.CameraUpX = self.CameraUpZ
					self.CameraUpZ = -1*temp
					move = true
				end
				if mX < -0.5 + 0.1 * aspectX then
					local temp = self.CameraUpX
					self.CameraUpX = self.CameraUpZ*-1
					self.CameraUpZ = temp
					move = true
				end
				if mY > 0.5 - 0.1 then
					self.CameraDistance = self.CameraDistance - 0.2
					if self.CameraDistance < 0.4 then
						self.CameraDistance = 0.4
					end
					move = true
				end
				if mY < -0.5 + 0.1 then
					self.CameraDistance = self.CameraDistance + 0.2
					if self.CameraDistance > 1.8 then
						self.CameraDistance = 1.8
					end
					move = true
				end
				if move == true then
					self.Camera:MoveToAndLookAt(self.CameraLookAtX-self.CameraUpX*self.CameraDistance*7.5,self.CameraDistance*10,self.CameraLookAtZ-self.CameraUpZ*self.CameraDistance*7.5,
												self.CameraUpX,0,self.CameraUpZ,
												self.CameraLookAtX,-4.5,self.CameraLookAtZ,
												0.5)
				end	
			end
			local rposition = self:GetComponent(self.TouchSprite1, "Position", 0)
			rposition:SetFloat3(0, 0, 1)
			local rposition = self:GetComponent(self.TouchSprite2, "Position", 0)
			rposition:SetFloat3(0, 0, 1)
		end
	end
end

NewCameraSystem.Initialize = function(self)
	self:SetName("Camera Update System")
	self:AddComponentTypeToFilter("CameraSystemComponent", FilterType.Mandatory)
	print("Camera Update System initialized!")
end

NewCameraSystem.PostInitialize = function(self)
	self.Camera = graphics:GetCamera()
	
	self.TouchScreen = world:CreateNewEntity()
	world:CreateComponentAndAddTo("Position", self.TouchScreen)
	world:CreateComponentAndAddTo("Rotation", self.TouchScreen)
	world:CreateComponentAndAddTo("Scale", self.TouchScreen)
	world:CreateComponentAndAddTo("PickBox", self.TouchScreen)
	local rposition = self:GetComponent(self.TouchScreen, "Position", 0)
	rposition:SetFloat3(0, 0, -100)		
	local rscale = self:GetComponent(self.TouchScreen, "Scale", 0)
	rscale:SetFloat3(1000, 1000, 1000)	
	local pickbox = self:GetComponent(self.TouchScreen, "PickBox", 0)
	pickbox:SetFloat2(1.0, 1.0)
	
	self.TouchSprite1 = world:CreateNewEntity()
	world:CreateComponentAndAddTo("Model", self.TouchSprite1)
	local model = self:GetComponent(self.TouchSprite1, "Model", 0)
	model:SetModel("touch", "quad", 2)
	world:CreateComponentAndAddTo("Position", self.TouchSprite1)
	world:CreateComponentAndAddTo("Rotation", self.TouchSprite1)
	world:CreateComponentAndAddTo("Scale", self.TouchSprite1)
	local rposition = self:GetComponent(self.TouchSprite1, "Position", 0)
	rposition:SetFloat3(0, 0, 1)		
	local rscale = self:GetComponent(self.TouchSprite1, "Scale", 0)
	rscale:SetFloat3(0.1, 0.1, 0.1)	
	
	
	self.TouchSprite2 = world:CreateNewEntity()
	world:CreateComponentAndAddTo("Model", self.TouchSprite2)
	local model = self:GetComponent(self.TouchSprite2, "Model", 0)
	model:SetModel("touch", "quad", 2)
	world:CreateComponentAndAddTo("Position", self.TouchSprite2)
	world:CreateComponentAndAddTo("Rotation", self.TouchSprite2)
	world:CreateComponentAndAddTo("Scale", self.TouchSprite2)	
	local lposition = self:GetComponent(self.TouchSprite2, "Position", 0)
	lposition:SetFloat3(0, 0, 1)		
	local lscale = self:GetComponent(self.TouchSprite2, "Scale", 0)
	lscale:SetFloat3(0.05, 0.05, 0.05)	
			
	self.Camera:MoveToAndLookAt(	self.CameraLookAtX-self.CameraUpX*self.CameraDistance*7.5,self.CameraDistance*10,self.CameraLookAtZ-self.CameraUpZ*self.CameraDistance*7.5,
									self.CameraUpX,0,self.CameraUpZ,
									self.CameraLookAtX,-4.5,self.CameraLookAtZ,
									1)
	
end