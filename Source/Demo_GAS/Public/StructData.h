#pragma once

#include "CoreMinimal.h"
#include "EnumFile.h"
#include "StructData.generated.h"

USTRUCT(BlueprintType)
struct FMoney :public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName Name = FName();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Amount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMesh> Mesh;
};

USTRUCT(BlueprintType)
struct FItemProperty : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName Name = FName();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Description = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 StackSize = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Power = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMesh> Mesh;
};

USTRUCT(BlueprintType)
struct FItemSlot : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FDataTableRowHandle ItemID;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Quantity = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EItemsType EItemType = EItemsType::EIT_Object;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Index = 0;
};

USTRUCT(BlueprintType, Blueprintable)
struct FInventoryItem : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FItemSlot> Weapon;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FItemSlot> Shield;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FItemSlot> Food;

};

