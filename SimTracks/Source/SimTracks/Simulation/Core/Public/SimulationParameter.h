// Copyright (c) 2025 Tobias Cutik

#pragma once

#include "CoreMinimal.h"
#include "SimulationParameter.generated.h"

USTRUCT()
struct SIMTRACKS_API FSimulationParameter
{
    GENERATED_BODY()

private:
    FString Name;
    float MinValue = 0.f;
    float MaxValue = 1.f;
    float StartingValue = 0.5f;

public:
    FString& GetName();
    float GetMinValue();
    float GetMaxValue();
    float GetStartingValue();
      
    FSimulationParameter(FString Name, float MinValue, float MaxValue, float StartingValue);
    FSimulationParameter() = default;
    ~FSimulationParameter();
};