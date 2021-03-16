// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Platforms/TPBasePlatform.h"
#include <Components/StaticMeshComponent.h>
#include <Components/TimelineComponent.h>
#include <Engine/EngineTypes.h>
#include <Components/PointLightComponent.h>
#include "TPLight.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlatformClientOnSwitchedSignature, bool, bIsOn);

UENUM(BlueprintType)
enum class ELightColour : uint8
{
	Red = 0,
	Green,
	Blue
};

UCLASS()
class TP_API ATPLight : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATPLight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* LightMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UPointLightComponent* LightPoint;
	
	UPROPERTY(EditInstanceOnly)
		ATPBasePlatform* Platform;

		FTimeline LightChangeTimeline;

		FTimerHandle TimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UCurveFloat* LightAlpha;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float BrightnessRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MaxBrightness = 70.0f;

	UPROPERTY(EditAnywhere, Category = "Light Client")
		FPlatformClientOnSwitchedSignature OnPlatformClientSwitched;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		ELightColour LightColour = ELightColour::Red;
	
	UPROPERTY()
		UMaterialInstanceDynamic* DynamicMaterial;
		
	UPROPERTY()
		UMaterialInterface* LightMaterial;
	
	UFUNCTION()
		void OnPlatformSwitched(bool bIsOn);

	UFUNCTION()
		void LightChangeProgress(float Value);

		UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float ForwardMovingRate = 1.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
