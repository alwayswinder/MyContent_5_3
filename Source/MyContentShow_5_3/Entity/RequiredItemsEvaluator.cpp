// Fill out your copyright notice in the Description page of Project Settings.


#include "RequiredItemsEvaluator.h"

bool FRequiredItemsEvaluator::Link(FStateTreeLinker& Linker)
{
	return FMassStateTreeEvaluatorBase::Link(Linker);
}

void FRequiredItemsEvaluator::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FMassStateTreeEvaluatorBase::Tick(Context, DeltaTime);
}
