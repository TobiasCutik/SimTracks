// Copyright (c) 2025 Tobias Cutik

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ComboBoxString.h"
#include "SimTracks/Simulation/Core/Public/EvaluationParameter.h"
#include "EvaluationParamSelectionUI.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNewEvaluationParamSelected);

UCLASS(Abstract)
class SIMTRACKS_API UEvaluationParamSelectionUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init(const TArray<FEvaluationParameter>& EvaluationParams);
	float GetSelectedEvaluationParamPerformance(const TMap<FString, float>& EvaluationParams) const;
	float GetSelectedEvaluationParamColorValue(const TMap<FString, float>& EvaluationParams);

	FOnNewEvaluationParamSelected NewEvaluationParamSelected;

private:
	UFUNCTION()
	void OnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

    UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UComboBoxString* ParamSelection;

	TArray<FEvaluationParameter> EvaluationParameters;
};