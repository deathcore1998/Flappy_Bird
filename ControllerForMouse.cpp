// Fill out your copyright notice in the Description page of Project Settings.


#include "ControllerForMouse.h"

AControllerForMouse::AControllerForMouse()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	DefaultMouseCursor = EMouseCursor::Default;
};