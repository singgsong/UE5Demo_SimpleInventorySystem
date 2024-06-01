#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Interaction.h"
#include "EnumFile.h"
#include "ItemBase.generated.h"

class UTimelineComponent;
class ADemo_GASCharacter;
class UCurveFloat;
class UCurveVector;
class UBoxComponent;
class UWidgetComponent;
class UInteractionWidget;
//class UInteractableComponent;

UCLASS()	
class DEMO_GAS_API AItemBase : public AActor, public IInteraction
{
	GENERATED_BODY()
	
public:	
	AItemBase();

	UFUNCTION()
	void HandleComponentSleeping(UPrimitiveComponent* SleepingComponent, FName BoneName);

	virtual void PickAndDrop(USceneComponent* UnVisibleHand) override;

	virtual void DropItem() override;

	virtual void Equip(ADemo_GASCharacter* Pawn, USceneComponent* Hand) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTimelineComponent> Timeline;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCurveFloat> Curve_LerpToSlot;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCurveVector> Curve_LerpToHand;


private:
	void LayDown();

	void LerpToHand();

	UFUNCTION()
	void LerpToHandUpdateFunc();

	bool bPickedUp = false;

	UPROPERTY(EditAnywhere)
	EItemType ItemType = EItemType::EIT_PhysicsObject;

	TObjectPtr<ADemo_GASCharacter> Player;


	TObjectPtr<AActor> Interactor;

public:
	FORCEINLINE bool IsPickedUp() const { return bPickedUp; }
	FORCEINLINE virtual EItemType GetItemType() const override { return ItemType; }

#pragma region 交互提示
public:
	//virtual void Initialize() override;
	//virtual void Interaction(AActor* interactor) override {}
	//virtual void EndInteraction(AActor* interactor) override{}
	//virtual void RemoveInteraction() override{}

	//virtual void ClientPreInteraction(AActor* interactor) override{}
	//virtual void ClientBeginInteraction(AActor* interactor) override{}
	//virtual void ClientEndInteraction(AActor* interactor) override{}

	//virtual void GameLoad() override{}

	//UPROPERTY(EditDefaultsOnly)
	//TObjectPtr<UInteractableComponent> InteractableComp;

	//UPROPERTY(EditDefaultsOnly)
	//TObjectPtr<UBoxComponent> InteractionRange;

	//UPROPERTY(EditDefaultsOnly)
	//TObjectPtr<UWidgetComponent> InteractionUI;

	EInputWay InputWay = EInputWay::EIW_Click;

#pragma endregion

};
