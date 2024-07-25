#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarWidget.generated.h"

class UImage;
class UWrapBox;
class UHeartWidget;

UCLASS()
class DEMO_GAS_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativePreConstruct() override;

	//UPROPERTY(EditAnywhere)
	//EHeartStyle HeartSyle = EHeartStyle::EHS_Empty;

	void CreateHealthBar();

	void UpdateHealthBarWidget(float InCurrentHealth);

protected:
	UPROPERTY(meta = (BindeWidget), BlueprintReadWrite)
	TObjectPtr<UWrapBox> HealthBar;

	UPROPERTY()
	TObjectPtr<UHeartWidget> HeartWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHeartWidget> HeartWidgetClass;

	UPROPERTY(EditAnywhere)
	float MaxHealth = 10.f;

public:
	UWrapBox* GetHealthBar() { return HealthBar; }

};
