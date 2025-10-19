#include "CoinItem.h"
#include "Engine/World.h"
#include "SpartaGameState.h" // GameState 헤더 추가

ACoinItem::ACoinItem()
{
    // 점수 기본값을 0으로 설정
    PointValue = 0;
    ItemType = "DefaultCoin";
}

void ACoinItem::ActivateItem(AActor* Activator)
{
    if (Activator && Activator->ActorHasTag("Player"))
    {
        if (UWorld* World = GetWorld())
        {
            if (ASpartaGameState* GameState = World->GetGameState<ASpartaGameState>())
            {
                GameState->AddScore(PointValue);
                GameState->OnCoinCollected(); // 코인 수집 알림
            }
        }
        DestroyItem();
    }
}