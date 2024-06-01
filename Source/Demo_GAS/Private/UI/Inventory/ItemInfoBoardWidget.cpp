#include "UI/Inventory/ItemInfoBoardWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include <UI/Inventory/InventoryDragDropOperation.h>

void UItemInfoBoardWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	//UpdateItemInfo();

}

void UItemInfoBoardWidget::UpdateItemInfo()
{
	if (!Item.ItemID.DataTable || Item.ItemID.RowName.IsNone()) return;
	FItemProperty* ItemProperty = Item.ItemID.DataTable->FindRow<FItemProperty>(Item.ItemID.RowName, "");

	ItemName->SetText(FText::FromString(FString::Printf(TEXT("%s"), *ItemProperty->Name.ToString())));
	ItemPower->SetText(FText::FromString(FString::Printf(TEXT("%.f"), ItemProperty->Power)));
	ItemDescription->SetText(ItemProperty->Description);
	switch (Item.EItemType)
	{
	case EItemsType::EIT_Weapon:
		ItemType->SetBrushFromTexture(Weapon);
		break;
	case EItemsType::EIT_Shield:
		ItemType->SetBrushFromTexture(Armor);
		break;
	}
}

