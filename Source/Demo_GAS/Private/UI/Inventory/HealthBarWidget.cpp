#include "UI/Inventory/HealthBarWidget.h"
#include "Components/WrapBox.h"
#include <UI/Inventory/HeartWidget.h>

void UHealthBarWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	HealthBar->SetWrapSize(470.0f); // 每排15颗心
}

void UHealthBarWidget::CreateHealthBar()
{
	HealthBar->ClearChildren();
	if (!HeartWidgetClass) return; // debug

	for (float i = 1; i <= MaxHealth; i++)
	{
		HeartWidget = CreateWidget<UHeartWidget>(GetOwningPlayer(), HeartWidgetClass);
		HeartWidget->ChangeImage(EHeartStyle::EHS_Full);
		HealthBar->AddChild(HeartWidget);
	}
}

void UHealthBarWidget::UpdateHealthBarWidget(float InCurrentHealth)
{
	// 拆成小数部分和整数部分
	float IntgralPart; 
	float FractionalPart = FMath::Modf(InCurrentHealth, &IntgralPart); 

	// 遍历整个血条
	int32 Count = 0;
	for (auto health : HealthBar->GetAllChildren())
	{
		Count++;
		HeartWidget = Cast<UHeartWidget>(health);
		if (Count <= StaticCast<int32>(IntgralPart))
		{
			HeartWidget->ChangeImage(EHeartStyle::EHS_Full);
		}
		else	// 判断小数点大小
		{
			if (!FractionalPart) // 小数点为零，血量更新完毕，后面全部变成空心
			{
				HeartWidget->ChangeImage(EHeartStyle::EHS_Empty);
			}
			else // 小数点不为零
			{
				if (FractionalPart > .5f) // 一格血
				{
					HeartWidget->ChangeImage(EHeartStyle::EHS_Full);
				}
				else if (FractionalPart == .5f) // 半格血
				{
					HeartWidget->ChangeImage(EHeartStyle::EHS_Half);
				}
				else // 不足半格，空心
				{
					HeartWidget->ChangeImage(EHeartStyle::EHS_Empty);
				}
				FractionalPart = 0; // 小数点处理完了，为了防止再次进入小数点判断
			}
		}
	}
}
