// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Blueprint/UserWidget.h"
#include "Brian_A_FinalGameMode.generated.h"

UCLASS(minimalapi)
class ABrian_A_FinalGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ABrian_A_FinalGameMode();
    virtual void HandleMatchHasStarted() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<UUserWidget> MyScoring;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<UUserWidget> FinalScore;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<UUserWidget*> ActiveWidgets;

    int NumBlocks;

    void CheckNumBlocks();
    void DelayedColoring();
    void DelayColoring();
};



