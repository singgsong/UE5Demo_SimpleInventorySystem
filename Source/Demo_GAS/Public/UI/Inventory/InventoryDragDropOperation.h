#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "StructData.h"
#include "InventoryDragDropOperation.generated.h"

UCLASS()
class DEMO_GAS_API UInventoryDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	FItemSlot Item;

	UPROPERTY(BlueprintReadOnly)
	int32 Index;

};
