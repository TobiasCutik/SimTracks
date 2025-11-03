// Copyright (c) 2025 Tobias Cutik

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Types/SlateEnums.h"
#include "HistoryChangeUI.generated.h"

UCLASS(Abstract)
class SIMTRACKS_API UHistoryChangeUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init(const FString& ChangedSimParamName, float ChangedSimParamValue, const FString& TrackStartTime);

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* SimulationParam;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* ChangedValue;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* BranchingTime;
};