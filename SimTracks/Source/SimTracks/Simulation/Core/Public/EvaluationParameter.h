// Copyright (c) 2025 Tobias Cutik

#pragma once

#include "CoreMinimal.h"
#include "EvaluationParameter.generated.h"

USTRUCT()
struct SIMTRACKS_API FEvaluationParameter
{
    GENERATED_BODY()

private: 
    FString Name;
    float WorstValue = 0.f;
    float BestValue = 1.f;
    float StartingValue = 0.f;

public:
    FString& GetName();
    float GetWorstValue();
    float GetBestValue();
    float GetStartingValue();

    FEvaluationParameter(FString Name, float WorstValue, float BestValue, float StartingValue);
    FEvaluationParameter() = default;
    ~FEvaluationParameter();
};