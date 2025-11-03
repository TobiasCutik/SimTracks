// Copyright (c) 2025 Tobias Cutik

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SimTracks/Simulation/UI/Public/SimTrackUI.h"
#include "SimTracks/Simulation/UI/Public/TrackArrowUI.h"
#include "HistoryChangeUI.h"
#include "HistoryTrackUI.h"
#include "SimTrackElement.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTrackClicked, unsigned int);

UCLASS()
class SIMTRACKS_API USimTrackElement : public UObject
{
	GENERATED_BODY()

public:
    USimTrackElement() = default;
    void CreateUI(UWidget* Owner, TSubclassOf<USimTrackUI> SimTrackUIClass, TSubclassOf<UTrackArrowUI> TrackArrowUIClass, 
        TSubclassOf<UHistoryTrackUI> HistoryTrackUIClass, unsigned int Id, const FString& Name);
    void CreateHistoryChangeUI(UWidget* Owner, TSubclassOf<UHistoryChangeUI> HistoryChangeUIClass, const FString& ChangedSimParamName, float ChangedSimParamValue, const FString& TrackStartTime);
    void CalculatePadding(float TrackStartTime, float TimelineLengthInSeconds, unsigned int ScreenWidth);
    bool HasId(unsigned int Id) const;

    USimTrackUI* GetSimTrackWidget() const;
    UTrackArrowUI* GetTrackArrowWidget() const;
    UHistoryTrackUI* GetHistoryTrackWidget() const;
    UHistoryChangeUI* GetHistoryChangeWidget() const;

    FOnTrackClicked TrackClicked;

private:
    UFUNCTION()
	void OnTrackButtonClicked();
    UFUNCTION()
	void OnTrackNameChanged(FText TrackName);

    UPROPERTY()
    USimTrackUI* SimTrackWidget = nullptr;
    UPROPERTY()
    UTrackArrowUI* TrackArrowWidget = nullptr;

    UPROPERTY()
	UHistoryTrackUI* HistoryTrackWidget = nullptr;
    UPROPERTY()
	UHistoryChangeUI* HistoryChangeWidget = nullptr;

	unsigned int TrackId = 0;
};