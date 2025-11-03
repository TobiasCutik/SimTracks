// Copyright (c) 2025 Tobias Cutik

#pragma once

#include "CoreMinimal.h"
#include "SimTrackEvaluationParams.generated.h"

USTRUCT()
struct SIMTRACKS_API FSimTrackEvaluationParams
{
    GENERATED_BODY()

public:
    // map of checkpoint times and checkpoint evaluation parameters
    TMap<float, TMap<FString, float>> EvaluationParamsOfCheckpoints;
    unsigned int TrackId = 0;

    FSimTrackEvaluationParams() = default;
    ~FSimTrackEvaluationParams() = default;
};