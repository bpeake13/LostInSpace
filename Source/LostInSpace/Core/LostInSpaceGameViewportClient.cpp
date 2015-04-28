// Fill out your copyright notice in the Description page of Project Settings.

#include "LostInSpace.h"
#include "LostInSpaceGameViewportClient.h"





bool ULostInSpaceGameViewportClient::InputKey(FViewport* Viewport, int32 ControllerId, FKey Key, EInputEvent EventType, float AmountDepressed /*= 1.f*/, bool bGamepad /*= false*/)
{
//	ControllerId += bGamepad ? 1 : 0;

	return Super::InputKey(Viewport, ControllerId, Key, EventType, AmountDepressed, bGamepad);
}

bool ULostInSpaceGameViewportClient::InputAxis(FViewport* Viewport, int32 ControllerId, FKey Key, float Delta, float DeltaTime, int32 NumSamples /*= 1*/, bool bGamepad /*= false*/)
{
//	ControllerId += bGamepad ? 1 : 0;

	return Super::InputAxis(Viewport, ControllerId, Key, Delta, DeltaTime, NumSamples, bGamepad);
}
