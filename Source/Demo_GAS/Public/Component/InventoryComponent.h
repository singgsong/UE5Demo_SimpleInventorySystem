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


#pragma region 显示库存界面
	// 输入行为
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

#pragma endregion

	UPROPERTY()
	TObjectPtr<ADemo_GASCharacter> PlayerCharacter;

#pragma region 血条
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Class)
	TSubclassOf<UHealthBarWidget> HealthBarWidgetClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UHealthBarWidget> HealthBarWidget;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 10.f;

	UPROPERTY(EditDefaultsOnly)
	float CurrentHealth = MaxHealth;

#pragma endregion
	
	UPROPERTY(VisibleAnywhere)
	int32 MoneyAmount = 0;

	UPROPERTY(EditAnywhere)
	FInventoryItem InventoryItem;

	UPROPERTY(EditAnywhere)
	int32 MaxWeaponSlotQuantities = 10;

	UPROPERTY(EditAnywhere)
	int32 MaxShieldSlotQuantities = 10;

	UPROPERTY(EditAnywhere)
	int32 MaxFoodSlotQuantities = 10;
public:

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UItemInfoBoardWidget> ItemInfoBoardWidget;

	FItemSlot EquippedWeapon;
	FItemSlot EquippedShield;

	UInventoryWidget* GetInventoryWidget() const { return InventoryWidget; }
	UHealthBarWidget* GetHealthBarWidget() const { return HealthBarWidget; }
	float GetCurrentHealth() const { return CurrentHealth; }
	ADemo_GASCharacter* GetPlayerCharacter() const { return PlayerCharacter; }

	FInventoryItem GetInventoryItem() const { return InventoryItem; }
	FInventoryItem& GetInventoryItem_Ref() { return InventoryItem; }

	void SetMoneyAmount(int32 Value) { MoneyAmount = Value; }
	void SetCurrentHealth(float Value) { CurrentHealth = Value; }
	void SetInventoryItem(const FInventoryItem InInventoryItem) { InventoryItem = InInventoryItem; }
};
