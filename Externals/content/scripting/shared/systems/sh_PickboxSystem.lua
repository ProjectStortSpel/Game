PickBoxSystem = System()

PickBoxSystem.Initialize = function(self)
	--	Set Name
	self:SetName("PickBoxSystem")
	
	--	Toggle EntitiesAdded
	self:UsingUpdate()

	--	Set Filter
	self:AddComponentTypeToFilter("Position", FilterType.Mandatory)
	self:AddComponentTypeToFilter("Scale", FilterType.Mandatory)
	self:AddComponentTypeToFilter("PickBox", FilterType.Mandatory)
end

PickBoxSystem.Update = function(self, dt)
	-- Fetch Aspect ratio from graphics:
	local AspectX, AspectY = GraphicDevice.GetAspectRatio()

	-- Get touch position for ray
	local tX, tY = GraphicDevice.GetTouchPosition()

	-- Calc view space ray dir
	local rX = tX * AspectX * -2
	local rY = tY * AspectY * -2
	
	local t = -10000000
	local newhit = nil
	local hit = false
	local PBhitX = 0
	local PBhitY = 0
	
	---- intersect section
	local entities = self:GetEntities()
	if #entities > 0 then
		local scales = world:GetComponents(entities, "Scale", 0)
		local positions = world:GetComponents(entities, "Position", 0)
		local pickboxes = world:GetComponents(entities, "PickBox", 0)
		
		for i = 1, #entities do
			local entity = entities[i]
			
			-- Get the pickbox size
			local halfwidth, halfheight = pickboxes[i]:GetFloat2(0)
			halfwidth = halfwidth * 0.5
			halfheight = halfheight * 0.5
			
			-- Get scale and multiply with halfsize
			local sX, sY, sZ = scales[i]:GetFloat3()
			halfwidth = halfwidth * sX
			halfheight = halfheight * sY
			
			-- Get position of object
			local X, Y, Z = positions[i]:GetFloat3()
			
			-- Calc ray distance from Z
			local hitX = rX * Z
			local hitY = rY * Z
			
			-- check if pickbox is hit
			if Z < 0.1 and Z > t then
				if hitX > X-halfwidth and hitX < X+halfwidth then
					if hitY > Y-halfheight and hitY < Y+halfheight then
							hit = true
							PBhitX = (hitX - X) / halfwidth * 0.5
							PBhitY = (hitY - Y) / halfheight * 0.5
							newhit = entity
							t = Z
					end
				end
			end
		end
	end
	
	if hit == true then
		-- ADD OnPickBoxHit to newhit
		if not world:EntityHasComponent(newhit, "OnPickBoxHit") then
			world:CreateComponentAndAddTo("OnPickBoxHit", newhit)
		end
		world:GetComponent(newhit, "OnPickBoxHit", "X"):SetFloat(PBhitX)
		world:GetComponent(newhit, "OnPickBoxHit", "Y"):SetFloat(PBhitY)

		-- Clear OnPickBoxHit from entities
		local entities = self:GetEntities("OnPickBoxHit")
		for i = 1, #entities do
			local entity = entities[i]
			if entity ~= newhit then
				world:RemoveComponentFrom("OnPickBoxHit", entity)
			end
		end
	else
		-- Clear OnPickBoxHit from entities
		local entities = self:GetEntities("OnPickBoxHit")
		for i = 1, #entities do
			world:RemoveComponentFrom("OnPickBoxHit", entities[i])
		end
	end
end
