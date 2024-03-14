// Fill out your copyright notice in the Description page of Project Settings.

 
#include "SWorldUserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogWorldUserWidget, All, All);

void USWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!IsValid(AttachedActor))
	{
		RemoveFromParent();

		UE_LOG(LogWorldUserWidget, Display, TEXT("AttachedActor no longer valid in %s"), *GetName());
		return;
	}
	
	FVector2D ScreenPosition;
	if ( UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation() + WorldOffset, ScreenPosition) )
	{
		const float Scale = UWidgetLayoutLibrary::GetViewportScale(this);
		check(Scale);
		ScreenPosition /= Scale;

		if (ParentSizeBox)
		{
			ParentSizeBox->SetRenderTranslation(ScreenPosition);
		}
	}
}
