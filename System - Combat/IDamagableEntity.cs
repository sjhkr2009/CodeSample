using UnityEngine;

/// <summary>
/// 플레이어나 적 등 공격을 받을 수 있는 객체를 나타내는 인터페이스. 자신의 현재 상태에 따라 히트 이벤트를 처리하고 공격자에게 AttackResult를 반환합니다.
/// 히트당한 경우 DamageData를 생성해 외부에 이벤트를 전달할 수 있습니다.
/// </summary>
public interface IDamagableEntity {
    public GameObject gameObject { get; }
    public AttackResult OnAttacked(IAttackableCollider attacker, Vector2 attackPos);
}
