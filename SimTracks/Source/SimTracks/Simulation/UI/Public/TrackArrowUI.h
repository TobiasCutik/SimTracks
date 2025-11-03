// Copyright (c) 2025 Tobias Cutik

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "TrackArrowUI.generated.h"

UCLASS(Abstract)
class SIMTRACKS_API UTrackArrowUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetArrowLineHeight(float Height);
	FVector2D GetArrowHeadImageSize() const;

	float LeftPadding = 0.f;

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* ArrowLine;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* ArrowHead;
};