#include "Actor/Item.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (ItemInfo.ItemID.RowName.IsNone()) return;
	FName RowName = ItemInfo.ItemID.RowName;
	FItemProperty* ItemProperty = ItemInfo.ItemID.DataTable->FindRow<FItemProperty>(RowName, FString());
	Mesh->SetStaticMesh(ItemProperty->Mesh);

}

