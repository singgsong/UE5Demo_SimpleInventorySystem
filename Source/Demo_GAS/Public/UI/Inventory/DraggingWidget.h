#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StructData.h"
#include "DraggingWidget.generated.h"

class UImage;

UCLASS()
class DEMO_GAS_API UDraggingWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativePreConstruct() override;

protected:
	FItemSlot Item;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image;

public:
	void SetItem(FItemSlot InItem) { Item = InItem; }
};
