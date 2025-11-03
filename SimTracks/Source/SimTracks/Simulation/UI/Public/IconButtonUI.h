// Copyright (c) 2025 Tobias Cutik

#pragma once

#include "CoreMinimal.h"
#include "SimTracks/Simulation/UI/Public/ButtonUI.h"
#include "IconButtonUI.generated.h"

UCLASS(Abstract)
class SIMTRACKS_API UIconButtonUI : public UButtonUI
{
	GENERATED_BODY()

public:
	void SwitchActiveTexture();

protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget), BlueprintReadOnly)
	class UImage* TextureTarget;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	UTexture2D* MainTexture;

private:
	bool bAlternativeTextureActive = false;

	UPROPERTY(EditInstanceOnly)
	UTexture2D* AlternativeTexture;
};