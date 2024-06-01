// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Demo_GASCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;
class UPhysicsHandleComponent;
class AItemBase;
class USceneCaptureComponent2D;

DECLARE_DELEGATE(FOnUseHeldItem)


UCLASS(config=Game)
class ADemo_GASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ADemo_GASCharacter();

	virtual void Tick(float DeltaSeconds) override;

	FOnUseHeldItem OnUseHeldItem;
	FORCEINLINE UStaticMesh* GetWeaponMesh() const;
	FORCEINLINE UStaticMesh* GetShieldMesh() const;

	FORCEINLINE void SetWeaponMesh(UStaticMesh* InMesh);
	FORCEINLINE void SetShieldMesh(UStaticMesh* InMesh);

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void Interact();
	void Throw();

#pragma region 运动系统变量
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

#pragma endregion

	UPROPERTY(EditAnywhere)
	TArray<TEnumAsByte<EObjectTypeQuery>> ItemTypes;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> ActorsToIgnore;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InteractAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ThrowAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> UnVisibleHand;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> RightHand;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPhysicsHandleComponent> PhysicsHandle;

	UPROPERTY(EditAnywhere)
	float InteractionDistance = 100;

	FHitResult OutHit;
	bool bTracedItem = false;

	UPROPERTY(EditAnywhere)
	float HoldingDistance = 100;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AItemBase> EquippedItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AItemBase> PickedUpItem;

	UPROPERTY(EditAnywhere)
	float ThrowVector = 10000;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneCaptureComponent2D> SceneCapture2D;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Weapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Shield;


public:
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return Camera; }
	FORCEINLINE USceneComponent* GetRightHand() const { return RightHand; }
	FORCEINLINE UStaticMeshComponent* GetWeapon() const { return Weapon; }
	FORCEINLINE UStaticMeshComponent* GetShield() const { return Shield; }

};

