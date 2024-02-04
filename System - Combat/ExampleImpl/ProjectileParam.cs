using System;
using UnityEngine;

/// <summary>
/// 투사체 생성 시 투사체에게 정보를 전송하는 파라미터. 투사체가 아닌 제자리 공격 효과 등에도 응용할 수 있습니다.
/// </summary>
public struct ProjectileParam {
    public AbilityBase ability;
    public IDamagableEntity[] ignoreTargets;
    public Vector3 startPoint;
    public Vector3 direction;
    public Action<Vector3> onAttack;
    public float damage;
    public float speed;
    public float range;
    public float size;
    public float attackInterval;
    public float lifeTime;
    public float startDelayTime;
    public float attackDurationTime;
    public float penetration;
    
    /// <summary>
    /// 현재 위치에서 직선 방향으로 나아가는 투사체. 생성 후 방향을 조정해야 합니다.
    /// </summary>
    public static ProjectileParam CreateStraightDefault(AbilityBase ability, Vector3 startPoint) {
        return new ProjectileParam() {
            ability = ability,
            startPoint = startPoint,
            size = 1f,
            range = 20f,
            lifeTime = 10f,
            penetration = -1,
            speed = 10f
        };
    }
    
    /// <summary>
    /// 움직이지 않고 생성된 자리를 타격하고 사라지는 효과
    /// </summary>
    public static ProjectileParam CreateOnceAttackDefault(AbilityBase ability, Vector3 startPoint) {
        return new ProjectileParam() {
            ability = ability,
            startPoint = startPoint,
            size = 1f,
            range = 1f,
            lifeTime = 1f,
            startDelayTime = 0f,
            attackDurationTime = 2f,
        };
    }
    
    /// <summary>
    /// 일정 시간 유지되어 범위 내 대상들을 지속적으로 타격하는 효과
    /// </summary>
    public static ProjectileParam CreatePeriodicAttackDefault(AbilityBase ability, Vector3 startPoint) {
        return new ProjectileParam() {
            ability = ability,
            startPoint = startPoint,
            size = 1f,
            range = 1f,
            lifeTime = 2f,
            startDelayTime = 0f,
            attackDurationTime = 2f,
            attackInterval = 0.5f
        };
    }
}
