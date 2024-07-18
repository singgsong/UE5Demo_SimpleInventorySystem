#include "UI/Inventory/InventoryWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"
#include "Components/WrapBox.h"
#include "UI/Inventory/SlotWidget.h"
#include "UI/Inventory/ItemInfoBoardWidget.h"
#include "Component/InventoryComponent.h"
#include "Chacacter/Demo_GASCharacter.h"
#include "Actor/Item.h"
#include "UI/Inventory/InventoryDragDropOperation.h"

void UInventoryWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	SetIsFocusable(true);
	//SetFocus();
}

// 关掉背包界面
FReply UInventoryWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	Super::NativeOnKeyDown(InGeometry, InKeyEvent);
	if (InKeyEvent.GetKey() == FKey(EKeys::I))
	{
		RemoveFromParent();
		APlayerController* Controller = GetOwningPlayer();
		Controller->bShowMouseCursor = false;
		Controller->SetInputMode(FInputModeGameOnly());
	}

	return FReply::Handled();
}

bool UInventoryWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	//bReadyToDragOneItem = false;
	if (!SlotWidget) return false;
	if (UInventoryDragDropOperation* Dragging = Cast<UInventoryDragDropOperation>(InOperation))
	{
		// 在插槽外释放拖拽的物品，物品回到原位
		switch (Dragging->Item.EItemType)
		{
		case EItemsType::EIT_Food:
			InventoryComp->GetInventoryItem_Ref().Food[Dragging->Index] = Dragging->Item;
			InventoryComp->UpdateInventoryWidget();
			break;
		case EItemsType::EIT_Weapon:
			InventoryComp->GetInventoryItem_Ref().Weapon[Dragging->Index] = Dragging->Item;
			InventoryComp->UpdateInventoryWidget();
			break;
		case EItemsType::EIT_Shield:
			InventoryComp->GetInventoryItem_Ref().Shield[Dragging->Index] = Dragging->Item;
			InventoryComp->UpdateInventoryWidget();
			break;
		default:
			break;
		}
	}

	return false;
}

void UInventoryWidget::UpdateMoney(int32 Moneys)
{
	int32 Value = UKismetMathLibrary::Clamp(Moneys, 0, 999999999);
	MoneyAmount->SetText(FText::AsNumber(Value));
}

void UInventoryWidget::SelectWidget(int32 Selection, const FLinearColor& Normal, const FLinearColor& NotSelected)
{
	switch (Selection)
	{
	case 0:
		WidgetSwitcher->SetActiveWidgetIndex(Selection);
		ArmorImage->SetColorAndOpacity(NotSelected);
		FoodImage->SetColorAndOpacity(NotSelected);
		SwordImage->SetColorAndOpacity(Normal);
		break;
	case 1:
		WidgetSwitcher->SetActiveWidgetIndex(Selection);
		SwordImage->SetColorAndOpacity(NotSelected);
		FoodImage->SetColorAndOpacity(NotSelected);
		ArmorImage->SetColorAndOpacity(Normal);
		break;
	case 2:
		WidgetSwitcher->SetActiveWidgetIndex(Selection);
		SwordImage->	SetColorAndOpacity(NotSelected);
		ArmorImage->	SetColorAndOpacity(NotSelected);
		FoodImage->	SetColorAndOpacity(Normal);
		break;
	default:
		break;
	}
}

void UInventoryWidget::SwitchControlColor(int32 Selection, UImage* ImageWidget, const FLinearColor& Selected, const FLinearColor& NotSelected)
{
	if (WidgetSwitcher->GetActiveWidgetIndex() == Selection)
	{
		ImageWidget->SetColorAndOpacity(Selected);
	}
	else
	{
		ImageWidget->SetColorAndOpacity(NotSelected);
	}
}

void UInventoryWidget::UpdateItemWidgets(FInventoryItem& InventoryItem)
{
	WeaponWidgets->ClearChildren();
	ArmorWidgets->ClearChildren();
	FoodWidgets->ClearChildren();

	if (!SlotWidgetClass) return;
	AddItem(InventoryItem.Weapon, WeaponWidgets);
	AddItem(InventoryItem.Shield, ArmorWidgets);
	AddItem(InventoryItem.Food, FoodWidgets);
}

void UInventoryWidget::AddItem(TArray<FItemSlot>& ItemSlot, UWrapBox* WrapBox)
{
	int32 Index = -1;
	for (FItemSlot& Item_Index : ItemSlot)
	{
		Index++;

		// 创建插槽
		SlotWidget = CreateWidget<USlotWidget>(this, SlotWidgetClass);

		// 设置插槽参数
		Item_Index.Index = Index;
		SlotWidget->SetItem(Item_Index);
		SlotWidget->SetIndex(Index);

		ItemInSlotHasBeEquippedOrNot(Item_Index, Index);

		// 添加插槽
		WrapBox->AddChild(SlotWidget);
	}
}

void UInventoryWidget::ItemInSlotHasBeEquippedOrNot(FItemSlot& Item_Index, const int32& Index)
{
	bool bSameItem = false;

	if (Item_Index.ItemID.RowName != FName("None"))
	{
		// 角色装备着武器或盾牌
		if (InventoryComp->GetPlayerCharacter()->GetWeaponMesh())
		{
			// 插槽里的是武器还是盾牌？
			if (Item_Index.EItemType == EItemsType::EIT_Weapon)
			{
				bSameItem = InventoryComp->EquippedWeapon.ItemID.RowName == Item_Index.ItemID.RowName;
				if (bSameItem && Index == InventoryComp->EquippedWeapon.Index)
				{
					SlotWidget->SetEquipped(true);
				}
				else
				{
					SlotWidget->SetEquipped(false);
				}
			}

		}
		if (InventoryComp->GetPlayerCharacter()->GetShieldMesh())
		{
			if (Item_Index.EItemType == EItemsType::EIT_Shield)
			{
				bSameItem = InventoryComp->EquippedShield.ItemID.RowName == Item_Index.ItemID.RowName;
				if (bSameItem && Index == InventoryComp->EquippedShield.Index)
				{
					SlotWidget->SetEquipped(true);
				}
				else
				{
					SlotWidget->SetEquipped(false);
				}
			}
		}
	}
}
