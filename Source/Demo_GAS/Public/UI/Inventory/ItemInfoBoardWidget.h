#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <StructData.h>
#include "ItemInfoBoardWidget.generated.h"

class UTextBlock;
class UImage;

UCLASS()
class DEMO_GAS_API UItemInfoBoardWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativePreConstruct() override;

	UFUNCTION(BlueprintCallable)
	void UpdateItemInfo();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemSlot Item;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemType;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemPower;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemDescription;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> Weapon;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> Armor;

public:

};
