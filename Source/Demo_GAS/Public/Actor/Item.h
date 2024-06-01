#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StructData.h"
#include "Item.generated.h"

UCLASS()
class DEMO_GAS_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();

	virtual void OnConstruction(const FTransform& Transform);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere)
	FItemSlot ItemInfo;

public:
	FItemSlot GetItemSlot() const { return ItemInfo; }
	void SetItemSlot(FItemSlot InItemInfo) { ItemInfo = InItemInfo; }
};
