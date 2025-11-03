// Copyright (c) 2025 Tobias Cutik

#pragma once

#include "CoreMinimal.h"
#include "SimTracks/Simulation/Core/Public/SimulationParameter.h"
#include "SimTracks/Simulation/Core/Public/EvaluationParameter.h"
#include "SimState.generated.h"

USTRUCT()
struct SIMTRACKS_API FSimState
{
    GENERATED_BODY()

public:
    bool bValid = true;       

    TMap<FString, float> SimulationParams;
    TMap<FString, float> EvaluationParams;

    FSimState(TArray<FSimulationParameter> SimulationParams, TArray<FEvaluationParameter> EvaluationParams);
    FSimState() = default;
    virtual ~FSimState();
};