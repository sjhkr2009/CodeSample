# Combat Interface

## 개요
게임 내 전투에서 공격 및 피격 처리를 위한 인터페이스 및 구조체를 담은 샘플 코드

---
## 구성 요소
- IAttackableCollider
  - 공격을 수행할 수 있는 객체를 나타냅니다. 
  - 모든 공격 효과는 이 인터페이스를 구현한 객체를 생성하여 구현되며, 자신을 생성한 무기나 스킬을 ParentAbility로 가지고 있습니다.
  - 유효한 대상에게 피격 이벤트를 전송합니다.
- IDamagableEntity
  - 피해를 입을 수 있는 객체를 나타냅니다.
  - IAttackableCollider에 의해 OnAttacked가 호출됩니다. 자신의 상태에 따라 피격 결과를 AttackResult로 반환합ㄴ.다.
  - 히트에 성공하면 DamageData를 생성해 외부에 이벤트를 전송합니다.
- DamageData
  - 공격자 정보, 위치, 방향, 피해량 등 공격에 관련된 세부 정보를 담고 있는 구조체입니다.
  - 피격자가 생성하며 외부에 전송되어 스탯 변경, UI 갱신, 피격 이펙트 출력 등의 처리에 사용합니다. 
- AttackResult
  - 공격의 결과를 나타내는 구조체로 피격 처리 후 공격자에게 반환됩니다.
  - 히트 수에 제한이 있거나 피격 위치에 추가 효과를 일으키는 등의 경우, 공격자가 추가적인 작업을 하기 위해 사용합니다.

## 사용 예시
인터페이스나 예시에서 사용중인 다른 클래스(AbilityBase, Projectile 등)들은 ExampleImpl 폴더를 참고하세요.

- 공격자가 공격을 수행하는 예시 코드
```csharp
void OnTriggerEnter2D(Collider2D other) {
    // 충돌로 관통력이 0 미만이 되면 파괴됩니다.
    if (penetration <= 0) return;
    if (!IsValidTarget(other.gameObject)) return;
    
    var damageHandler = other.GetComponent<IDamagableEntity>();
    if (damageHandler == null) return;
    
    var attackPos = other.bounds.ClosestPoint(transform.position);
    var result = damageHandler.OnAttacked(this, attackPos);
    Param.onAttack?.Invoke(attackPos);
    ShowHitEffect();
    
    if (result.isHit) {
        penetration--;
        if (penetration <= 0) PoolManager.Abandon(gameObject);
    }
}
```

- 피격자가 피해를 처리하는 예시 코드
```csharp
public AttackResult OnAttacked(IAttackableCollider attacker, Vector2 attackPos) {
    if (GameManager.IsPause || Status.IsDead) return AttackResult.None;
    if (attacker.Damage <= 0) return AttackResult.Hit;
    var data = new DamageData(attacker, this, attackPos);
    ApplyDamage(data, attackPos);
    return Status.IsDead ? AttackResult.Dead : AttackResult.Hit;
}

void ApplyDamage(DamageData data, Vector2 attackPos) {
    Status.Hp -= data.damage;
    GameBroadcaster.CallEnemyHit(data, Status);
    
    if (Status.Hp <= 0) {
        Dead();
    } else {
        view.OnHit(data);
        moveStrategy.OnHit(data);
    }
}
```