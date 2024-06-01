#include "Game/MyPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Component/InventoryComponent.h"

AMyPlayerController::AMyPlayerController()
{
	// 库存系统
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	// 初始化库存
	InventoryComponent->InitializeInteractInput(this);
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}


