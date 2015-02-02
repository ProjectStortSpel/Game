PickBoxSystem = System()

PickBoxSystem.Update = function(self, dt, taskIndex, taskCount)

	-- Fetch Aspect ratio from graphics:
	local AspectX, AspectY = graphics:GetAspectRatio()

	-- Get touch position for ray
	local tX, tY = graphics:GetTouchPosition()

	-- Calc view space ray dir
	local rX = tX * AspectX * -2
	local rY = tY * AspectY * -2
	
	local t = -10000000
	local hitentity = nil
	local hit = false
	
	---- intersect section
	local entities = self:GetEntities()
	for i = 1, #entities do
		local entity = entities[i]
		
		local scale = self:GetComponent(entity, "Scale", 0)
		local position = self:GetComponent(entity, "Position", 0)
		local pickbox = self:GetComponent(entity, "PickBox", 0)
		
		-- Get the pickbox size
		local halfwidth = pickbox:GetFloat(0) * 0.5
		local halfheight = pickbox:GetFloat(1) * 0.5
		
		-- Get scale and multiply with halfsize
		local sX, sY, sZ = scale:GetFloat3()
		halfwidth = halfwidth * sX
		halfheight = halfheight * sY
		
		-- Get position of object
		local X, Y, Z = position:GetFloat3()
		
		-- Calc ray distance from Z
		local hitX = rX * Z
		local hitY = rY * Z
		
		-- check if pickbox is hit
		if Z < 0.1 and Z > t then
			if hitX > X-halfwidth and hitX < X+halfwidth then
				if hitY > Y-halfheight and hitY < Y+halfheight then
						hit = true
						hitentity = entities[i]
						t = Z
				end
			end
		end
		if world:EntityHasComponent(entities[i], "OnPickBoxHit") then
			world:RemoveComponentFrom("OnPickBoxHit", entities[i])
		end
	end
	if hit == true then
		if not world:EntityHasComponent(hitentity, "OnPickBoxHit") then
			world:CreateComponentAndAddTo("OnPickBoxHit", hitentity)
		end
	end
end

PickBoxSystem.Initialize = function(self)
	self:SetName("PickBox System")
	self:UsingUpdate()
	self:AddComponentTypeToFilter("Position", FilterType.Mandatory)
	self:AddComponentTypeToFilter("Scale", FilterType.Mandatory)
	self:AddComponentTypeToFilter("PickBox", FilterType.Mandatory)
end