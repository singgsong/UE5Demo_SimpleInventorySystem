#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HeartWidget.generated.h"

UENUM()
enum class EHeartStyle : uint8
{
	EHS_Full,
	EHS_Half,
	EHS_Empty
};

UCLASS()
class DEMO_GAS_API UHeartWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void ChangeImage(const EHeartStyle& Syle);

	UPROPERTY(meta = (BindeWidget))
	TObjectPtr<UImage> Heart;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> FullHeart;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> HalfHeart;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> EmptyHeart;

public:


};
