// Copyright (c) 2025 Tobias Cutik

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ComboBoxString.h"
#include "Components/EditableTextBox.h"
#include "SimTracks/Simulation/Core/Public/SimulationParameter.h"
#include "Types/SlateEnums.h"
#include "SimulationParamChangerUI.generated.h"

UCLASS(Abstract)
class SIMTRACKS_API USimulationParamChangerUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init(const TArray<FSimulationParameter>& SimulationParams);
	const FString& GetCurrentSimParamName() const;
	float GetCurrentValueInput() const;

private:
	UFUNCTION()
	void OnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
	UFUNCTION()
	void OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

    UPROPERTY(EditAnywhere, meta = (BindWidget))
	UComboBoxString* ParamSelection;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UEditableTextBox* ParamInput;

	TArray<FSimulationParameter> SimulationParameters;

	FString CurrentSimParamName = FString();
	float CurrentValueInput = 0.f;
};