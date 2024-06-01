#include "UI/Inventory/HealthBarWidget.h"
#include "Components/WrapBox.h"
#include <UI/Inventory/HeartWidget.h>

void UHealthBarWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	HealthBar->SetWrapSize(470.0f); // ÿ��15����
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
	// ���С�����ֺ���������
	float IntgralPart; 
	float FractionalPart = FMath::Modf(InCurrentHealth, &IntgralPart); 

	// ��������Ѫ��
	int32 Count = 0;
	for (auto health : HealthBar->GetAllChildren())
	{
		Count++;
		HeartWidget = Cast<UHeartWidget>(health);
		if (Count <= StaticCast<int32>(IntgralPart))
		{
			HeartWidget->ChangeImage(EHeartStyle::EHS_Full);
		}
		else	// �ж�С�����С
		{
			if (!FractionalPart) // С����Ϊ�㣬Ѫ��������ϣ�����ȫ����ɿ���
			{
				HeartWidget->ChangeImage(EHeartStyle::EHS_Empty);
			}
			else // С���㲻Ϊ��
			{
				if (FractionalPart > .5f) // һ��Ѫ
				{
					HeartWidget->ChangeImage(EHeartStyle::EHS_Full);
				}
				else if (FractionalPart == .5f) // ���Ѫ
				{
					HeartWidget->ChangeImage(EHeartStyle::EHS_Half);
				}
				else // �����񣬿���
				{
					HeartWidget->ChangeImage(EHeartStyle::EHS_Empty);
				}
				FractionalPart = 0; // С���㴦�����ˣ�Ϊ�˷�ֹ�ٴν���С�����ж�
			}
		}
	}
}
