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
		local camera = graphics:GetCamera()
		local px, py, pz = camera:GetPosition()
		local lx, ly, lz = camera:GetLook()
		local rx, ry, rz = camera:GetRight()
		local ux, uy, uz = camera:GetUp()
		
		local halfentities = #entities/2
		px = px + lx * self.CameraDistance + rx * (-halfentities + i - 0.5) * 0.08 + ux * self.UpOffset
		py = py + ly * self.CameraDistance + ry * (-halfentities + i - 0.5) * 0.08 + uy * self.UpOffset
		pz = pz + lz * self.CameraDistance + rz * (-halfentities + i - 0.5) * 0.08 + uz * self.UpOffset
		
		local rx, ry, rz
		rx = 0.0
		ry = atan2custom(lz, lx)
		rz = atan2custom(ly, math.sqrt(lx * lx + lz * lz))
	  
		local entity = entities[i]
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
	self:AddComponentTypeToFilter("Position", FilterType.Mandatory)
	self:AddComponentTypeToFilter("CardAction", FilterType.Mandatory)
	
	print("Card position System initialized!")
end