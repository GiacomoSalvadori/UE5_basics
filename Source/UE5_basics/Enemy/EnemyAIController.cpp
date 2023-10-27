// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "Enemy.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EngineUtils.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"
#include "UE5_basics/TP_Character/TP_Character.h"

AEnemyAIController::AEnemyAIController() {
	PrimaryActorTick.bCanEverTick = true;

	// Set AI perception
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception"));
	
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SightConfig->SightRadius = 3000.0f;
	SightConfig->LoseSightRadius = 3500.0f;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->PeripheralVisionAngleDegrees = 45.0f;
	SightConfig->AutoSuccessRangeFromLastSeenLocation = 1600.0f;

	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
	HearingConfig->HearingRange = 3000.0f;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;

	PerceptionComponent->ConfigureSense(*SightConfig); //0
	PerceptionComponent->ConfigureSense(*HearingConfig); // 1
	PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
}

void AEnemyAIController::BeginPlay() {
	Super::BeginPlay();

	RunBehaviorTree(BehaviourTree);
	// Inscribe to delegate to stop behaviour tree when the pawn die
	AEnemy* ControlledPawn = dynamic_cast<AEnemy*>(GetPawn());
	
	if (IsValid(ControlledPawn)) {
		ControlledPawn->HealthComponent->OnHealtToZero.AddDynamic(this, &AEnemyAIController::StopAI);
		// Detect player if hit by gun
		ControlledPawn->HealthComponent->OnGetDamage.AddDynamic(this, &AEnemyAIController::DetectPlayer);
		// Set the character's walk speed
		GetBlackboardComponent()->SetValueAsFloat("OriginalWalkSpeed", ControlledPawn->GetCharacterMovement()->MaxWalkSpeed);
	}
	
	// Add OnPerceptionUpdate_SenseManagement to the UE4's perception component
	PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnPerceptionUpdate_SenseManagement);
}


void AEnemyAIController::StopAI() {
	BrainComponent->StopLogic("Death");
	AEnemy* ControlledPawn = dynamic_cast<AEnemy*>(GetPawn());
	
	if (IsValid(ControlledPawn)) {		
		ControlledPawn->GetCharacterMovement()->MaxWalkSpeed = 0.0f;
		ControlledPawn->HealthComponent->OnHealtToZero.RemoveDynamic(this, &AEnemyAIController::StopAI);
		ControlledPawn->HealthComponent->OnGetDamage.RemoveDynamic(this, &AEnemyAIController::DetectPlayer);
		ControlledPawn->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ControlledPawn->WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	Destroy();
}

void AEnemyAIController::DetectPlayer() {
	GetBlackboardComponent()->SetValueAsBool("SeePlayer", true);
}

void AEnemyAIController::OnPerceptionUpdate_SenseManagement(const TArray<AActor*>& UpdateActors) {
	for (auto& Actor : UpdateActors) {
		ATP_Character* PlayerCharacter = dynamic_cast<ATP_Character*>(Actor);
		
		if (IsValid(PlayerCharacter)) { // If spotted the player character
			const FActorPerceptionInfo* ActorInfo = PerceptionComponent->GetActorInfo(*Actor);
			
			FAISenseID SightID = SightConfig->GetSenseID();

			if (ActorInfo->LastSensedStimuli.IsValidIndex(SightID)) { // Was the sight stimuled?
				if (ActorInfo->LastSensedStimuli[SightID].WasSuccessfullySensed()) {
					ManageSight();
				}
			}

			FAISenseID HearingID = HearingConfig->GetSenseID();

			if (ActorInfo->LastSensedStimuli.IsValidIndex(HearingID)) { // Was the hearing stimuled?
				if (ActorInfo->LastSensedStimuli[HearingID].WasSuccessfullySensed()) {
					ManageHearing();
				}
			}
		}
	}
}

void AEnemyAIController::ManageSight(){
	DetectPlayer();
	NotifyTeammate();
}

void AEnemyAIController::ManageHearing(){
	DetectPlayer();
}

void AEnemyAIController::NotifyTeammate() {
	FVector MyLocation = GetPawn()->GetActorLocation();
	for (TActorIterator<AEnemyAIController> It(GetWorld()); It; ++It) {
		AEnemyAIController* Teammate = *It;
		FVector TeammateLocation = Teammate->GetPawn()->GetActorLocation();
		if (FVector::Distance(MyLocation, TeammateLocation) < TeammateAdviseRadius) { // Advise teammate in a certain radius
			Teammate->DetectPlayer(); // In this case teammate automatically detect player
		}
	}
}