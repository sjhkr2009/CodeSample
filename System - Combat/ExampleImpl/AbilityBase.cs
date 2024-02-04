/// <summary>
/// 장비, 스킬 등 캐릭터에 장착되는 효과들의 기반 클래스   
/// </summary>
public abstract class AbilityBase {
    public abstract AbilityIndex Index { get; }

    protected AbilityData _data; 
    public AbilityData Data => _data ??= AbilityDataContainer.LoadData(Index);
    public virtual int Level { get; set; }
    public virtual bool InvokeOnHit => false;
    public virtual bool IgnoreKnockBack => false;
    
    /// <summary>
    /// 캐릭터에 이 효과가 장착될 때 호출됩니다.
    /// </summary>
    public virtual void Initialize(AbilityController controller) {
        Level = 1;
    }
    
    /// <summary>
    /// 이 효과를 업그레이드할 때마다 호출됩니다.
    /// </summary>
    public virtual void Upgrade() {
        Level++;
    }
    
    /// <summary>
    /// 이 효과가 장착 해제될 때 호출됩니다.
    /// </summary>
    public virtual void Abandon() { }
    
    /// <summary>
    /// 다른 효과가 장착/해제/업그레이드될 때 호출됩니다. 상호작용이 필요한 경우 changedAbility 상태에 따라 추가 동작을 수행할 때 사용합니다.
    /// </summary>
    public virtual void OnChangeOtherAbility(AbilityBase changedAbility) { }
}
