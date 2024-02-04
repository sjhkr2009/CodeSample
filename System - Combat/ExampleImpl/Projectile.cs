using System.Linq;
using UnityEngine;

/// <summary>
/// 공격 가능한 투사체를 구현한 예시. 유효한 대상에게 히트 이벤트를 전송합니다.
/// </summary>
public class Projectile : GameListenerBehaviour, IAttackableCollider {
    [TagSelector] public string targetTag;

    public ProjectileParam Param { get; protected set; }
    public float Damage => Param.damage;
    public AbilityBase ParentAbility => Param.ability;
    
    public virtual void Initialize(ProjectileParam param) {
        Param = param;
        if (param.size > 0f) {
            transform.localScale = Vector3.one * param.size;
        }
    }

    public virtual bool IsValidTarget(GameObject target) {
        if (string.IsNullOrEmpty(targetTag)) return false;
        if (Param.ignoreTargets != null && Param.ignoreTargets.Any(t => t.gameObject == target)) return false;
        
        return target.CompareTag(targetTag);
    }
    
    protected virtual void OnTriggerEnter2D(Collider2D other) {
        if (!IsValidTarget(other.gameObject)) return;

        var damageHandler = other.GetComponent<IDamagableEntity>();
        var attackPos = other.bounds.ClosestPoint(transform.position);
        damageHandler?.OnAttacked(this, attackPos);
        Param.onAttack?.Invoke(attackPos);
    }

    protected virtual void OnDisable() {
        Param = default;
    }
}
