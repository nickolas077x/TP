// Fill out your copyright notice in the Description page of Project Settings.


#include "TPBasePlatform.h"
#include <Components/CapsuleComponent.h>
#include <Net/UnrealNetwork.h>

// Sets default values
ATPBasePlatform::ATPBasePlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	USceneComponent* DefaultPlatformRoot = CreateDefaultSubobject<USceneComponent>(TEXT("PlatformRoot"));
	RootComponent = DefaultPlatformRoot;

	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform"));
	PlatformMesh->SetupAttachment(DefaultPlatformRoot);

	PlatformCollisionMesh = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	PlatformCollisionMesh->SetupAttachment(PlatformMesh);
}


void ATPBasePlatform::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATPBasePlatform, bIsOn);
}

// Called when the game starts or when spawned
void ATPBasePlatform::BeginPlay()
{
	Super::BeginPlay();

	PlatformCollisionMesh->OnComponentBeginOverlap.AddDynamic(this, &ATPBasePlatform::OnBoxOverlapBegin);
	PlatformCollisionMesh->OnComponentEndOverlap.AddDynamic(this, &ATPBasePlatform::OnBoxOverlapEnd);

	StartLocation = PlatformMesh->GetRelativeLocation();
	EndLocation = PlatformMesh->GetRelativeLocation() - FVector(0.0f, 0.0f, 9.0f);

	if (IsValid(PlatformAlpha))
	{
		FOnTimelineFloat PlatformMovementProgress;

		PlatformMovementProgress.BindUFunction(this, FName("PlatformMovementProgress"));
		PlatformPushTimeline.AddInterpFloat(PlatformAlpha, PlatformMovementProgress);
		PlatformPushTimeline.SetLooping(false);

		FOnTimelineEvent OnFinishedCallback;
		OnFinishedCallback.BindUFunction(this, FName("OnTimelineFinished"));
		PlatformPushTimeline.SetTimelineFinishedFunc(OnFinishedCallback);
	}
}

void ATPBasePlatform::PlatformMovementProgress(float Value)
{
	FVector NewLocation = FMath::Lerp(StartLocation, EndLocation, Value);
	PlatformMesh->SetRelativeLocation(NewLocation);
}


void ATPBasePlatform::ChangeStatus(bool bIsOnOld)
{
	bIsOn = bIsOnOld;
}

void ATPBasePlatform::OnBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority())
	{
		ChangeStatus(true);
		if (OnPlatformClientSwitched.IsBound())
		{
			OnPlatformClientSwitched.Broadcast(bIsOn);
		}

		PlatformPushTimeline.SetPlayRate(ForwardMovingRate);
		PlatformPushTimeline.Play();
	}
}

void ATPBasePlatform::OnBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (HasAuthority())
	{
		ChangeStatus(false);
		if (OnPlatformClientSwitched.IsBound())
		{
			OnPlatformClientSwitched.Broadcast(bIsOn);
		}

		PlatformPushTimeline.SetPlayRate(BackwardMovingRate);
	}	PlatformPushTimeline.Reverse();
}

// Called every frame
void ATPBasePlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PlatformPushTimeline.TickTimeline(DeltaTime);
}

