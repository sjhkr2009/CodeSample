using UnityEngine;

/// <summary>
/// 공격 가능한 콜라이더를 나타내는 인터페이스. 투사체는 물론 근접/지속/범위 공격도 범위내에 IAttackableCollider를 생성하는 방식으로 구현됩니다. 
/// 유효한 대상과 충돌한 경우 IDamagableEntity.OnAttacked()를 호출하여 대상에게 히트 이벤트를 전달합니다.
/// </summary>
public interface IAttackableCollider {
    /** 이 공격 효과의 위치를 의미합니다. 피격자는 이 transform의 방향에 따라 히트당한 위치를 파악합니다. */
    public Transform transform { get; }
    
    /** 자신을 생성한 능력(무기/스킬 등)을 의미합니다. */
    public AbilityBase ParentAbility { get; }
    
    public float Damage { get; }
}
