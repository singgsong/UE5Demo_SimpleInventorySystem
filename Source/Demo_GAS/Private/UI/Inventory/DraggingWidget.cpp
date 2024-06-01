#include "UI/Inventory/DraggingWidget.h"
#include "Components/Image.h"

void UDraggingWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (!Item.ItemID.DataTable && Item.ItemID.RowName.IsNone()) return;
	FItemProperty* Row = Item.ItemID.DataTable->FindRow<FItemProperty>(Item.ItemID.RowName, "");

	Image->SetBrushFromTexture(Row->Thumbnail);
}
