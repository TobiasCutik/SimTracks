// Copyright (c) 2025 Tobias Cutik


#include "SimTracks/Simulation/UI/Public/SimTrackElement.h"

void USimTrackElement::CreateUI(UWidget* Owner, TSubclassOf<USimTrackUI> SimTrackUIClass, TSubclassOf<UTrackArrowUI> TrackArrowUIClass, 
	TSubclassOf<UHistoryTrackUI> HistoryTrackUIClass, unsigned int Id, const FString& Name)
{
	SimTrackWidget = CreateWidget<USimTrackUI>(Owner, SimTrackUIClass);
	SimTrackWidget->SetActiveVisually(true);
	SimTrackWidget->SetName(FText::FromString(Name));
	SimTrackWidget->GetOnClickedEvent().AddDynamic(this, &USimTrackElement::OnTrackButtonClicked);

	TrackArrowWidget = CreateWidget<UTrackArrowUI>(Owner, TrackArrowUIClass);
	TrackId = Id;

	HistoryTrackWidget = CreateWidget<UHistoryTrackUI>(Owner, HistoryTrackUIClass);
	HistoryTrackWidget->Init(Name);
	HistoryTrackWidget->TrackNameChanged.AddUObject(this, &USimTrackElement::OnTrackNameChanged);
}

void USimTrackElement::CreateHistoryChangeUI(UWidget* Owner, TSubclassOf<UHistoryChangeUI> HistoryChangeUIClass, const FString& ChangedSimParamName, float ChangedSimParamValue, const FString& TrackStartTime)
{
	HistoryChangeWidget = CreateWidget<UHistoryChangeUI>(Owner, HistoryChangeUIClass);
	HistoryChangeWidget->Init(ChangedSimParamName, ChangedSimParamValue, TrackStartTime);
}

void USimTrackElement::CalculatePadding(float TrackStartTime, float TimelineLengthInSeconds, unsigned int ScreenWidth)
{
	if (!SimTrackWidget || !TrackArrowWidget)
		return;

	const float TimelinePercentage = TrackStartTime / TimelineLengthInSeconds;
	SimTrackWidget->LeftPadding = TimelinePercentage * ScreenWidth;

	const FVector2D ArrowHeadImageSize = TrackArrowWidget->GetArrowHeadImageSize();
	TrackArrowWidget->LeftPadding = SimTrackWidget->LeftPadding - ArrowHeadImageSize.X;
}

bool USimTrackElement::HasId(unsigned int Id) const
{
	return TrackId == Id;
}

USimTrackUI* USimTrackElement::GetSimTrackWidget() const
{
	return SimTrackWidget;
}

UTrackArrowUI* USimTrackElement::GetTrackArrowWidget() const
{
	return TrackArrowWidget;
}

UHistoryTrackUI* USimTrackElement::GetHistoryTrackWidget() const
{
	return HistoryTrackWidget;
}

UHistoryChangeUI* USimTrackElement::GetHistoryChangeWidget() const
{
	return HistoryChangeWidget;
}

void USimTrackElement::OnTrackButtonClicked()
{
	TrackClicked.Broadcast(TrackId);
}

void USimTrackElement::OnTrackNameChanged(FText TrackName)
{
	SimTrackWidget->SetName(TrackName);
}