// Copyright (c) 2025 Tobias Cutik

#pragma once

#include "CoreMinimal.h"
#include "AgentState.generated.h"

USTRUCT()
struct SIMTRACKS_API FAgentState
{
    GENERATED_BODY()

public:
    FAgentState() = default;
    ~FAgentState();

    FVector Location = FVector();
    FVector Movement = FVector();
    FRandomStream InfectionRandomStream;

    float InfectionRate = 0.f;
    float RecoveryTime = 0.f;
    float CurrentTimeUntilRecovered = -1.f;

    bool bInfected = false;
    bool bRecovered = false;

    unsigned int ContactsAmount = 0;
};