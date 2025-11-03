// Copyright (c) 2025 Tobias Cutik

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SimTracks/Simulation/UI/Public/SimTracksUI.h"
#include "SimTracksUIWrapper.generated.h"

UCLASS(Abstract)
class SIMTRACKS_API USimTracksUIWrapper : public UUserWidget
{
	GENERATED_BODY()

public:
	USimTracksUI* GetSimTracksUI() const;

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class USimTracksUI* SimTracksUI;
};