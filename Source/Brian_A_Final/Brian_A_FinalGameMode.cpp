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


}

void ABrian_A_FinalGameMode::CheckNumBlocks()
{
    NumBlocks--;
    if ((NumBlocks > 0) || (!FinalScore))
        return;

    for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
        if (Iterator->IsValid())
            AddView(Iterator->Get(), FinalScore);
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

    if (!MyScoring)
        return;

    for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
        if (Iterator->IsValid())
            AddView(Iterator->Get(), MyScoring);
}

void ABrian_A_FinalGameMode::DelayColoring()
{
    FTimerHandle PostBeginPlayDelay;
    GetWorldTimerManager().SetTimer(PostBeginPlayDelay, this, &ABrian_A_FinalGameMode::DelayedColoring, 1.0f, false);
}

void ABrian_A_FinalGameMode::AddView(APlayerController* playercontroler, TSubclassOf<UUserWidget> newWidget)
{
    ABrian_A_FinalCharacter* mycharater = Cast<ABrian_A_FinalCharacter>(playercontroler->GetCharacter());

    if ((mycharater) && (newWidget))
        mycharater->ClientLoadHud(newWidget);
}

