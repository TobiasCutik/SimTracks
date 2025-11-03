// Copyright (c) 2025 Tobias Cutik

#pragma once

#include "CoreMinimal.h"
#include "SimTracks/Simulation/UserOverridables/C++/Public/SimState.h"
#include "Checkpoint.generated.h"

USTRUCT()
struct SIMTRACKS_API FCheckpoint
{
    GENERATED_BODY()

public:
    FCheckpoint() = default;
    FCheckpoint(const std::shared_ptr<const FSimState>& SimState, float SimulationTime);
    ~FCheckpoint();
    
    std::shared_ptr<const FSimState> SimState = nullptr;
    float Time = 0.f;
};