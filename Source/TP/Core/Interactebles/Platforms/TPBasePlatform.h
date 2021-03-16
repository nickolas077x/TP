// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/TimelineComponent.h>
#include <Engine/EngineTypes.h>
#include <Components/StaticMeshComponent.h>
#include <Components/BoxComponent.h>
#include "TPBasePlatform.generated.h"

UCLASS()
class TP_API ATPBasePlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATPBasePlatform();

	DECLARE_EVENT_OneParam(ATPBasePlatform, FPlatformClientOnSwitchedSignature, bool);
		FPlatformClientOnSwitchedSignature OnPlatformClientSwitched;

protected:
	virtual void BeginPlay() override;

	float MovementRate = 1.0f;

	FTimeline PlatformPushTimeline;

	FTimerHandle TimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* PlatformMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UBoxComponent* PlatformCollisionMesh;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Transient)
		FVector EndLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient)
		FVector StartLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float ForwardMovingRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float BackwardMovingRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float WaitingTime = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UCurveFloat* PlatformAlpha;

	UFUNCTION(BlueprintCallable)
		void MoveThePlatform();

	UFUNCTION()
		void PlatformMovementProgress(float Value);

	UFUNCTION()
		void OnTimelineFinished();

	UFUNCTION()
		void OnBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
