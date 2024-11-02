// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/WidgetUtility.h"

void UWidgetUtility::SetSliderMouseUsesStep(USlider* Slider, bool UsesStep)
{
	if(Slider)
	{
		Slider->MouseUsesStep = UsesStep;
	}
}
