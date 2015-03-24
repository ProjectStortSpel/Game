SliderSystem = System()

SliderSystem.Initialize = function(self)
	--	Set Name
	self:SetName("SliderSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	self:UsingUpdate()
	
	--	Set Filter
	self:AddComponentTypeToFilter("MenuSlider", FilterType.Mandatory)
end

SliderSystem.EntitiesAdded = function(self, dt, entities)
	for n = 1, #entities do
		local entity = entities[n]
		if not world:EntityHasComponent(entity, "MenuSliderButton") then
			local currentvalue = world:GetComponent(entity, "MenuSlider", "Current"):GetFloat(0)
			local minvalue = world:GetComponent(entity, "MenuSlider", "Min"):GetFloat()
			local maxvalue = world:GetComponent(entity, "MenuSlider", "Max"):GetFloat()
			local sliderbuttonpos = (currentvalue - minvalue) / (maxvalue - minvalue) - 0.5
			local button = world:CreateNewEntity()
			world:CreateComponentAndAddTo("Model", button)
			world:CreateComponentAndAddTo("Position", button)
			world:CreateComponentAndAddTo("Rotation", button)
			world:CreateComponentAndAddTo("Scale", button)
			world:CreateComponentAndAddTo("Parent", button)
			world:CreateComponentAndAddTo("KillWhenOrphan", button)
			world:GetComponent(button, "Model", 0):SetModel("sliderbutton", "quad", 3)
			world:GetComponent(button, "Position", 0):SetFloat3(sliderbuttonpos, 0, 0.01)
			world:GetComponent(button, "Rotation", 0):SetFloat3(0, 0, 0)
			world:GetComponent(button, "Scale", 0):SetFloat3(0.125, 1, 1)
			world:GetComponent(button, "Parent", 0):SetInt(entity)

			world:CreateComponentAndAddTo("MenuSliderButton", entity)
			world:GetComponent(entity, "MenuSliderButton", "ButtonId"):SetInt(button)
		end
	end
end

SliderSystem.Update = function(self, dt)
	local pressedButtons = self:GetEntities("OnPickBoxHit")
	if #pressedButtons > 0 then
		local pressedButton = pressedButtons[1]
		if Input.GetTouchState(0) == InputState.Down then
			if world:EntityHasComponent(pressedButton, "MenuSliderButton") then
				local SliderButton = world:GetComponent(pressedButton, "MenuSliderButton", "ButtonId"):GetInt(0)
				local command = world:GetComponent(pressedButton, "MenuSlider", "ConsoleCommand"):GetText()
				local stepvalue = world:GetComponent(pressedButton, "MenuSlider", "Step"):GetFloat()
				local minvalue = world:GetComponent(pressedButton, "MenuSlider", "Min"):GetFloat()
				local maxvalue = world:GetComponent(pressedButton, "MenuSlider", "Max"):GetFloat()
				local hitpos = world:GetComponent(pressedButton, "OnPickBoxHit", "X"):GetFloat()
				local value = (minvalue + (maxvalue-minvalue) * (hitpos + 0.5))/stepvalue
				local flooredvalue = math.floor(value)
				if value-flooredvalue > 0.75 then
					flooredvalue = flooredvalue + 1
				end
				local currentvalue = flooredvalue*stepvalue
				local sliderbuttonpos = (currentvalue - minvalue) / (maxvalue - minvalue) - 0.5
				
				world:GetComponent(SliderButton, "Position", "X"):SetFloat(sliderbuttonpos)
				world:GetComponent(pressedButton, "MenuSlider", "Current"):SetFloat(currentvalue)
				Console.AddToCommandQueue(command.." "..currentvalue)
			end
		end
	end
end