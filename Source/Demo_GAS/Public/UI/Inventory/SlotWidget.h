#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StructData.h"
#include "SlotWidget.generated.h"

class USizeBox;
class UImage;
class UTextBlock;
class UButton;
class UItemInfoBoardWidget;
class UActionMenuWidget;
class UInventoryComponent;
class UDraggingWidget;
class UInventoryDragDropOperation;

/**
 * 
 */
UCLASS()
class DEMO_GAS_API USlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativePreConstruct() override;

	UInventoryComponent* InventoryComp;

	void SetEquippedImageVisibility(bool bVisibility);

	FReply DetectDrag(const FPointerEvent& PointerEvent, UWidget* WidgetDetectingDrag, FKey DragKey);

protected:
	void SetIconVisibility(bool InbVisibility);

	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	void HandleFoodDrop(bool bSameItem, UInventoryDragDropOperation* Dragging);

	void HandleEquipmentDrop(bool bSameItem, TArray<FItemSlot>& EuqipmentArr, UInventoryDragDropOperation* Dragging);

	void DropToEmptySlot(UInventoryDragDropOperation* Dragging);

	UInventoryDragDropOperation* CreateDragDropOperation(TSubclassOf<UInventoryDragDropOperation> Operation);

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	TObjectPtr<USizeBox> QuantityBox;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	TObjectPtr<UTextBlock> QuantityText;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	TObjectPtr<UImage> ItemImage;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	TObjectPtr<UImage> SelectedImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> EquippedImage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FItemSlot Item;

	int32 Index = 0;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UItemInfoBoardWidget> ItemInfoBoardWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UActionMenuWidget> ActionMenuWidgetClass;

	UPROPERTY()
	TObjectPtr<UActionMenuWidget> ActionMenuWidget;

	FString UseButtonText;
	FItemProperty* ItemProperty;
	bool bEquipped = false;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UDraggingWidget> DraggingWidgetClass;
	TObjectPtr<UDraggingWidget> DraggingWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UInventoryDragDropOperation> DragDropOperationClass;

public:
	bool bReadyToDragOneItem = false;
	bool bDragingOneItem = false;

	void SetItem(const FItemSlot& InItem) { Item = InItem; }
	void SetIndex(int32 InItem) { Index = InItem; }
	void SetEquipped(bool Value) { bEquipped = Value; }
};
