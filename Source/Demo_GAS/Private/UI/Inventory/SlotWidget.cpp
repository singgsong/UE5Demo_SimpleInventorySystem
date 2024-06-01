#include "UI/Inventory/SlotWidget.h"
#include "Components/SizeBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include <Game/MyPlayerController.h>
#include <UI/Inventory/ActionMenuWidget.h>
#include "Component/InventoryComponent.h"
#include "Chacacter/Demo_GASCharacter.h"
#include "UI/Inventory/InventoryWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UI/Inventory/DraggingWidget.h"
#include "UI/Inventory/InventoryDragDropOperation.h"
#include <EnhancedInputComponent.h>
#include "Game/MyPlayerController.h"
#include "UI/Inventory/ItemInfoBoardWidget.h"

void USlotWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	// 更新图标
	if (!Item.Quantity)
	{
		SetIconVisibility(false);
	}
	else
	{
		SetIconVisibility(true);

		QuantityText->SetText(FText::AsNumber(Item.Quantity));
		if (!Item.ItemID.DataTable || Item.ItemID.RowName.IsNone()) return;
		ItemProperty = Item.ItemID.DataTable->FindRow<FItemProperty>(Item.ItemID.RowName, "");
		ItemImage->SetBrushFromTexture(ItemProperty->Thumbnail);
	}

	SetEquippedImageVisibility(bEquipped);

	if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetOwningPlayer()))
	{
		InventoryComp = PlayerController->GetInventoryComponent();
	}

	ItemInfoBoardWidget = InventoryComp->GetInventoryWidget()->GetItemInfoBroadWidget();
	//ItemInfoBoardWidget->SetVisibility(ESlateVisibility::Hidden);
}

void USlotWidget::SetEquippedImageVisibility(bool bVisibility)
{ 
	bVisibility ? EquippedImage->SetVisibility(ESlateVisibility::Visible) : EquippedImage->SetVisibility(ESlateVisibility::Hidden);
}

void USlotWidget::SetIconVisibility(bool InbVisibility)
{
	ESlateVisibility State = InbVisibility ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	QuantityBox->SetVisibility(State);
	ItemImage->SetVisibility(State);
}

FReply USlotWidget::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);

	// 鼠标右键打开插槽操作菜单
	if (InMouseEvent.IsMouseButtonDown(FKey(EKeys::RightMouseButton)) && Item.Quantity)
	{
		// 关掉之前打开的动作菜单
		if (ActionMenuWidget) 
			ActionMenuWidget->RemoveFromParent();

		// 打开动作菜单
		ActionMenuWidget = CreateWidget<UActionMenuWidget>(this, ActionMenuWidgetClass);
		ActionMenuWidget->SetIndex(Index);
		ActionMenuWidget->SetItem(Item);
		ActionMenuWidget->SetSlotWidget(this);
		ActionMenuWidget->AddToViewport();

		// 更新按钮状态
		if (Item.ItemID.DataTable && !Item.ItemID.RowName.IsNone())
		{
			UseButtonText = ActionMenuWidget->GetUseButtonText();
			ActionMenuWidget->UpdateUseButton_Text(UseButtonText);
		}
	}
	else if (bReadyToDragOneItem && InMouseEvent.IsMouseButtonDown(FKey(EKeys::LeftMouseButton)))
	{
		bDragingOneItem = true;
		return DetectDrag(InMouseEvent, this, FKey(EKeys::LeftMouseButton));
	}
	// 鼠标左键拿起插槽里的全部物品
	else if (InMouseEvent.IsMouseButtonDown(FKey(EKeys::LeftMouseButton)))
	{
		return DetectDrag(InMouseEvent, this, FKey(EKeys::LeftMouseButton));
		// NativeOnDragDetected被自动调用
	}

	return FReply::Handled();
}

FReply USlotWidget::DetectDrag(const FPointerEvent& PointerEvent, UWidget* WidgetDetectingDrag, FKey DragKey)
{
	// 对着WidgetBlueprintLibrary里的DetectDrag照猫画虎抄的。
	if (PointerEvent.GetEffectingButton() == DragKey || PointerEvent.IsTouchEvent())
	{
		FReply Reply = FReply::Handled();
		if (WidgetDetectingDrag)
		{
			TSharedPtr<SWidget> SlateWidgetDetectingDrag = WidgetDetectingDrag->GetCachedWidget();
			if (SlateWidgetDetectingDrag.IsValid())
			{
				return Reply.DetectDrag(SlateWidgetDetectingDrag.ToSharedRef(), DragKey);
				//Reply.NativeReply = Reply.NativeReply.DetectDrag(SlateWidgetDetectingDrag.ToSharedRef(), DragKey);
			}
		}
	}

	return FReply::Unhandled();
}

void USlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	if (!DraggingWidgetClass || !DragDropOperationClass || !Item.Quantity) return;
	if (!bDragingOneItem)
	{
		switch (Item.EItemType)
		{
		case EItemsType::EIT_Food:
			InventoryComp->GetInventoryItem_Ref().Food[Index].Quantity = 0;
			InventoryComp->UpdateInventoryWidget();
			break;

		case EItemsType::EIT_Weapon:
			InventoryComp->GetInventoryItem_Ref().Weapon[Index].Quantity = 0;
			InventoryComp->UpdateInventoryWidget();
			break;
		case EItemsType::EIT_Shield:
			InventoryComp->GetInventoryItem_Ref().Shield[Index].Quantity = 0;
			InventoryComp->UpdateInventoryWidget();
			break;
		default:
			break;
		}
		// 创建拖拽时显示的控件
		DraggingWidget = CreateWidget<UDraggingWidget>(this, DraggingWidgetClass);
		DraggingWidget->SetItem(Item);

		TObjectPtr<UInventoryDragDropOperation> DragDropOperation;
		DragDropOperation = CreateDragDropOperation(DragDropOperationClass);
		DragDropOperation->DefaultDragVisual = DraggingWidget; // 设置拖拽时显示的控件
		DragDropOperation->Item = Item;
		DragDropOperation->Index = Index;
		OutOperation = DragDropOperation;
	}
	else
	{
		switch (Item.EItemType)
		{
		case EItemsType::EIT_Food:
			InventoryComp->GetInventoryItem_Ref().Food[Index].Quantity--;
			InventoryComp->UpdateInventoryWidget();
			break;

		case EItemsType::EIT_Weapon:
			InventoryComp->GetInventoryItem_Ref().Weapon[Index].Quantity = 0;
			InventoryComp->UpdateInventoryWidget();
			break;
		case EItemsType::EIT_Shield:
			InventoryComp->GetInventoryItem_Ref().Shield[Index].Quantity = 0;
			InventoryComp->UpdateInventoryWidget();
			break;
		default:
			break;
		}
		// 创建拖拽时显示的控件
		DraggingWidget = CreateWidget<UDraggingWidget>(this, DraggingWidgetClass);
		DraggingWidget->SetItem(Item);

		TObjectPtr<UInventoryDragDropOperation> DragDropOperation;
		DragDropOperation = CreateDragDropOperation(DragDropOperationClass);
		DragDropOperation->DefaultDragVisual = DraggingWidget; // 设置拖拽时显示的控件
		DragDropOperation->Item = Item;
		DragDropOperation->Item.Quantity = 1; // 拿起一个
		DragDropOperation->Index = Index;
		OutOperation = DragDropOperation;
	}
}

UInventoryDragDropOperation* USlotWidget::CreateDragDropOperation(TSubclassOf<UInventoryDragDropOperation> Operation)
{
	// 对着WidgetBlueprintLibrary里的CreateDragDropOperation照猫画虎抄的。
	UInventoryDragDropOperation* DragOperation = nullptr;
	if (Operation)
	{
		DragOperation = NewObject<UInventoryDragDropOperation>(GetTransientPackage(), Operation);
	}
	else
	{
		DragOperation = NewObject<UInventoryDragDropOperation>();
	}
	return DragOperation;
}

bool USlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (UInventoryDragDropOperation* Dragging = Cast<UInventoryDragDropOperation>(InOperation))
	{
		// 因为插槽显不显示物品是以物品数量(Item.Quantity)来判断的，所以得先判断Item.Quantity是否大于零
		if (Item.Quantity)
		{
			bool bSameItem = Dragging->Item.ItemID.RowName == Item.ItemID.RowName;

			switch (Dragging->Item.EItemType)
			{
			case EItemsType::EIT_Food:
				HandleFoodDrop(bSameItem, Dragging);
				break;
			case EItemsType::EIT_Weapon:
				HandleEquipmentDrop(bSameItem, InventoryComp->GetInventoryItem_Ref().Weapon, Dragging);
				break;
			case EItemsType::EIT_Shield:
				HandleEquipmentDrop(bSameItem, InventoryComp->GetInventoryItem_Ref().Shield, Dragging);
				break;
			default:
				break;
			}
		}
		// 移到空插槽
		else if (!Item.Quantity)
		{
			DropToEmptySlot(Dragging);
		}

		bDragingOneItem = false;
		InventoryComp->UpdateInventoryWidget();
	}
	return true; // 返回ture表示拖放事件已经被处理，父级控件的NativeOnDrop将不会被调用！
}

FReply USlotWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	if (InKeyEvent.GetKey() == FKey(EKeys::LeftControl))
	{
		bReadyToDragOneItem = true;
		FTimerHandle DragOneItemTimer;
		FTimerDelegate Delegate;
		Delegate.BindLambda([this]()
			{
				bReadyToDragOneItem = false;
			});
		GetWorld()->GetTimerManager().SetTimer(DragOneItemTimer, Delegate, 1.f, false);

	}
	return FReply::Handled();
}

void USlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	SetKeyboardFocus();

}

void USlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	InventoryComp->GetInventoryWidget()->SetKeyboardFocus();
}

void USlotWidget::HandleFoodDrop(bool bSameItem, UInventoryDragDropOperation* Dragging)
{
	// 一样的食物？
	if (bSameItem)
	{
		// 该种食物还能堆叠？
		if (Item.Quantity < ItemProperty->StackSize)
		{
			// 拿起的食物 和 插槽里的食物 数量总和
			int32 QuantityAmount = Item.Quantity + Dragging->Item.Quantity;
			// 数量总和大于堆叠数？-》放入最大堆叠数，剩余的放回原位
			if (QuantityAmount > ItemProperty->StackSize)
			{
				Dragging->Item.Quantity = QuantityAmount - ItemProperty->StackSize;
				InventoryComp->GetInventoryItem_Ref().Food[Index].Quantity = ItemProperty->StackSize;
				InventoryComp->GetInventoryItem_Ref().Food[Dragging->Index] = Dragging->Item;
			}
			// 小于堆叠数？-》一并放入插槽
			else
			{
				InventoryComp->GetInventoryItem_Ref().Food[Index].Quantity = QuantityAmount;
			}
		}
		// 插槽满了
		else if (Item.Quantity == ItemProperty->StackSize)
		{
			InventoryComp->GetInventoryItem_Ref().Food[Dragging->Index] = Dragging->Item;
		}
	}
	else // 交换两个插槽的物品
	{
		InventoryComp->GetInventoryItem_Ref().Food[Index] = Dragging->Item;
		InventoryComp->GetInventoryItem_Ref().Food[Dragging->Index] = Item;
	}
}

void USlotWidget::HandleEquipmentDrop(bool bSameItem, TArray<FItemSlot>& EuqipmentArr, UInventoryDragDropOperation* Dragging)
{
	// 一样的物品，放回原位
	if (bSameItem)
	{
		EuqipmentArr[Dragging->Index] = Dragging->Item;
	}
	// 不一样的物品，替换位置
	else
	{
		EuqipmentArr[Index] = Dragging->Item;
		EuqipmentArr[Dragging->Index] = Item;
	}
}

void USlotWidget::DropToEmptySlot(UInventoryDragDropOperation* Dragging)
{
	switch (Dragging->Item.EItemType)
	{
	case EItemsType::EIT_Food:
		InventoryComp->GetInventoryItem_Ref().Food[Index] = Dragging->Item;
		break;
	case EItemsType::EIT_Weapon:
		InventoryComp->GetInventoryItem_Ref().Weapon[Index] = Dragging->Item;
		break;
	case EItemsType::EIT_Shield:
		InventoryComp->GetInventoryItem_Ref().Shield[Index] = Dragging->Item;
		break;
	default:
		break;
	}
}
