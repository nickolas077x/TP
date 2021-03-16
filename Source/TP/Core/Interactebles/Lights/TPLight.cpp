// Fill out your copyright notice in the Description page of Project Settings.


#include "TPLight.h"
#include <Components/SceneComponent.h>

// Sets default values
ATPLight::ATPLight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = DefaultRoot;

	LightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshLight"));
	LightMesh->SetupAttachment(DefaultRoot);

	LightPoint = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	LightPoint->SetupAttachment(DefaultRoot);

}

// Called when the game starts or when spawned
void ATPLight::BeginPlay()
{
	Super::BeginPlay();

	LightMaterial = LightMesh->GetMaterial(0);
	DynamicMaterial = UMaterialInstanceDynamic::Create(LightMaterial, NULL);
	LightMesh->SetMaterial(0, DynamicMaterial);

	Platform->OnPlatformClientSwitched.AddUObject(this, &ATPLight::OnPlatformSwitched);

	FOnTimelineFloat LightChangeProgress;

	LightChangeProgress.BindUFunction(this, FName("LightChangeProgress"));
	LightChangeTimeline.AddInterpFloat(LightAlpha, LightChangeProgress);
	LightChangeTimeline.SetLooping(false);
	LightChangeTimeline.SetPlayRate(BrightnessRate);

	if (LightColour == ELightColour::Blue)
	{
		DynamicMaterial->SetVectorParameterValue(TEXT("Colour"), FVector(0.f, 0.f, 0.9f));
		LightPoint->SetLightColor(FLinearColor(0.0f, 0.0f, 0.9f, 0.5f));
	}
	if (LightColour == ELightColour::Red)
	{
		DynamicMaterial->SetVectorParameterValue(TEXT("Colour"), FVector(9.f, 0.f, 0.0f));
		LightPoint->SetLightColor(FLinearColor(0.9f, 0.0f, 0.0f, 0.5f));
	}
	if (LightColour == ELightColour::Green)
	{
		DynamicMaterial->SetVectorParameterValue(TEXT("Colour"), FVector(0.f, 9.f, 0.0f));
		LightPoint->SetLightColor(FLinearColor(0.0f, 0.9f, 0.f, 0.5f));
	}
	LightPoint->SetIntensity(0.0001f);
}



void ATPLight::OnPlatformSwitched(bool bIsOn)
{
	if (IsValid(LightMaterial))
	{	
		if (bIsOn)
		{
			LightChangeTimeline.Play();
		}
		else
		{
			LightChangeTimeline.Reverse();
		}

	}
}


void ATPLight::LightChangeProgress(float Value)
{
	float Brightness = FMath::Lerp(0.0f, MaxBrightness, Value);
	DynamicMaterial->SetScalarParameterValue(TEXT("Bright"), Brightness);
	LightPoint->SetIntensity(Brightness * 5000);

	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Out")));
}

// Called every frame
void ATPLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LightChangeTimeline.TickTimeline(DeltaTime);
}

