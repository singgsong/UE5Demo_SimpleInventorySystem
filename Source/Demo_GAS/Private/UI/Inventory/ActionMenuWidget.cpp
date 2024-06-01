#include "UI/Inventory/ActionMenuWidget.h"
#include "Components/SizeBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include <Game/MyPlayerController.h>
#include "Component/InventoryComponent.h"
#include "UI/Inventory/InventoryWidget.h"
#include "Chacacter/Demo_GASCharacter.h"
#include "UI/Inventory/SlotWidget.h"
#include "Actor/Item.h"

void UActionMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 控件在鼠标位置生成
	float LocationX = 0;
	float LocationY = 0;
	GetOwningPlayer()->GetMousePosition(LocationX, LocationY);

	FVector2D Position = FVector2D(LocationX, LocationY);
	SetPositionInViewport(Position, true);

	// 获取库存组件
	if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetOwningPlayer()))
	{
		InventoryComp = PlayerController->GetInventoryComponent();
	}

	UseButton->OnClicked.AddDynamic(this, &ThisClass::UseItem);
	DropButton->OnClicked.AddDynamic(this, &ThisClass::DropItem);
}

void UActionMenuWidget::UpdateUseButton_Text(FString String)
{
	UseButton_Text->SetText(FText::FromString(String));
}

void UActionMenuWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	RemoveFromParent(); 
}

FString UActionMenuWidget::GetUseButtonText()
{
	ItemProperty = Item.ItemID.DataTable->FindRow<FItemProperty>(Item.ItemID.RowName, "");
	if (!ItemProperty) return TEXT("");

	switch (Item.EItemType)
	{
	case EItemsType::EIT_Weapon:
		return (InventoryComp->GetPlayerCharacter()->GetWeapon()->GetStaticMesh() == ItemProperty->Mesh &&
			InventoryComp->EquippedWeapon.Index == Item.Index) ?
			TEXT("卸下") : TEXT("装备");
	case EItemsType::EIT_Shield:
		return (InventoryComp->GetPlayerCharacter()->GetShield()->GetStaticMesh() == ItemProperty->Mesh &&
			InventoryComp->EquippedShield.Index == Item.Index) ?
			TEXT("卸下") : TEXT("装备");
	case EItemsType::EIT_Food:
		return TEXT("食用");
	default:
		return TEXT("");
	}
}

void UActionMenuWidget::UseItem()
{
	switch (Item.EItemType)
	{
	case EItemsType::EIT_Weapon:
		Equip("Weapon");
		InventoryComp->EquippedWeapon = Item;
		InventoryComp->GetInventoryWidget()->UpdateItemWidgets(InventoryComp->GetInventoryItem_Ref());
		break;
	case EItemsType::EIT_Shield:
		Equip("Shield");
		InventoryComp->EquippedShield = Item;
		InventoryComp->GetInventoryWidget()->UpdateItemWidgets(InventoryComp->GetInventoryItem_Ref());
		break;
	case EItemsType::EIT_Food:
		EatFood();
		break;
	}
}

void UActionMenuWidget::EatFood()
{
	InventoryComp->UpdateHealth(ItemProperty->Power);

	// 更新库存数据
	InventoryComp->GetInventoryItem_Ref().Food[Index].Quantity = --Item.Quantity;
	// 更新库存图标
	InventoryComp->GetInventoryWidget()->UpdateItemWidgets(InventoryComp->GetInventoryItem_Ref());

	if (Item.Quantity == 0) RemoveFromParent();
}

void UActionMenuWidget::Equip(const FString& Equipment)
{
	// 装备类型映射到装备组件
	TMap<FString, UStaticMeshComponent*> EquipmentMap =
	{
		{ "Weapon", InventoryComp->GetPlayerCharacter()->GetWeapon() },
		{ "Shield", InventoryComp->GetPlayerCharacter()->GetShield() }
	};

	// 查找装备类型对应的装备组件
	UStaticMeshComponent* MeshComp = EquipmentMap.FindRef(Equipment);

	UStaticMesh* CharacterEquippedMesh = MeshComp->GetStaticMesh();
	UStaticMesh * SlotMesh = ItemProperty->Mesh;

	// 角色穿着的装备和插槽里的装备不一样
	if (CharacterEquippedMesh != SlotMesh)
	{
		MeshComp->SetStaticMesh(ItemProperty->Mesh); // 穿上插槽里的装备
	}
	// 角色穿着的装备和插槽里的装备一样
	else
	{
		// 是同一种装备，但不是同一个
		switch (Item.EItemType)
		{
		case EItemsType::EIT_Weapon:
			if (Item.Index != InventoryComp->EquippedWeapon.Index)
			{
				MeshComp->SetStaticMesh(ItemProperty->Mesh); // 穿上插槽里的装备
			}
			else
			{
				MeshComp->SetStaticMesh(nullptr);
			}
				break;
		case EItemsType::EIT_Shield:
			if (Item.Index != InventoryComp->EquippedShield.Index)
			{
				MeshComp->SetStaticMesh(ItemProperty->Mesh); // 穿上插槽里的装备
			}
			else
			{
				MeshComp->SetStaticMesh(nullptr);
			}
			break;
		default:
			break;
		}
		//InventoryComp->GetInventoryWidget()->UpdateItemWidgets(InventoryComp->GetInventoryItem_Ref());
	}
	RemoveFromParent();

}

void UActionMenuWidget::DropItem()
{
	if (!ItemActorClass) return;
	FTransform DropLocation = GetOwningPlayerPawn()->GetActorTransform() + FTransform(FRotator::ZeroRotator, FVector(30.f, 0.f, 0.f), FVector::ZeroVector);
	AItem* Actor = GetWorld()->SpawnActorDeferred<AItem>(ItemActorClass, DropLocation);
	Actor->SetItemSlot(Item);
	Actor->FinishSpawning(DropLocation);

	switch (Item.EItemType)
	{
	case EItemsType::EIT_Food:
		//InventoryComp->GetInventoryItem_Ref().Food.RemoveAtSwap(Index); // 这会把整个插槽"丢"掉的
		InventoryComp->GetInventoryItem_Ref().Food[Index].Quantity--;
		break;
	case EItemsType::EIT_Weapon:
		if (InventoryComp->GetPlayerCharacter()->GetWeaponMesh() == ItemProperty->Mesh)
		{
			InventoryComp->GetPlayerCharacter()->SetWeaponMesh(nullptr);
		}
		InventoryComp->GetInventoryItem_Ref().Weapon[Index].Quantity--;
		break;
	case EItemsType::EIT_Shield:
		if (InventoryComp->GetPlayerCharacter()->GetShieldMesh() == ItemProperty->Mesh)
		{
			InventoryComp->GetPlayerCharacter()->SetShieldMesh(nullptr);
		}
		InventoryComp->GetInventoryItem_Ref().Shield[Index].Quantity--;
		break;
	default:
		break;
	}
	InventoryComp->UpdateInventoryWidget();

	RemoveFromParent();
}
