// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TweenTypes.h"
#include "TypeTween.h"
#include "MyTweenableActor.generated.h"

UCLASS()
class TYPETWEENEXAMPLES_API AMyTweenableActor : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMyTweenableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Tween")
	FTransform Offset;

	UPROPERTY(EditAnywhere, Category = "Tween")
	FTweenConfig MovePreset;
	UPROPERTY(EditAnywhere, Category = "Tween")
	FTweenConfig RotatePreset;
	UPROPERTY(EditAnywhere, Category = "Tween")
	FTweenConfig ScalePreset;

private:
	//TypeTween::FTweenHandle MoveTween;

	void CompileChecks();
};
