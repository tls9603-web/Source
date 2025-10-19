#include "MineItem.h"
#include "Components/SphereComponent.h"
#include "TimerManager.h" // SetTimer 사용을 위해 추가
#include "Kismet/GameplayStatics.h" // ApplyDamage 사용

AMineItem::AMineItem()
{
    ExplosionDelay = 5.0f;
    ExplosionRadius = 300.0f;
    ExplosionDamage = 30.0f;
    ItemType = "Mine";

    ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
    ExplosionCollision->InitSphereRadius(ExplosionRadius);
    ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    ExplosionCollision->SetupAttachment(Scene);
}

void AMineItem::ActivateItem(AActor* Activator)
{
    // 5초 후 폭발 실행
    GetWorld()->GetTimerManager().SetTimer(ExplosionTimerHandle, this, &AMineItem::Explode, ExplosionDelay);
}

void AMineItem::Explode()
{
    TArray<AActor*> OverlappingActors;
    ExplosionCollision->GetOverlappingActors(OverlappingActors);

    for (AActor* Actor : OverlappingActors)
    {
        if (Actor && Actor->ActorHasTag("Player"))
        {
            // 데미지를 발생시켜 Actor->TakeDamage()가 실행되도록 함
            UGameplayStatics::ApplyDamage(
                Actor,              // 데미지를 받을 액터
                ExplosionDamage,    // 데미지 양
                nullptr,            // 데미지를 유발한 주체 (Controller)
                this,               // 데미지를 유발한 오브젝트 (지뢰)
                UDamageType::StaticClass() // 기본 데미지 유형
            );
        }
    }

    // 폭발 이후 지뢰 아이템 파괴
    DestroyItem();
}