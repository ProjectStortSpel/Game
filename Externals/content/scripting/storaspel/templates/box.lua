local Box = EntityTemplate()
Box.Name = "Box"
Box:SetFloat3("Position", 0.0, -1.0, 0.0)
Box:SetFloat3("Rotation", 0.0, 0.0, 0.0)
Box:SetFloat3("Scale", 1.0, 1.0, 1.0)
Box:SetModel("Model", "cube", "cube")
EntityTemplateManager:AddTemplate(Box)