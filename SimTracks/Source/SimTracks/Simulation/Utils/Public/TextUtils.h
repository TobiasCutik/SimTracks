// Copyright (c) 2025 Tobias Cutik

#pragma once

#include "CoreMinimal.h"
#include "TextUtils.generated.h"

UCLASS()
class SIMTRACKS_API UTextUtils : public UObject
{
	GENERATED_BODY()

public:
	static FText AsEnNumber(const float Value);
	static FText AsEnNumber(const double Value);
};