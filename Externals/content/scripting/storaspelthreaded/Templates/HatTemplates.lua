local Hat = EntityTemplate()
Hat.Name = "nerzhulHat"
Hat:SetModel("Model", "nerzhul", "nerzhul", 0)
Hat:SetFloat3("Position", 0.05, -0.06, 0.14)
Hat:SetFloat3("Rotation", 0.2, 0, 0.6)
Hat:SetFloat3("Scale", 0.25, 0.25, 0.25)
Hat:SetFloat3("Color", 1, 1, 1)
Hat:AddComponent("ParentColor") 		-- ADDS PLAYER COLOR
Hat:SetInt("ParentJoint", 5) 			-- OPTIONAL: ADDS HAT TO HEAD JOINT
Hat:AddComponent("SyncNetwork")
EntityTemplateManager:AddTemplate(Hat)

Hat = EntityTemplate()
Hat.Name = "totemHat"
Hat:SetModel("Model", "totemhat", "totemhat", 0)
Hat:SetFloat3("Position", -0.14, 0.2, 0.1)
Hat:SetFloat3("Rotation", 0.2, 0, 0.6)
Hat:SetFloat3("Scale", 1.4, 1.4, 1.4)
Hat:SetFloat3("Color", 1, 1, 1)
Hat:AddComponent("ParentColor") 		-- ADDS PLAYER COLOR
Hat:SetInt("ParentJoint", 5) 			-- OPTIONAL: ADDS HAT TO HEAD JOINT
Hat:AddComponent("SyncNetwork")
EntityTemplateManager:AddTemplate(Hat)

Hat = EntityTemplate()
Hat.Name = "eggMan"
Hat:SetModel("Model", "stone", "stone", 0)
Hat:SetFloat3("Position", 0, 0, 0)
Hat:SetFloat3("Rotation", 0, 0, -4.14)
Hat:SetFloat3("Scale", 1, 1, 1)
Hat:SetFloat3("Color", 1, 1, 1)
Hat:AddComponent("ParentColor") 		-- ADDS PLAYER COLOR
Hat:SetInt("ParentJoint", 1) 			-- OPTIONAL: ADDS HAT TO HEAD JOINT
Hat:AddComponent("SyncNetwork")
EntityTemplateManager:AddTemplate(Hat)

Hat = EntityTemplate()
Hat.Name = "stoneHat"
Hat:SetModel("Model", "smallstone", "smallstone", 0)
Hat:SetFloat3("Position", -0.14, 0.2, 0.1)
Hat:SetFloat3("Rotation", 0.2, 0, 0.6)
Hat:SetFloat3("Scale", 0.14, 0.14, 0.14)
Hat:SetFloat3("Color", 1, 1, 1)
Hat:AddComponent("ParentColor") 		-- ADDS PLAYER COLOR
Hat:SetInt("ParentJoint", 5) 			-- OPTIONAL: ADDS HAT TO HEAD JOINT
Hat:AddComponent("SyncNetwork")
EntityTemplateManager:AddTemplate(Hat)

Hat = EntityTemplate()
Hat.Name = "totemtopHat"
Hat:SetModel("Model", "totemtop", "totemtop", 0)
Hat:SetFloat3("Position", -0.04, 0.061, 0.045)
Hat:SetFloat3("Rotation", 0.2, 0, 0.6)
Hat:SetFloat3("Scale", 1.1, 1.1, 1.1)
Hat:SetInt("ParentJoint", 5) 			-- OPTIONAL: ADDS HAT TO HEAD JOINT
Hat:AddComponent("SyncNetwork")
EntityTemplateManager:AddTemplate(Hat)

Hat = EntityTemplate()
Hat.Name = "islandHat"
Hat:SetModel("Model", "miniisland", "distantisland", 0)
Hat:SetFloat3("Position", 0.0, 0.0, 0.0)
Hat:SetFloat3("Rotation", 0.2, 0, 0.6)
Hat:SetFloat3("Scale", 0.5, 0.5, 0.5)
Hat:SetFloat3("Color", 1, 1, 1)
Hat:AddComponent("ParentColor") 		-- ADDS PLAYER COLOR
Hat:SetInt("ParentJoint", 5) 			-- OPTIONAL: ADDS HAT TO HEAD JOINT
Hat:AddComponent("SyncNetwork")
EntityTemplateManager:AddTemplate(Hat)

Hat = EntityTemplate()
Hat.Name = "bushHat"
Hat:SetModel("Model", "bush", "bush", 0)
Hat:SetFloat3("Position", 0.0, 0.0, 0.0)
Hat:SetFloat3("Rotation", 0.2, 0, 0.6)
Hat:SetFloat3("Scale", 0.5, 0.5, 0.5)
Hat:SetFloat3("Color", 1, 1, 1)
Hat:AddComponent("ParentColor") 		-- ADDS PLAYER COLOR
Hat:SetInt("ParentJoint", 5) 			-- OPTIONAL: ADDS HAT TO HEAD JOINT
Hat:AddComponent("SyncNetwork")
EntityTemplateManager:AddTemplate(Hat)

Hat = EntityTemplate()
Hat.Name = "topHat"
Hat:SetModel("Model", "tophat", "tophat", 0)
Hat:SetFloat3("Position", -0.025, 0.030, 0.05)
Hat:SetFloat3("Rotation", 0.2, 0, 0.6)
Hat:SetFloat3("Scale", 1.0, 1.0, 1.0)
Hat:SetFloat3("Color", 1, 1, 1)
Hat:AddComponent("ParentColor") 		-- ADDS PLAYER COLOR
Hat:SetInt("ParentJoint", 5) 			-- OPTIONAL: ADDS HAT TO HEAD JOINT
Hat:AddComponent("SyncNetwork")
EntityTemplateManager:AddTemplate(Hat)