#include "UI/Inventory/HeartWidget.h"
#include "Components/Image.h"

void UHeartWidget::ChangeImage(const EHeartStyle& Syle)
{
	switch (Syle)
	{
	case EHeartStyle::EHS_Full:
		Heart->SetBrushFromTexture(FullHeart);
		break;
	case EHeartStyle::EHS_Half:
		Heart->SetBrushFromTexture(HalfHeart);
		break;
	case EHeartStyle::EHS_Empty:
		Heart->SetBrushFromTexture(EmptyHeart);
		break;
	}
}
