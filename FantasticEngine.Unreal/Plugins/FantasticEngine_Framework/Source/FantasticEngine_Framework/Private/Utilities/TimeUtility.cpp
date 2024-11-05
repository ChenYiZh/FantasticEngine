// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/TimeUtility.h"

FTimespan UTimeUtility::Substract(const FDateTime& A, const FDateTime& B)
{
	return A - B;
}
