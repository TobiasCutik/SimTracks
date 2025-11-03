// Copyright (c) 2025 Tobias Cutik

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "Types/SlateEnums.h"
#include "HistoryTrackUI.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTrackNameChanged, FText);

UCLASS(Abstract)
class SIMTRACKS_API UHistoryTrackUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init(const FString& TrackName);

	FOnTrackNameChanged TrackNameChanged;

private:
	UFUNCTION()
	void OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UEditableTextBox* TrackNameInput;
};