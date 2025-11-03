// Copyright (c) 2025 Tobias Cutik

#pragma once

#include "CoreMinimal.h"
#include "SimTracks/EpidemicSimulation/Public/AgentState.h"
#include "SimTracks/Simulation/UserOverridables/C++/Public/SimState.h"
#include "EpidemicSimState.generated.h"

USTRUCT()
struct SIMTRACKS_API FEpidemicSimState : public FSimState
{
    GENERATED_BODY()

public:
    FEpidemicSimState(const TArray<FSimulationParameter>& SimulationParams, const TArray<FEvaluationParameter>& EvaluationParams);
    FEpidemicSimState() = default;

    // start with zero to change movement also at the very beginning
    float TimeUntilAgentMovementChange = 0.f;

    FRandomStream AgentMovementRandomStream = FRandomStream();
    TArray<FAgentState> AgentStates;
};