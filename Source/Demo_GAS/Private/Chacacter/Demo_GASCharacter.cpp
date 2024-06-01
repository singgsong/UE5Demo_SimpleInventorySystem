#include "Chacacter/Demo_GASCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include <Actor/ItemBase.h>
#include "Interface/Interaction.h"
#include "EnumFile.h"
#include "Components/SceneCaptureComponent2D.h"

ADemo_GASCharacter::ADemo_GASCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; 
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); 

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 400.0f; 
	CameraBoom->bUsePawnControlRotation = true; 

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom); 
	Camera->bUsePawnControlRotation = false; 

	UnVisibleHand = CreateDefaultSubobject<USceneComponent>(TEXT("UnVisibleHand"));
	UnVisibleHand->SetupAttachment(CameraBoom);
	
	RightHand = CreateDefaultSubobject<USceneComponent>(TEXT("RightHand"));
	RightHand->SetupAttachment(GetMesh());
	
	// Inventory
	SceneCapture2D = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture2D"));
	SceneCapture2D->SetupAttachment(GetRootComponent());

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), FName("RightHand"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Shield = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shield"));
	Shield->SetupAttachment(GetMesh(), FName("LeftHand"));
	Shield->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ADemo_GASCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FVector StartLoca = Camera->GetComponentLocation();
	FVector Forward = Camera->GetForwardVector();
	FVector EndLoca = InteractionDistance * Forward + StartLoca;
	//DrawDebugSphere(GetWorld(), EndLoca, 10.f, 12, FColor::Yellow);
	bTracedItem =  UKismetSystemLibrary::LineTraceSingleForObjects(
		this,
		StartLoca, 
		EndLoca, 
		ItemTypes, 
		false, 
		ActorsToIgnore, 
		EDrawDebugTrace::None, 
		OutHit,
		true);
}

UStaticMesh* ADemo_GASCharacter::GetWeaponMesh() const
{
	return Weapon->GetStaticMesh();
}

UStaticMesh* ADemo_GASCharacter::GetShieldMesh() const
{
	return Shield->GetStaticMesh();
}

void ADemo_GASCharacter::SetWeaponMesh(UStaticMesh* InMesh)
{
	Weapon->SetStaticMesh(InMesh);
}

void ADemo_GASCharacter::SetShieldMesh(UStaticMesh* InMesh)
{
	Shield->SetStaticMesh(InMesh);
}

void ADemo_GASCharacter::BeginPlay()
{
	Super::BeginPlay();
	SceneCapture2D->ShowOnlyActorComponents(this);
}

void ADemo_GASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADemo_GASCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADemo_GASCharacter::Look);
		
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ADemo_GASCharacter::Interact);
		EnhancedInputComponent->BindAction(ThrowAction, ETriggerEvent::Started, this, &ADemo_GASCharacter::Throw);
	}
}

#pragma region 运动系统
void ADemo_GASCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ADemo_GASCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

#pragma endregion

// 原力抓取
void ADemo_GASCharacter::Interact()
{
	AActor* HitActor = OutHit.GetActor();
	
	if (IInteraction* Interact = Cast<IInteraction>(HitActor))
	{
		if (Interact->GetItemType() == EItemType::EIT_PhysicsObject) // 物理对象
		{
			Interact->PickAndDrop(UnVisibleHand);
		}
		else // 工具
		{
			if (!PickedUpItem)
			{
				PickedUpItem = Cast<AItemBase>(HitActor);
				Interact->Equip(this, RightHand);
			}
			else
			{
				PickedUpItem->DropItem();
				PickedUpItem = nullptr;
			}
		}
	}
}

void ADemo_GASCharacter::Throw()
{
	if (PickedUpItem)
	{
		PickedUpItem->DropItem();
		PickedUpItem = nullptr;
	}
}

