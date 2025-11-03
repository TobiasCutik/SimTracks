// Copyright (c) 2025 Tobias Cutik

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SimTracks/Simulation/UI/Public/SimTracksUI.h"
#include "SimTracksController.generated.h"

UCLASS()
class SIMTRACKS_API ASimTracksController : public APlayerController
{
	GENERATED_BODY()

public:
	void SetupInput(USimTracksUI* SimTracksWidget);
};
