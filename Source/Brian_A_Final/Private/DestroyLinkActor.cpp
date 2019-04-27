// Fill out your copyright notice in the Description page of Project Settings.

#include "DestroyLinkActor.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Pawn.h"
#include "Brian_A_FinalGameMode.h"
#include "ScoringPlayerState.h"


// Sets default values
ADestroyLinkActor::ADestroyLinkActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;
    SetReplicateMovement(true);
    SetReplicates(true);
    

    MyVisual = CreateDefaultSubobject<UStaticMeshComponent>( "linkedVisual");
    this->RootComponent = MyVisual;
    MyVisual->SetCollisionProfileName("BlockAll");
    MyVisual->SetSimulatePhysics(true);
    MyVisual->SetEnableGravity(true);

    MyCollision = CreateDefaultSubobject<UBoxComponent>("boxComponent");
    MyCollision->AttachToComponent(MyVisual, FAttachmentTransformRules::KeepRelativeTransform);
    MyCollision->SetCollisionProfileName("OverlapAll");
}

// Called when the game starts or when spawned
void ADestroyLinkActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADestroyLinkActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADestroyLinkActor::SetColor(int color) { ServerSetColor(color); }
bool ADestroyLinkActor::ServerSetColor_Validate(int color) { return true; }
void ADestroyLinkActor::ServerSetColor_Implementation(int color)
{
   // if (Role != ROLE_Authority)
   //     return;

    this->colorSetting = color;
    if ((color > 0) && (color <= this->ColorList.Num()))
        MyVisual->SetMaterial(0, ColorList[color]);
}

void ADestroyLinkActor::SetDestroyed(ABrian_A_FinalCharacter* ShootingPlayer) { ServerSetDestroyed(ShootingPlayer); }
bool ADestroyLinkActor::ServerSetDestroyed_Validate(ABrian_A_FinalCharacter* ShootingPlayer) { return true; }
void ADestroyLinkActor::ServerSetDestroyed_Implementation(ABrian_A_FinalCharacter* ShootingPlayer)
{
    if (Role != ROLE_Authority)
        return;

    if (!ShootingPlayer)
        return;

    TArray<AActor*> overlapping;
    GetOverlappingActors(overlapping, this->GetClass());
    MyCollision->SetCollisionProfileName("BlockAll");

    for (AActor* object : overlapping)
    {
        ADestroyLinkActor* destroychecker = Cast<ADestroyLinkActor>(object);
        if ((destroychecker) && (destroychecker->colorSetting == this->colorSetting))
            destroychecker->SetDestroyed(ShootingPlayer);
    }


    if (colorSetting)
    {
        ShootingPlayer->GetController()->PlayerState->Score += 1;
        GetWorld()->GetAuthGameMode<ABrian_A_FinalGameMode>()->CheckNumBlocks();
    }
    this->colorSetting = 0;


    this->Destroy();
}

void ADestroyLinkActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    //TODO: Replicate variables
    //Don't want to replicate to client that owns this weapon
    //DOREPLIFETIME_CONDITION(ASWeapon, HitScanTrace, COND_SkipOwner);
    DOREPLIFETIME(ADestroyLinkActor, colorSetting);
    DOREPLIFETIME(ADestroyLinkActor, MyVisual);
    DOREPLIFETIME(ADestroyLinkActor, MyCollision);
    DOREPLIFETIME(ADestroyLinkActor, ColorList);
}