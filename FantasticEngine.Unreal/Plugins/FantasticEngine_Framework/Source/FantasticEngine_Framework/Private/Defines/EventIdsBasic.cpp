// Fill out your copyright notice in the Description page of Project Settings.


#include "Defines/EventIdsBasic.h"

int32 UEventIdsBasic::GET_READY()
{
	return 1;
}

int32 UEventIdsBasic::SHOW_LOADING()
{
	return 2;
}

int32 UEventIdsBasic::LOADING_PROGRESS()
{
	return 3;
}

int32 UEventIdsBasic::HIDE_LOADING()
{
	return 4;
}

int32 UEventIdsBasic::EID_SCREEN_SIZE_CHANGED()
{
	return 5;
}

int32 UEventIdsBasic::EID_ACTOR_GO_DIE()
{
	return 10;
}
