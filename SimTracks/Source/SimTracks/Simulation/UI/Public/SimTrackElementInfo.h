// Copyright (c) 2025 Tobias Cutik

#pragma once

#include "CoreMinimal.h"
#include "SimTrackElementInfo.generated.h"

USTRUCT()
struct SIMTRACKS_API FSimTrackElementInfo
{
    GENERATED_BODY()

public:
    unsigned int TrackId = 0;
    float StartTime = 0.f;
    float EndTime = 0.f;
    unsigned int ChildDepth = 0.f;

    FSimTrackElementInfo() = default;
    ~FSimTrackElementInfo() = default;
};