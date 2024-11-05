// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/IWidgetFilter.h"

// Add default functionality here for any IIWidgetFilter functions that are not pure virtual.
bool IIWidgetFilter::OnWidgetOpening_Implementation(const FName& WidgetKeyName, EUIAnimType& AnimationIn,
                                                    float& AnimationInSeconds, UWidgetParam* Param)
{
	return true;
}

void IIWidgetFilter::OnWidgetOpen_Implementation(UBaseWidget* Widget, UWidgetParam* Param)
{
}

void IIWidgetFilter::OnWidgetShown_Implementation(UBaseWidget* Widget)
{
}

bool IIWidgetFilter::OnWidgetClosing_Implementation(UBaseWidget* Widget, EUIAnimType& AnimationOut,
                                                    float& AnimationOutSeconds)
{
	return true;
}

void IIWidgetFilter::OnWidgetHidden_Implementation(UBaseWidget* Widget)
{
}

void IIWidgetFilter::OnWidgetClosed_Implementation(UBaseWidget* Widget)
{
}
