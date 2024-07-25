#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StructData.h"
#include "MoneyActor.generated.h"

UCLASS()
class DEMO_GAS_API AMoneyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AMoneyActor();
	//virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FDataTableRowHandle MoneysDataTable;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> Mesh;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Health = 0.f;

	FORCEINLINE const FDataTableRowHandle& GetMoneysDataTable() const { return MoneysDataTable; }
};
