// Copyright (c) 2025 Tobias Cutik


#include "SimTracks/Simulation/UI/Public/SimulationParamChangerUI.h"
#include "SimTracks/Simulation/Utils/Public/TextUtils.h"

void USimulationParamChangerUI::Init(const TArray<FSimulationParameter>& SimulationParams)
{
	ParamInput->OnTextCommitted.AddDynamic(this, &USimulationParamChangerUI::OnTextCommitted);
	ParamSelection->OnSelectionChanged.AddDynamic(this, &USimulationParamChangerUI::OnSelectionChanged);

	SimulationParameters = SimulationParams;

	for (auto& SimulationParam : SimulationParameters)
		ParamSelection->AddOption(SimulationParam.GetName());

	if (!SimulationParameters.IsEmpty())
		ParamSelection->SetSelectedOption(SimulationParameters[0].GetName());
}

void USimulationParamChangerUI::OnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	FSimulationParameter CurrentSimParam = FSimulationParameter();

	for (auto& SimulationParam : SimulationParameters)
	{
		if (SimulationParam.GetName().Equals(SelectedItem))
		{
			CurrentSimParam = SimulationParam;
			break;
		}
	}
	
	ParamInput->SetText(UTextUtils::AsEnNumber(CurrentSimParam.GetStartingValue()));

	CurrentSimParamName = SelectedItem;
	CurrentValueInput = CurrentSimParam.GetStartingValue();
}

void USimulationParamChangerUI::OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	const int SelectedParamIndex = ParamSelection->GetSelectedIndex();
	FSimulationParameter CurrentSimParam = FSimulationParameter();

	if (SelectedParamIndex >= 0)
		CurrentSimParam = SimulationParameters[SelectedParamIndex];

	const FString TextAsString = Text.ToString();

	if (!TextAsString.IsNumeric())
	{
		ParamInput->SetText(UTextUtils::AsEnNumber(CurrentSimParam.GetStartingValue()));
		CurrentValueInput = CurrentSimParam.GetStartingValue();
		return;
	}

	const float InputAsFloat = FCString::Atof(*TextAsString);

	if (InputAsFloat < CurrentSimParam.GetMinValue())
	{
		ParamInput->SetText(UTextUtils::AsEnNumber(CurrentSimParam.GetMinValue()));
		CurrentValueInput = CurrentSimParam.GetMinValue();
	}
	else if (InputAsFloat > CurrentSimParam.GetMaxValue())
	{
		ParamInput->SetText(UTextUtils::AsEnNumber(CurrentSimParam.GetMaxValue()));
		CurrentValueInput = CurrentSimParam.GetMaxValue();
	}
	else
	{
		CurrentValueInput = InputAsFloat;
	}
}

const FString& USimulationParamChangerUI::GetCurrentSimParamName() const
{
	return CurrentSimParamName;
}

float USimulationParamChangerUI::GetCurrentValueInput() const
{
	return CurrentValueInput;
}