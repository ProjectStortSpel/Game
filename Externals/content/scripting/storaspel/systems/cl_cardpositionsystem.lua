CardPositionSystem = System()
CardPositionSystem.CameraDistance = 0.5
CardPositionSystem.Scale = 0.1
CardPositionSystem.UpOffset = -0.2

function atan2custom(Y, X)
  local pi = 3.14159265358979
  local product = 0

  if tonumber(Y) == nil or tonumber(X) == nil then
    return 0
  end

  if X == 0 and Y == 0 then
    return 0
  end

  if X == 0 then
    product = pi / 2
    if Y < 0 then
      product = product * 3
    end
  else
    product = math.atan(Y / X)
    if X < 0 then
      product = product + pi
    end
  end
  return product
end

CardPositionSystem.Update = function(self, dt)

	local entities = self:GetEntities()
	for i = 1, #entities do
		
		local index = world:GetComponent(entities[i], "CardIndex", "Index"):GetInt()

		local camera = graphics:GetCamera()
		local px, py, pz = camera:GetPosition()
		local lx, ly, lz = camera:GetLook()
		local rx, ry, rz = camera:GetRight()
		local ux, uy, uz = camera:GetUp()
		
		local offsetYfactor = 1.0

		local entity = entities[i]

		if world:EntityHasComponent(entity, "SelectCard") then

			local data = self:GetComponent(entity, "SelectCard", "Index"):GetInt()
			offsetYfactor = 0.7 - (data*0.08)

		end
		
		local halfentities = #entities/2
		px = px + lx * self.CameraDistance + rx * (-halfentities + index - 0.5) * 0.08 + ux * self.UpOffset * offsetYfactor
		py = py + ly * self.CameraDistance + ry * (-halfentities + index - 0.5) * 0.08 + uy * self.UpOffset * offsetYfactor
		pz = pz + lz * self.CameraDistance + rz * (-halfentities + index - 0.5) * 0.08 + uz * self.UpOffset * offsetYfactor
		
		local rx, ry, rz
		rx = 0.0
		ry = atan2custom(lz, lx)
		rz = atan2custom(ly, math.sqrt(lx * lx + lz * lz))
	  
		local position = self:GetComponent(entity, "Position", 0)
		position:SetFloat3(px, py, pz)
		
		local rotation = self:GetComponent(entity, "Rotation", 0)
		rotation:SetFloat3(-rx, -ry + 0.5 * 3.14159265358979, -rz - 0.5 * 3.14159265358979)
		
		local scale = self:GetComponent(entity, "Scale", 0)
		scale:SetFloat3(self.Scale, self.Scale, self.Scale)
	end
end

CardPositionSystem.Initialize = function(self)
	self:SetName("Card position System")
	--self:AddComponentTypeToFilter("Position", FilterType.Mandatory)
	self:AddComponentTypeToFilter("CardAction", FilterType.Mandatory)
	self:AddComponentTypeToFilter("CardIndex", FilterType.Mandatory)
	
	print("Card position System initialized!")
end

CardPositionSystem.OnEntityAdded = function(self, entityId)

	print("CardPositionSystem.OnEntityAdded")

	world:CreateComponentAndAddTo("Model", entityId)
	world:CreateComponentAndAddTo("Position", entityId)
	world:CreateComponentAndAddTo("Rotation", entityId)
	world:CreateComponentAndAddTo("Scale", entityId)

	local model = self:GetComponent(entityId, "Model", 0)
	local action = self:GetComponent(entityId, "CardAction", 0):GetString()
	if action == "Forward" then
		model:SetModel("forward", "cards")
	elseif action == "Backward" then
		model:SetModel("back", "cards")
	elseif action == "TurnRight" then
		model:SetModel("turnright", "cards")
	elseif action == "TurnLeft" then
		model:SetModel("turnleft", "cards")
	elseif action == "TurnAround" then
		model:SetModel("turnaround", "cards")
	else
		model:SetModel("dodge", "cards")
	end
end