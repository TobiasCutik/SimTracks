// Copyright (c) 2025 Tobias Cutik


#include "SimTracks/Simulation/UI/Public/HistoryTrackUI.h"

void UHistoryTrackUI::Init(const FString& TrackName)
{
	TrackNameInput->SetText(FText::FromString(TrackName));

	TrackNameInput->OnTextCommitted.AddDynamic(this, &UHistoryTrackUI::OnTextCommitted);
}

void UHistoryTrackUI::OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	TrackNameChanged.Broadcast(Text);
}