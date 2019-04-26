// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Brian_A_FinalCharacter.h"
#include "DestroyLinkActor.generated.h"

UCLASS()
class BRIAN_A_FINAL_API ADestroyLinkActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADestroyLinkActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
    int colorSetting;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
    UStaticMeshComponent* MyVisual;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
    UBoxComponent* MyCollision;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
    TArray<UMaterialInterface*> ColorList;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
    void SetColor(int color);
    void SetDestroyed(ABrian_A_FinalCharacter* ShootingPlayer);

    UFUNCTION(NetMulticast, Reliable, WithValidation)
    void ServerSetColor(int color);

    UFUNCTION(Server, Reliable, WithValidation)
    void ServerSetDestroyed(ABrian_A_FinalCharacter* ShootingPlayer);
	
};
