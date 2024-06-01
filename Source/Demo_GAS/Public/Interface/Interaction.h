#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnumFile.h"
#include "Interaction.generated.h"

class UPhysicsHandleComponent;
class ADemo_GASCharacter;

UINTERFACE(MinimalAPI)
class UInteraction : public UInterface
{
	GENERATED_BODY()
};

class DEMO_GAS_API IInteraction
{
	GENERATED_BODY()

public:
	virtual void PickAndDrop(USceneComponent* UnVisibleHand) { }
	virtual void DropItem() { }
	virtual EItemType GetItemType() const { return EItemType::EIT_PhysicsObject; }
	virtual void Equip(ADemo_GASCharacter* Pawn, USceneComponent* Hand) { }
};
