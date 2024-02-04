using UnityEngine;

/// <summary>
/// 지속적으로 가까운 적을 향해 직선으로 날아가는 투사체를 발사하는 무기
/// </summary>
public class FireBulletWeapon : AbilityBase, IWeaponAbility {
    public override AbilityIndex Index => AbilityIndex.WeaponAutoGun;
    
    public string bulletPrefabName = "Bullet01";

    public override bool InvokeOnHit => true;
    
    public float Damage { get; set; }
    public float AttackInterval { get; set; }
    public float AttackRange { get; set; }
    public float BulletSpeed { get; set; }
    public float Penetration { get; set; }

    protected float fireTimer;
    protected Scanner scanner;
    protected Transform fireTransform;
    
    public override void Initialize(AbilityController controller) {
        base.Initialize(controller);

        SetDataByLevel();
        scanner = controller.Scanner;
        fireTransform = controller.FireTransform;
    }

    public void OnEveryFrame(float deltaTime) {
        var elapsedTime = deltaTime * GameManager.Player.Status.AttackSpeed;

        fireTimer += elapsedTime;

        if (fireTimer > AttackInterval) {
            fireTimer = 0f;
            Fire();
        }
    }

    public override void OnChangeOtherAbility(AbilityBase changedAbility) {
    }

    public override void Upgrade() {
        base.Upgrade();
        SetDataByLevel();
    }
    
    private void SetDataByLevel() {
        Damage = Data.GetValue(AbilityValueType.Damage, Level);
        AttackInterval = Data.GetValue(AbilityValueType.AttackInterval, Level);
        AttackRange = Data.GetValue(AbilityValueType.AttackRange, Level);
        Penetration = Data.GetIntValue(AbilityValueType.Penetration, Level);
        BulletSpeed = Data.GetValue(AbilityValueType.ObjectSpeed, Level);
    }
    
    protected virtual void Fire() {
        if (!scanner.NearestTarget) return;
        
        var targetPos = scanner.NearestTarget.position;
        var dir = (targetPos - fireTransform.position).normalized;

        ProjectileSpawner.Spawn(bulletPrefabName, CreateParam(fireTransform.position, dir));
    }

    public ProjectileParam CreateParam(Vector2 startPos, Vector3 direction) {
        var param = ProjectileParam.CreateStraightDefault(this, startPos);
        param.damage = Damage * GameManager.Player.Status.AttackPower;
        param.range = AttackRange;
        param.penetration = Penetration;
        param.direction = direction;
        param.speed = BulletSpeed;

        return param;
    }
}
