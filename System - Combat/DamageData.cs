using UnityEngine;

/// <summary>
/// IDamagableEntity가 피격당했을 때, 외부 시스템에 이벤트를 전송하기 위한 구조체.
/// 피격자의 스탯이나 View를 관리하는 곳에서도 DamageData를 받아서 적절한 데이터 변경 및 연출을 수행합니다. 
/// </summary>
public struct DamageData {
    public IAttackableCollider attacker;
    public IDamagableEntity hitEntity;
    public AbilityBase hitAbility;
    public Vector2 attackPos;
    public Vector2 attackDirection;
    public float damage;

    public DamageData(IAttackableCollider attacker, IDamagableEntity hitEntity, Vector2 attackPos) {
        this.hitEntity = hitEntity;
        this.attackPos = attackPos;
        this.attacker = attacker;
        attackDirection = attacker.transform.up;
        this.damage = attacker.Damage;
        hitAbility = attacker.ParentAbility;
    }
}
