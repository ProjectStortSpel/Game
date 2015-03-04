local Card = EntityTemplate()
Card.Name = "Card"
Card:AddComponent("Card")
Card:AddComponent("CardPrio")
Card:AddComponent("CardAction")
EntityTemplateManager:AddTemplate(Card)


local CardAboveHead = EntityTemplate()
CardAboveHead.Name = "CardAboveHead"

	CardAboveHead:AddComponent("SyncNetwork")
	CardAboveHead:AddComponent("WorldToViewSpace")
	CardAboveHead:AddComponent("Parent")
	CardAboveHead:AddComponent("Model")
	CardAboveHead:AddComponent("CardAboveHead")
	CardAboveHead:AddComponent("Position")
	CardAboveHead:AddComponent("Rotation")
	CardAboveHead:AddComponent("Scale")
	CardAboveHead:AddComponent("Color")

EntityTemplateManager:AddTemplate(CardAboveHead)