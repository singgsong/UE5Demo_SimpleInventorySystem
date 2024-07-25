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
	TObjectPtr<UStaticMesh> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Amount = 0;
};

USTRUCT(BlueprintType)
struct FItemProperty : public FTableRowBase
{// (12 + 4) + 24 + 8 + 8 + (4 + 4) +(4 + 4) = 72，
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Description = FText(); // 24 byte，对齐要求：8 byte

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Thumbnail;  // 8 byte，对齐要求：8 byte

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMesh> Mesh; // 8 byte，对齐要求：8 byte

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName Name = FName(); // 12 byte，对齐要求：4 byte

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 StackSize = 0; // 4 byte，对齐要求：4 byte

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Power = 0.f; // 4 byte，对齐要求：4 byte
};

USTRUCT(BlueprintType)
struct FItemSlot
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
struct FInventoryItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FItemSlot> Weapon;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FItemSlot> Shield;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FItemSlot> Food;

};

