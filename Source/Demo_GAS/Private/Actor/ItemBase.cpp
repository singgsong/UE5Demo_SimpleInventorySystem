#include "Actor/ItemBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/EngineTypes.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Chacacter/Demo_GASCharacter.h"
#include "Curves/CurveVector.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"

AItemBase::AItemBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetSimulatePhysics(true);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	Timeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));

}

void AItemBase::BeginPlay()
{
	Super::BeginPlay();

	Mesh->OnComponentSleep.AddDynamic(this, &AItemBase::HandleComponentSleeping);
}

// 当物体静止时，停止物理模拟
void AItemBase::HandleComponentSleeping(UPrimitiveComponent* SleepingComponent, FName BoneName)
{
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
}

#pragma region 原力交互
// 捡起和丢下
void AItemBase::PickAndDrop(USceneComponent* Hand)
{
	if (!bPickedUp) // 捡起
	{
		bPickedUp = true;
		Mesh->SetSimulatePhysics(false);
		FAttachmentTransformRules Rules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true);
		Mesh->AttachToComponent(Hand, Rules);
	}
	else // 丢下
	{
		LayDown();
	}
}

// 工具
void AItemBase::Equip(ADemo_GASCharacter* Pawn, USceneComponent* Hand)
{
	if (Player = Cast<ADemo_GASCharacter>(Pawn))
	{
		if (!bPickedUp)
		{
			Mesh->SetSimulatePhysics(false);
			Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
			LerpToHand();
		}
		else
		{
			LayDown();
		}
	}
}

void AItemBase::LayDown()
{
	bPickedUp = false;
	FDetachmentTransformRules Rules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, false);
	DetachFromActor(Rules);
	Mesh->SetSimulatePhysics(true);
}

void AItemBase::LerpToHand()
{
	FOnTimelineFloat OnFlot;
	FOnTimelineVector OnVector;
	FOnTimelineEvent OnUpdateFunc;
	FOnTimelineEventStatic OnFinishedFunc;

	OnUpdateFunc.BindDynamic(this, &AItemBase::LerpToHandUpdateFunc);
	OnFinishedFunc.BindLambda([this]()
		{
			if (!Player) return;
			FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
			AttachToComponent(Player->GetRightHand(), Rules);
		});

	Timeline->AddInterpFloat(Curve_LerpToSlot, OnFlot);
	Timeline->AddInterpVector(Curve_LerpToHand, OnVector);
	Timeline->SetTimelinePostUpdateFunc(OnUpdateFunc);
	Timeline->SetTimelineFinishedFunc(OnFinishedFunc);
	Timeline->PlayFromStart();
}

void AItemBase::LerpToHandUpdateFunc()
{
	if (!Player) return;
	const float UpdateFloat = Curve_LerpToSlot->GetFloatValue(Timeline->GetPlaybackPosition());
	const FVector UpdateVector = Curve_LerpToHand->GetVectorValue(Timeline->GetPlaybackPosition());
	const FVector Location = UKismetMathLibrary::VLerp(GetActorLocation(), Player->GetRightHand()->GetComponentLocation() + UpdateVector, UpdateFloat);
	const FRotator Rotator = UKismetMathLibrary::RLerp(GetActorRotation(), Player->GetRightHand()->GetComponentRotation(), UpdateFloat, true);
	SetActorLocationAndRotation(Location, Rotator);
}

void AItemBase::DropItem()
{
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	LayDown();
}

#pragma endregion
