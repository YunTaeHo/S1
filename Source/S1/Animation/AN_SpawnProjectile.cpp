
// Copyright (c) S1

#include "AN_SpawnProjectile.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(AN_SpawnProjectile)

UAN_SpawnProjectile::UAN_SpawnProjectile(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

void UAN_SpawnProjectile::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		AActor* OwnerActor = MeshComp->GetOwner();

		if (BulletFactory)
		{
			// MeshComp에서 월드 가져오기
			UWorld* World = MeshComp->GetWorld();
			if (World)
			{
				FTransform Transform = MeshComp->GetSocketTransform(BoneSocket);

				World->SpawnActor<AS1Bullet>(BulletFactory, Transform.GetLocation(), OwnerActor->GetActorRotation());
			}
		}
	}
}


