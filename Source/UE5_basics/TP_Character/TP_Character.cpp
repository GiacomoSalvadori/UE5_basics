// Fill out your copyright notice in the Description page of Project Settings.


#include "TP_Character.h"

#include "AITestsCommon.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Runtime/AIModule/Classes/Perception/AIPerceptionStimuliSourceComponent.h"
#include "UE5_basics/Enemy/Enemy.h"
#include "UE5_basics/Other/HealthComponent.h"
#include "UE5_basics/Other/WeaponSlot.h"


//////////////////////////////////////////////////////////////////////////
// ATP_ThirdPersonCharacter

ATP_Character::ATP_Character() {
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	
	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->SetCrouchedHalfHeight(52.0f);

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	
	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
		
	// Add a mesh for the weapon
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "hand_rSocket");
	
	// Add Stimuli Source
	StimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimuli Source"));

	//Add component for Health management
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));

	//Set other variabled
	ActiveWeapon = 0;
	ActiveThrowable = 0;
	CheckCoverRadius = 60.0f;
	ActualEight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	bIsAiming = false;
}

void ATP_Character::BeginPlay() {
	Super::BeginPlay();
	
	bCanMove = true;
	MagBullets = Arsenal[ActiveWeapon].MagCapacity;
	FireTime = 0.0f;
	FVector WeaponLocation = GetMesh()->GetSocketLocation("hand_rSocket");
	FRotator WeaponRotaion = GetMesh()->GetSocketRotation("hand_rSocket");
	
	MaxSpeedWalkingOrig = GetCharacterMovement()->MaxWalkSpeed;
	if (MovementCurve && OffsetCurve) {
		FOnTimelineFloat ProgressFunctionLength;
		ProgressFunctionLength.BindUFunction(this, "HandleProgressArmLength");
		AimTimeline.AddInterpFloat(MovementCurve, ProgressFunctionLength);

		FOnTimelineVector ProgressFunctionOffset;
		ProgressFunctionOffset.BindUFunction(this, "HandleProgressCameraOffset");
		AimTimeline.AddInterpVector(OffsetCurve, ProgressFunctionOffset);

	}

	if (CrouchCurve) {
		FOnTimelineFloat ProgressFunctionCrouch;
		ProgressFunctionCrouch.BindUFunction(this, "HandleProgressCrouch");
		CrouchTimeline.AddInterpFloat(CrouchCurve, ProgressFunctionCrouch);
	}


	HealthComponent->OnHealtToZero.AddDynamic(this, &ATP_Character::StopCharacter);
}

void ATP_Character::OnConstruction(const FTransform & Transform) {
	if (Arsenal.Num() > 0) {
		WeaponMesh->SetStaticMesh(Arsenal[0].WeaponMesh);
	}
}


void ATP_Character::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	AimTimeline.TickTimeline(DeltaTime);
	CrouchTimeline.TickTimeline(DeltaTime);
	
	AutomaticFire(DeltaTime);
}

//////////////////////////////////////////////////////////////////////////
// Timeline management

void ATP_Character::HandleProgressArmLength(float Length) {
	CameraBoom->TargetArmLength = Length;
}

void ATP_Character::HandleProgressCameraOffset(FVector Offset) {
	CameraBoom->SocketOffset = Offset;
}

void ATP_Character::HandleProgressCrouch(float Height) {
	ActualEight = Height;
}

//////////////////////////////////////////////////////////////////////////
// Input

void ATP_Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) {
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ATP_Character::JumpCharacter);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ATP_Character::StopJumpingCharacter);
	
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ATP_Character::AimInWeapon);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &ATP_Character::AimOutWeapon);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ATP_Character::Fire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ATP_Character::StopFire);

	PlayerInputComponent->BindAction("Cover", IE_Pressed, this, &ATP_Character::ToggleCover);
	
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ATP_Character::ReloadWeapon);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATP_Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATP_Character::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ATP_Character::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ATP_Character::LookUpAtRate);
}

//////////////////////////////////////////////////////////////////////////
// Movement and rotation

void ATP_Character::TurnAtRate(float Rate) {
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ATP_Character::LookUpAtRate(float Rate) {
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}


void ATP_Character::MoveForward(float Value) {
	if ((Controller != NULL) && (Value != 0.0f) & !bIsInCover && bCanMove) {
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ATP_Character::MoveRight(float Value) {
	if ((Controller != NULL) && (Value != 0.0f) && bCanMove) {
		if (!bIsInCover) {
			// find out which way is right
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get right vector 
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			// add movement in that direction
			AddMovementInput(Direction, Value);
		} else {
			//Move according to the cover actor's position
			if (CheckAroundMe(CheckCoverRadius)) {
				AddMovementInput(CoverDirectionMovement, Value);
			} else {
				ExitFromCover();
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Mechanic: Aim

void ATP_Character::AimInWeapon() {
	if (!bIsUsingArch) {
		bIsUsingWeapon = true;
		AimIn();
	}
}

void ATP_Character::AimOutWeapon() {
	bIsUsingWeapon = false;
	AimOut();
}

void ATP_Character::AimIn() {
	GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Green, TEXT("Aim In"));
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->MaxWalkSpeed = MaxSpeedAiming;
	bIsAiming = true;
	if (bIsInCover) {
		GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Green, TEXT("Aim from cover"));
		StopCrouchCharacter();
	}
	AimTimeline.Play();
	OnCharacterAim.Broadcast();
}

void ATP_Character::AimOut() {
	GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Green, TEXT("Aim Out"));
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = MaxSpeedWalkingOrig;
	bIsAiming = false;
	if (bIsInCover) {
		GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Green, TEXT("Stop aim from cover"));
		CrouchCharacter();
	}
	AimTimeline.Reverse();
	OnCharacterStopAim.Broadcast();
}

//////////////////////////////////////////////////////////////////////////
// Mechanic: Jump

void ATP_Character::JumpCharacter() {
	if (CanJump()) {
		Jump();
		GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Green, TEXT("jumping"));
	}
}

void ATP_Character::StopJumpingCharacter() {
	StopJumping();
}

void ATP_Character::Landed(const FHitResult& Hit) {
	GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Green, TEXT("landed"));
	OnCharacterLanding.Broadcast();
}

/** This is a UE4 function of AActor class*/
void ATP_Character::OnJumped_Implementation() { 
	GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Green, TEXT("On jumped"));
	OnCharacterJumping.Broadcast();
}

//////////////////////////////////////////////////////////////////////////
// Mechanic: Crouch

void ATP_Character::CrouchCharacter() {
	
	if (CanCrouch()) {
		GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Green, TEXT("Crouch in"));
		Crouch();
		CrouchTimeline.Play();
		OnCharacterCrouch.Broadcast();
	}
}

void ATP_Character::StopCrouchCharacter() {
	GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Green, TEXT("Uncrouch"));
	if (GetCharacterMovement()->IsCrouching()) {
		UnCrouch();
		CrouchTimeline.Reverse();
		OnCharacterUncrouch.Broadcast();
	}
}

//////////////////////////////////////////////////////////////////////////
// Mechanic: Fire with weapon

void ATP_Character::FireFromWeapon() {
	FCollisionQueryParams Params;
	// Ignore the player's pawn
	Params.AddIgnoredActor(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	// The hit result gets populated by the line trace
	FHitResult Hit;

	float WeaponRange = Arsenal[ActiveWeapon].Range;

	FVector Start = FollowCamera->GetComponentLocation();
	FVector End = Start + (FollowCamera->GetComponentRotation().Vector() * WeaponRange);

	if (!bIsAiming) {
		float WeaponOffset = Arsenal[ActiveWeapon].Offset;
		GEngine->AddOnScreenDebugMessage(-1, 5.2f, FColor::Emerald, TEXT("Not aiming!"));
		Start = WeaponMesh->GetComponentLocation() + (WeaponMesh->GetForwardVector() * WeaponOffset);
		End = Start + (WeaponMesh->GetComponentRotation().Vector() * WeaponRange);
	}

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Pawn, Params);
	OnCharacterTraceLine.Broadcast();

	if (bHit) {
		//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 3.0f);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Arsenal[ActiveWeapon].HitEFX, Hit.ImpactPoint);
		AEnemy* HitActor = Cast<AEnemy>(Hit.GetActor());

		if (HitActor) {
			HitActor->GetHealthComponent()->GetDamage(Arsenal[ActiveWeapon].Damage);
		}
	}
}

void ATP_Character::AutomaticFire(float DeltaTime) {
	if (Arsenal[ActiveWeapon].IsAutomatic && bIsFiring) {
		FireTime += DeltaTime;
		if (FireTime >= Arsenal[ActiveWeapon].Rate) {
			FireTime = 0.0f;
			if (MagBullets > 0) {
				FireFromWeapon();
				MagBullets--;
			} else {
				StopFire();
				ReloadWeapon();
			}
		}
	}
}

void ATP_Character::Fire() {
	if (!bIsReloading) {
		if (bIsUsingArch) {
			StartThrow();
		} else {
			bIsFiring = true;
			FireTime = 0.0f;
			if (MagBullets > 0) {
				FireFromWeapon();
				MagBullets--;
			} else {
				StopFire();
				ReloadWeapon();
			}
		}
	}
}

void ATP_Character::StopFire() {
	bIsFiring = false;
	FireTime = 0.0f;
}

FWeaponSlot ATP_Character::RetrieveActiveWeapon() {
	return Arsenal[ActiveWeapon];
}

//////////////////////////////////////////////////////////////////////////
// Mechanic: Cover

void ATP_Character::ToggleCover() {
	bool bIsNear = CheckAroundMe(CheckCoverRadius);
	if (bIsNear) { // Check if cover is near
		bIsInCover = !bIsInCover;
		// DEBUG message
		if (bIsInCover) {
			GEngine->AddOnScreenDebugMessage(-1, 2.2f, FColor::Green, TEXT("Cover now!"));
		} else {
			GEngine->AddOnScreenDebugMessage(-1, 2.2f, FColor::Green, TEXT("Not Cover now!"));
		}
		
		if (bIsInCover && Cover) {			
			FRotator CoverRotation;
			Cover->RetrieveMovementDirectionAndFacingRotation(CoverDirectionMovement, CoverRotation);
			SetActorRotation(CoverRotation);
			CrouchCharacter();
		} else {
			ExitFromCover();
		}
	}
}

void ATP_Character::ExitFromCover() {
	bIsInCover = false;
	StopCrouchCharacter();
}


bool ATP_Character::CheckAroundMe(float Radius) {
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	FCollisionShape CollShape = FCollisionShape::MakeSphere(Radius);

	TArray<FHitResult> Hit;

	FVector Pos = GetCapsuleComponent()->GetComponentLocation();
	bool bHit = GetWorld()->SweepMultiByChannel(Hit, Pos, Pos, FQuat::Identity, ECC_Pawn, CollShape, Params);
	//DrawDebugSphere(GetWorld(), Pos, Radius, 12, FColor::Orange, false, 0.2f);
	
	if (bHit) {
		for (auto& HitActor : Hit) {
			ACoverActor* HitCover = Cast<ACoverActor>(HitActor.GetActor());
			if (HitCover) {
				Cover = HitCover;
				return true;
			}
		}
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////
// Mechanic: Reload

void ATP_Character::ReloadWeapon() {
	if (!bIsUsingArch && !bIsReloading) {
		GEngine->AddOnScreenDebugMessage(-1, 5.2f, FColor::Red, TEXT("Start Reload!"));
		bIsReloading = true;
		OnCharacterStartReload.Broadcast();
	}
}

void ATP_Character::EndReload() {
	GEngine->AddOnScreenDebugMessage(-1, 5.2f, FColor::Orange, TEXT("End Reload!"));
	bIsReloading = false;
	MagBullets = Arsenal[ActiveWeapon].MagCapacity;
}

int ATP_Character::MagCounter() {
	return MagBullets;
}

//////////////////////////////////////////////////////////////////////////
// Mechanic: Throw objects

void ATP_Character::StartThrow() {
	OnCharacterStartThrow.Broadcast();
	EnableMovement(false);
}

void ATP_Character::EndThrow() {
	OnCharacterThrow.Broadcast();
	EnableMovement(true);
}

void ATP_Character::ThrowObject() {
	FActorSpawnParameters SpawnParams;
	FVector ThrowStartLocation = FollowCamera->GetComponentLocation();
	FRotator ThrowRotator = FollowCamera->GetComponentRotation();
	FVector OutVelocity = PredictThrowablePath();
	//AActorThrowable* ThrowedObj = GetWorld()->SpawnActor<AActorThrowable>(AActorThrowable::StaticClass(), ThrowStartLocation, ThrowRotator, SpawnParams);
	//ThrowedObj->Init(GetEquipThrowable(), OutVelocity);
}

FVector ATP_Character::PredictThrowablePath() {
	FVector ThrowStartLocation = FollowCamera->GetComponentLocation();
	FVector ThrowEndLocation = ThrowStartLocation + (FollowCamera->GetForwardVector() * MaxThrowLength);
	FVector OutVelocity;
	UGameplayStatics::SuggestProjectileVelocity_CustomArc(GetWorld(), OutVelocity, ThrowStartLocation, ThrowEndLocation, 0.0f, 0.5f);

	return OutVelocity;
}

FThrowable ATP_Character::GetEquipThrowable() {
	return Throwables[ActiveThrowable];
}


//////////////////////////////////////////////////////////////////////////
// Utilities

void ATP_Character::StopCharacter() {
	HealthComponent->OnHealtToZero.RemoveDynamic(this, &ATP_Character::StopCharacter);
	if (bIsAiming) {
		AimOut();
	}
	
	EnablePlayerInput(false);
}

void ATP_Character::EnablePlayerInput(bool Enabled) {
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (Enabled) {
		EnableInput(PlayerController);
	} else {
		DisableInput(PlayerController);
	}
}

void ATP_Character::EnableMovement(bool Enabled) {
	bCanMove = Enabled;
}

bool ATP_Character::IsAimingWithWeapon() {
	return bIsAiming && bIsUsingWeapon;
}

bool ATP_Character::IsAimingWithArch() {
	return bIsAiming && bIsUsingArch;
}
