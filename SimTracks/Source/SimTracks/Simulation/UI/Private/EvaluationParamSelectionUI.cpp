// Copyright (c) 2025 Tobias Cutik


#include "SimTracks/Simulation/UI/Public/EvaluationParamSelectionUI.h"
#include "Kismet/KismetMathLibrary.h"

void UEvaluationParamSelectionUI::Init(const TArray<FEvaluationParameter>& EvaluationParams)
{
	ParamSelection->OnSelectionChanged.AddDynamic(this, &UEvaluationParamSelectionUI::OnSelectionChanged);

	EvaluationParameters = EvaluationParams;

	for (auto& EvaluationParam : EvaluationParameters)
		ParamSelection->AddOption(EvaluationParam.GetName());

	if (!EvaluationParameters.IsEmpty())
		ParamSelection->SetSelectedOption(EvaluationParameters[0].GetName());
}

void UEvaluationParamSelectionUI::OnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	NewEvaluationParamSelected.Broadcast();
}

float UEvaluationParamSelectionUI::GetSelectedEvaluationParamPerformance(const TMap<FString, float>& EvaluationParams) const
{
	const FString SelectedEvaluationParam = ParamSelection->GetSelectedOption();
	return *EvaluationParams.Find(SelectedEvaluationParam);
}

float UEvaluationParamSelectionUI::GetSelectedEvaluationParamColorValue(const TMap<FString, float>& EvaluationParams)
{
	const FString SelectedEvaluationParam = ParamSelection->GetSelectedOption();
	const auto Value = EvaluationParams.Find(SelectedEvaluationParam);

	for (auto& EvaluationParam : EvaluationParameters)
	{
		if (EvaluationParam.GetName().Equals(SelectedEvaluationParam))
		{
			float Result = 0.f;

			if (EvaluationParam.GetBestValue() > EvaluationParam.GetWorstValue())
			{
				Result = UKismetMathLibrary::NormalizeToRange(*Value, EvaluationParam.GetWorstValue(), EvaluationParam.GetBestValue());
			}
			else
			{
				Result = UKismetMathLibrary::NormalizeToRange(*Value, EvaluationParam.GetBestValue(), EvaluationParam.GetWorstValue());
				Result = 1.f - Result;
			}

			return Result;
		}
	}

	return 0.f;
}