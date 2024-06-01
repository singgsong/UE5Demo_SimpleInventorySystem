#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StructData.h"
#include "InventoryWidget.generated.h"

class UTextBlock;
class UImage;
class UWidgetSwitcher;
class UWrapBox;
class USlotWidget;
class UItemInfoBoardWidget;
class AItem;
class UInventoryComponent;

/**
 * 
 */
UCLASS()
class DEMO_GAS_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativePreConstruct() override;


protected:
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	 
public:
	void UpdateMoney(int32 Moneys);

	UFUNCTION(BlueprintCallable)
	void SelectWidget(int32 Selection, const FLinearColor& Normal, const FLinearColor& NotSelected);

	UFUNCTION(BlueprintCallable)
	void SwitchControlColor(int32 Selection, UImage* ImageWidget, const FLinearColor& Normal, const FLinearColor& NotSelected);

	void UpdateItemWidgets(FInventoryItem& InventoryItem);

protected:
	void AddItem(TArray<FItemSlot>& ItemSlot, UWrapBox* WrapBox);

	void ItemInSlotHasBeEquippedOrNot(FItemSlot& Item_Index, const int32& Index);

	UPROPERTY(meta = (BindeWidget))
	TObjectPtr<UTextBlock> MoneyAmount;

	UPROPERTY(meta = (BindeWidget), BlueprintReadOnly, Category = "库存|分类按钮")
	TObjectPtr<UImage> SwordImage;

	UPROPERTY(meta = (BindeWidget), BlueprintReadOnly, Category = "库存|分类按钮")
	TObjectPtr<UImage> FoodImage;

	UPROPERTY(meta = (BindeWidget), BlueprintReadOnly, Category = "库存|分类按钮")
	TObjectPtr<UImage> ArmorImage;

	UPROPERTY(meta = (BindeWidget), BlueprintReadOnly, Category = "库存|库存物品")
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher;

	UPROPERTY(meta = (BindeWidget), BlueprintReadOnly, Category = "库存|库存物品")
	TObjectPtr<UWrapBox> WeaponWidgets;

	UPROPERTY(meta = (BindeWidget), BlueprintReadOnly, Category = "库存|库存物品")
	TObjectPtr<UWrapBox> ArmorWidgets;

	UPROPERTY(meta = (BindeWidget), BlueprintReadOnly, Category = "库存|库存物品")
	TObjectPtr<UWrapBox> FoodWidgets;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USlotWidget> SlotWidgetClass;

	TObjectPtr<USlotWidget> SlotWidget;

	//bool bReadyToDragOneItem = false;

	UPROPERTY(meta = (BindeWidget), BlueprintReadOnly)
	TObjectPtr<UItemInfoBoardWidget> ItemInfoBroadWidget;

	//UPROPERTY(EditDefaultsOnly)
	//TSubclassOf<AItem> ItemActorClass;

public:
	TObjectPtr<UInventoryComponent> InventoryComp;

	UItemInfoBoardWidget* GetItemInfoBroadWidget() { return ItemInfoBroadWidget; }
	
	//bool IsReadyToDragOneItem(bool bReady) { return bReady; }
};
