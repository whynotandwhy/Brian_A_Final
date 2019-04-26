// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Brian_A_FinalGameMode.h"
#include "Brian_A_FinalHUD.h"
#include "Brian_A_FinalCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Public/DestroyLinkActor.h"
#include "UObject/ConstructorHelpers.h"

ABrian_A_FinalGameMode::ABrian_A_FinalGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ABrian_A_FinalHUD::StaticClass();
}

void ABrian_A_FinalGameMode::HandleMatchHasStarted()
{
    Super::HandleMatchHasStarted();

    DelayColoring();

    if (MyScoring)
    {
        auto newWidget = CreateWidget<UUserWidget>(GetWorld(), MyScoring);
        newWidget->AddToViewport();
        ActiveWidgets.Add(newWidget);
    }
}

void ABrian_A_FinalGameMode::CheckNumBlocks()
{
    NumBlocks--;
    if ((NumBlocks > 0) || (!FinalScore))
        return;

    auto newWidget = CreateWidget<UUserWidget>(GetWorld(), FinalScore);
    newWidget->AddToViewport();
    ActiveWidgets.Add(newWidget);
}

void ABrian_A_FinalGameMode::DelayedColoring()
{
    TArray<AActor*> BlocksToSet;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADestroyLinkActor::StaticClass(), BlocksToSet);

    for (AActor* psudoBlock : BlocksToSet)
    {
        ADestroyLinkActor* actualBlock = Cast<ADestroyLinkActor>(psudoBlock);
        if (!actualBlock)
            continue;
        actualBlock->SetColor(FMath::RandRange(1, 3));
        NumBlocks++;
    }
}

void ABrian_A_FinalGameMode::DelayColoring()
{
    FTimerHandle PostBeginPlayDelay;
    GetWorldTimerManager().SetTimer(PostBeginPlayDelay, this, &ABrian_A_FinalGameMode::DelayedColoring, 1.0f, false);
}

