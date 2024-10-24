// Fill out your copyright notice in the Description page of Project Settings.


#include "MyTimedActor.h"

#include "Components/BillboardComponent.h"

// Sets default values
AMyTimedActor::AMyTimedActor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		// A helper class object we use to find target UTexture2D object in resource package
		ConstructorHelpers::FObjectFinderOptional<UTexture2D> NoteTextureObject;

		// Icon sprite category name
		FName ID_Notes;

		// Icon sprite display name
		FText NAME_Notes;

		FConstructorStatics()
			// Use helper class object to find the texture
			// "/Engine/EditorResources/S_Note" is resource path
			: NoteTextureObject(TEXT("/Game/Icons/S_Clock"))
			, ID_Notes(TEXT("Notes"))
			, NAME_Notes(NSLOCTEXT("SpriteCategory", "Notes", "Notes"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// We need a scene component to attach Icon sprite
	USceneComponent* SceneComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("SceneComp"));
	RootComponent = SceneComponent;
	RootComponent->Mobility = EComponentMobility::Static;

#if WITH_EDITORONLY_DATA
	SpriteComponent = ObjectInitializer.CreateEditorOnlyDefaultSubobject<UBillboardComponent>(this, TEXT("Sprite"));
	if (SpriteComponent && ConstructorStatics.NoteTextureObject.Succeeded())
	{
		SpriteComponent->Sprite = ConstructorStatics.NoteTextureObject.Get();       // Get the sprite texture from helper class object
		SpriteComponent->SpriteInfo.Category = ConstructorStatics.ID_Notes;     // Assign sprite category name
		SpriteComponent->SpriteInfo.DisplayName = ConstructorStatics.NAME_Notes;    // Assign sprite display name
		SpriteComponent->SetupAttachment(RootComponent);  // Attach sprite to scene component
		SpriteComponent->Mobility = EComponentMobility::Static;
		SpriteComponent->EditorScale = 1.0f;
	}
#endif // WITH_EDITORONLY_
}

// Called when the game starts or when spawned
void AMyTimedActor::BeginPlay()
{
	Super::BeginPlay();

	StartTimer();	
}

// Called every frame
void AMyTimedActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AMyTimedActor::StartTimer()
{
	UWorld* World = GetWorld();
	if(!IsValid(World))
	{
		return;
	}

	FTimerManager& TimerManager = World->GetTimerManager();
	if(!TimerManager.IsTimerActive(TimerHandler))
	{
		TimerManager.SetTimer(TimerHandler, this, &AMyTimedActor::TimerCallback, Duration, bUseInLoop);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, "Timer is set!");
	} else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Timer is used!");
	}
}

void AMyTimedActor::TimerCallback()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, "Timer is over!");
	UWorld* World = GetWorld();
	if(!IsValid(World))
	{
		return;
	}

	if(!bUseInLoop)
	{
		World->GetTimerManager().ClearTimer(TimerHandler); // clean up the timer	
	}	
	
	OnTimerRunsOut(); // this is the implementable event
}