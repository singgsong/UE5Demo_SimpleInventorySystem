#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StructData.h"
#include "ActionMenuWidget.generated.h"

class USizeBox;
class UButton;
class UTextBlock;
class UInventoryComponent;
class USlotWidget;
class AItem;

UCLASS()
class DEMO_GAS_API UActionMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;

	void UpdateUseButton_Text(FString String);

	FString GetUseButtonText();

protected:
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;


	UFUNCTION()
	void UseItem();
	void EatFood();
	void Equip(const FString& Equipment);

	UFUNCTION()
	void DropItem();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FItemSlot Item;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> ActionMenuSizeBox;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	TObjectPtr<UButton> UseButton;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	TObjectPtr<UButton> DropButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> UseButton_Text;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UInventoryComponent> InventoryComp;

	FItemProperty* ItemProperty;

	UPROPERTY()
	TObjectPtr<USlotWidget> SlotWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AItem> ItemActorClass;

	int32 Index = 0;

public:
	UButton* GetUseButton() const { return UseButton; }

	void SetItem(const FItemSlot InItem) { Item = InItem; }
	void SetIndex(const int32 InIndex) { Index = InIndex; }
	void SetSlotWidget(USlotWidget* InSlotWidget) { SlotWidget = InSlotWidget; }
};
