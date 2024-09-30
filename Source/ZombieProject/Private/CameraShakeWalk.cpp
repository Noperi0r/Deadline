// Fill out your copyright notice in the Description page of Project Settings.

#include "CameraShakeWalk.h"

UCameraShakeWalk::UCameraShakeWalk()
{
	OscillationDuration = 0.25f;
	OscillationBlendInTime = 0.1f;
	OscillationBlendOutTime = 0.2f;

	RotOscillation.Pitch.Amplitude = 0.6f;
	RotOscillation.Pitch.Frequency = 7.5f;

	RotOscillation.Yaw.Amplitude = 0.4f;
	RotOscillation.Yaw.Frequency = 6.f;

	RotOscillation.Pitch.Amplitude = 0.4f;
	RotOscillation.Pitch.Frequency = 3.f;


}
