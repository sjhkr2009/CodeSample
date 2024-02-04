/// <summary>
/// IDamagableEntity가 히트 이벤트를 받으면, 자신의 상태에 따라 히트 처리를 하고 공격자에게 결과를 반환합니다.
/// 공격자가 타겟 수 제한이 있는 투사체거나 사망 시 추가 효과를 퍼뜨리는 등의 경우, 히트 결과에 따라 추가 처리를 할 수 있습니다. 
/// </summary>
public struct AttackResult {
    public bool isHit;
    public bool isDead;

    public AttackResult(bool isHit, bool isDead) {
        this.isHit = isHit;
        this.isDead = isDead;
    }

    public static AttackResult None => new AttackResult(false, false);
    public static AttackResult Hit => new AttackResult(true, false);
    public static AttackResult Dead => new AttackResult(true, true);
}
