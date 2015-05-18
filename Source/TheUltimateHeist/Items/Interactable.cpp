// Fill out your copyright notice in the Description page of Project Settings.

#include "TheUltimateHeist.h"
#include "Interactable.h"

bool AInteractable::CanInteract_Implementation(AActor * Actor)
{
	return true;
}

void AInteractable::Interact(AActor * Actor)
{
	Interacted(Actor);
	OnInteract.ExecuteIfBound();
}

void AInteractable::SetHighlight_Implementation(bool Highlight)
{
	auto AllComponents = GetComponents();
	for (auto Component : AllComponents)
	{
		auto MeshComponent = Cast<UMeshComponent>(Component);
		if (MeshComponent)
		{
			MeshComponent->SetRenderCustomDepth(Highlight);
		}
	}
}