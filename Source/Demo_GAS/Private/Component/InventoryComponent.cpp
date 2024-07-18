#include "Component/InventoryComponent.h"
#include <EnhancedInputComponent.h>
#include "UI/Inventory/InventoryWidget.h"
#include "Kismet/GameplayStatics.h"
#include <Chacacter/Demo_GASCharacter.h>
#include "Components/CapsuleComponent.h"
#include <Actor/MoneyActor.h>
#include "UI/Inventory/HealthBarWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Actor/Item.h"
#include "UI/Inventory/InteractWidget.h"
#include "UI/Inventory/ItemInfoBoardWidget.h"
#include "Components/StaticMeshComponent.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetActive(false);
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// 初始化库存界面
	if (!InventoryWidgetClass || !HealthBarWidgetClass || !InteractWidgetClass || !ItemInfoBoardWidgetClass) return;
	InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetClass);// 此时是看不见 InventoryWidget 的

	InventoryWidget->UpdateMoney(0);

	// HUD显示血条
	HealthBarWidget = CreateWidget<UHealthBarWidget>(GetWorld(), HealthBarWidgetClass);
	HealthBarWidget->CreateHealthBar();
	HealthBarWidget->AddToViewport();

	InteractWidget = CreateWidget<UInteractWidget>(GetWorld(), InteractWidgetClass);

	// 最大堆叠数
	InventoryItem.Weapon.SetNum(MaxWeaponSlotQuantities);
	InventoryItem.Shield.SetNum(MaxShieldSlotQuantities);
	InventoryItem.Food.SetNum(MaxFoodSlotQuantities);
	InventoryWidget->InventoryComp = this;
	InventoryWidget->UpdateItemWidgets(InventoryItem);

	//ItemInfoBoardWidget = CreateWidget<UItemInfoBoardWidget>(GetWorld(), ItemInfoBoardWidgetClass);

}

void UInventoryComponent::InitializeInteractInput(APlayerController* InPlayerController)
{
	PlayerController = InPlayerController;
	if (PlayerController)
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, this, &ThisClass::OpenInventory);
			EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ThisClass::PickUpItemToInventory);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryComponent未初始化"));
	}

	// 绑定玩家胶囊体的碰撞事件
	if (PlayerCharacter = Cast<ADemo_GASCharacter>(PlayerController->GetPawn()))
		PlayerCharacter->GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::PickUpItem);
}

void UInventoryComponent::PickUpItem(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AMoneyActor* Money = Cast<AMoneyActor>(OtherActor))
	{
		const FMoney* Row = Money->GetMoneysDataTable().DataTable->FindRow<FMoney>(Money->GetMoneysDataTable().RowName, FString("Blue"));
		
		UpdateMoneyAmount(Row->Amount);

		UpdateHealth(Money->Health);

		Money->Destroy();
	}
}

void UInventoryComponent::UpdateMoneyAmount(int32 Value)
{
	MoneyAmount = FMath::Clamp(MoneyAmount + Value, 0, 999999999);
	InventoryWidget->UpdateMoney(MoneyAmount);
}

void UInventoryComponent::UpdateHealth(float Value)
{
	CurrentHealth = FMath::Clamp(CurrentHealth + Value, 0.f, MaxHealth);
	HealthBarWidget->UpdateHealthBarWidget(CurrentHealth);
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FItemSlot ItemInfo;
	AActor* ItemActor = nullptr;
	if (!InteractWidget) return;
	if (TraceItemToPickUp(ItemInfo, ItemActor))
	{
		InteractWidget->AddToViewport();
	}
	else
	{
		InteractWidget->RemoveFromParent();
	}
	
}

bool UInventoryComponent::TraceItemToPickUp(FItemSlot& InItemInfo, AActor*& InItemActor)
{
	// 检测物品
	FVector Start = PlayerCharacter->GetActorLocation() + FVector(0.f, 0.f, -80.f);
	FVector End = PlayerCharacter->GetActorForwardVector() * 300.f + Start;
	TArray<AActor*> IgnoreActor;
	IgnoreActor.Add(PlayerCharacter);
	FHitResult OutHit;

	UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(), 
		Start, End, 
		10.f, 
		ETraceTypeQuery::TraceTypeQuery1, 
		false, 
		IgnoreActor, 
		EDrawDebugTrace::None, 
		OutHit, 
		true);

	// 返回检测结果
	if (AItem* Item = Cast<AItem>(OutHit.GetActor()))
	{
		InItemInfo = Item->GetItemSlot();
		InItemActor = Item;
		return true;
	}
	else
	{
		return false;
	}

}

void UInventoryComponent::OpenInventory()
{
	if (!InventoryWidget) return;
	InventoryWidget->AddToViewport();

	// 在装备界面显示血条
	HealthBarWidget->RemoveFromParent();
	HealthBarWidget->AddToViewport();

	PlayerController->bShowMouseCursor = true;
	PlayerController->SetInputMode(FInputModeUIOnly());

	InventoryWidget->SetUserFocus(PlayerController);
	//InventoryWidget->SetKeyboardFocus(); // 用这个也行

	InventoryWidget->UpdateItemWidgets(InventoryItem);
}

void UInventoryComponent::PickUpItemToInventory()
{
	FItemSlot ItemPickedUp;
	AActor* ItemActor = nullptr;
	if (TraceItemToPickUp(ItemPickedUp, ItemActor))
	{
		// 判断捡起物品的类型
		switch (ItemPickedUp.EItemType)	
		{	
		case EItemsType::EIT_Weapon:
			UpdateItemData(InventoryItem.Weapon, ItemPickedUp, ItemActor);
			InventoryWidget->UpdateItemWidgets(InventoryItem);
			break;
		case EItemsType::EIT_Shield:
			UpdateItemData(InventoryItem.Shield, ItemPickedUp, ItemActor);
			InventoryWidget->UpdateItemWidgets(InventoryItem);
			break;
		case EItemsType::EIT_Food:   
			UpdateItemData(InventoryItem.Food, ItemPickedUp, ItemActor);
			break;
		}
	}
}

void UInventoryComponent::UpdateItemData(TArray<FItemSlot>& InventoryItemSlots, FItemSlot& Item_PickedUp, AActor* Item)
{
	for (auto& Item_BackPack : InventoryItemSlots)
	{
		// 一样的物品?
		if (Item_PickedUp.ItemID.RowName == Item_BackPack.ItemID.RowName)
		{
			FItemProperty* ItemItemPickedUpProperty = Item_PickedUp.ItemID.DataTable->FindRow<FItemProperty>(Item_PickedUp.ItemID.RowName, FString());
			// 可以堆叠？
			if (ItemItemPickedUpProperty->StackSize >= Item_PickedUp.Quantity + Item_BackPack.Quantity)
			{
				// 快点放进来吧~
				Item_BackPack.ItemID = Item_PickedUp.ItemID;
				Item_BackPack.EItemType = Item_PickedUp.EItemType;
				Item_BackPack.Quantity = Item_PickedUp.Quantity + Item_BackPack.Quantity;

				// 物品已放入背包，跳出循环
				Item->Destroy();
				break;
			}
		}
		// 不是一样的物品？
		else
		{
			// 不一样的物品，但是背包里还没有该物品？
			if (Item_BackPack.Quantity == 0)
			{
				// 快点放进来吧~
				Item_BackPack = Item_PickedUp;

				// 物品已放入背包，跳出循环
				Item->Destroy();
				break;
			}

			// 一样的物品，而且不能继续堆叠，那就去下一个索引看看吧~
			continue;
		}
	}
}

void UInventoryComponent::UpdateInventoryWidget()
{
	InventoryWidget->UpdateItemWidgets(InventoryItem);
}



