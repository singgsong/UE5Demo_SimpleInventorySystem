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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Health = 0.f;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FDataTableRowHandle MoneysDataTable;

public:
	FORCEINLINE const FDataTableRowHandle& GetMoneysDataTable() const { return MoneysDataTable; }
};
