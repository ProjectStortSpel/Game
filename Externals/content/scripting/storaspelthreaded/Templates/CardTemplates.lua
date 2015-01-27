local Card = EntityTemplate()
Card.Name = "Card"
Card:AddComponent("Card")
Card:AddComponent("CardPrio")
Card:AddComponent("CardAction")
EntityTemplateManager:AddTemplate(Card)