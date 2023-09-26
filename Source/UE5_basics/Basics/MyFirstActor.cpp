// Fill out your copyright notice in the Description page of Project Settings.


#include "MyFirstActor.h"

// Sets default values
AMyFirstActor::AMyFirstActor()
{
 	PrimaryActorTick.bCanEverTick = true; // can tick?
	PrimaryActorTick.TickInterval = 0.2f; // what is the interval tick?

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	/*	
	BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	BoxComponent->SetGenerateOverlapEvents(true);
	BoxComponent->SetBoxExtent(FVector(200.f, 200.f, 100.f));
	BoxComponent->SetCanEverAffectNavigation(false);
	*/
	BoxComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMyFirstActor::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &AMyFirstActor::OnBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &AMyFirstActor::OnEndOverlap);

	// Bind example
	/*
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AFP_FirstPersonCharacter * FPCharacter = dynamic_cast<AFP_FirstPersonCharacter*>(PlayerCharacter);
    if(IsValid(FPCharacter))
    {
    	FPCharacter->OnLineTraceCast.AddDynamic(this, &AMyFirstActor::BindTest);
    }
    */
}

void AMyFirstActor::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if(!IsValid(OtherActor))
	{
		return;
	}

	UClass* OtherActorClass = OtherActor->GetClass();
	for(auto& SensedClass : SensedCharacterClasses)
	{
		if(SensedClass == OtherActorClass || OtherActorClass->IsChildOf(SensedClass))
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, "IN: "+OtherActor->GetName());
		}
	}
	
	/*
	UWorld* World = GetWorld();
	if(!IsValid(World))
	{
		return;
	}
	
	if(OtherActor == UGameplayStatics::GetPlayerPawn(World, 0))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, "IN: "+OtherActor->GetName());
	}
	*/
}

void AMyFirstActor::OnEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if(!IsValid(OtherActor))
	{
		return;
	}

	UClass* OtherActorClass = OtherActor->GetClass();
	for(auto& SensedClass : SensedCharacterClasses)
	{
		if(SensedClass == OtherActorClass || OtherActorClass->IsChildOf(SensedClass))
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, "OUT: "+OtherActor->GetName());
		}
	}

	/*
	UWorld* World = GetWorld();
	if(!IsValid(World))
	{
		return;
	}
	
	if(OtherActor == UGameplayStatics::GetPlayerPawn(World, 0))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, "OUT: "+OtherActor->GetName());
	}
	*/
}

void AMyFirstActor::BindTest()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Purple, "Player has shoot!");
}

// Called every frame
void AMyFirstActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GEngine->AddOnScreenDebugMessage(896745, 2.f, FColor::Green, FString::Printf(TEXT("world time: %f"), GetWorld()->TimeSeconds));
	GEngine->AddOnScreenDebugMessage(8967444, 2.f, FColor::Blue, "world time: "+FString::SanitizeFloat(GetWorld()->TimeSeconds));
}
