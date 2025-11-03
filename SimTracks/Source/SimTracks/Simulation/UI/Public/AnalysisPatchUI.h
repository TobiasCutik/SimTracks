// Copyright (c) 2025 Tobias Cutik

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "AnalysisPatchUI.generated.h"

UCLASS(Abstract)
class SIMTRACKS_API UAnalysisPatchUI : public UUserWidget
{
	GENERATED_BODY()

public:
	bool WasCreatedAtTime(float Time) const;
	void SetDesiredSize(FVector2D Size);

	float SimulationTime = 0.f;
	float EvaluationParamValue = 0.f;

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Patch;
};