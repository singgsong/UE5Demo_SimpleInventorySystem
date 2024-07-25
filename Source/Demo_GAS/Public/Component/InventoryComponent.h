#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StructData.h"
#include "InventoryComponent.generated.h"

class UInputAction;
class UInventoryWidget;
class UHealthBarWidget;
class UInteractWidget;
class UItemInfoBoardWidget;
class ADemo_GASCharacter;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DEMO_GAS_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void InitializeInteractInput(APlayerController* InPlayerController);

	void UpdateHealth(float Value);

	void UpdateItemData(TArray<FItemSlot>& InventoryItemSlots, FItemSlot& Item_PickedUp, AActor* Item);

	FORCEINLINE void UpdateInventoryWidget();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void PickUpItem(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void UpdateMoneyAmount(int32 Value);

	void OpenInventory();

	bool TraceItemToPickUp(FItemSlot& InItemInfo, AActor*& InItemActor);

	void PickUpItemToInventory();

public:
	FItemSlot EquippedWeapon;
	FItemSlot EquippedShield;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UItemInfoBoardWidget> ItemInfoBoardWidget;

protected:
	UPROPERTY(EditAnywhere)
	FInventoryItem InventoryItem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> InventoryAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> InteractAction;

	UPROPERTY()
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(EditDefaultsOnly, Category = Class)
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;

	UPROPERTY(BlueprintReadOnly) // 不暴露给蓝图好像会被垃圾回收系统自动清空
	TObjectPtr<UInventoryWidget> InventoryWidget;

	UPROPERTY(EditDefaultsOnly, Category = Class)
	TSubclassOf<UInteractWidget> InteractWidgetClass;

	UPROPERTY()
	TObjectPtr<UInteractWidget> InteractWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Class)
	TSubclassOf<UItemInfoBoardWidget> ItemInfoBoardWidgetClass;

	UPROPERTY()
	TObjectPtr<ADemo_GASCharacter> PlayerCharacter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Class)
	TSubclassOf<UHealthBarWidget> HealthBarWidgetClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UHealthBarWidget> HealthBarWidget;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 10.f;

	UPROPERTY(EditDefaultsOnly)
	float CurrentHealth = MaxHealth;

	UPROPERTY(VisibleAnywhere)
	int32 MoneyAmount = 0;

	UPROPERTY(EditAnywhere)
	int32 MaxWeaponSlotQuantities = 10;

	UPROPERTY(EditAnywhere)
	int32 MaxShieldSlotQuantities = 10;

	UPROPERTY(EditAnywhere)
	int32 MaxFoodSlotQuantities = 10;

public:
	FORCEINLINE UInventoryWidget* GetInventoryWidget() const { return InventoryWidget; }
	FORCEINLINE UHealthBarWidget* GetHealthBarWidget() const { return HealthBarWidget; }
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }
	FORCEINLINE ADemo_GASCharacter* GetPlayerCharacter() const { return PlayerCharacter; }

	FORCEINLINE FInventoryItem GetInventoryItem() const { return InventoryItem; }
	FORCEINLINE FInventoryItem& GetInventoryItem_Ref() { return InventoryItem; }

	FORCEINLINE void SetMoneyAmount(int32 Value) { MoneyAmount = Value; }
	FORCEINLINE void SetCurrentHealth(float Value) { CurrentHealth = Value; }
	FORCEINLINE void SetInventoryItem(const FInventoryItem InInventoryItem) { InventoryItem = InInventoryItem; }
};
