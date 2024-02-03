using System.Collections.Generic;
using System.Linq;
using UnityEngine;
using Object = UnityEngine.Object;

public class PoolManager {
    /** 외부에 별도로 싱글톤 클래스를 관리하는 곳이 있다면 Instance는 제거해도 됩니다. */
    public static PoolManager Instance { get; } = new PoolManager();
    
    /** 게임 내 모든 풀의 오브젝트들을 담을 공통 부모 객체를 생성하고 관리합니다. */
    public Transform PoolParent {
        get {
            if (_poolParent) return _poolParent;

            var poolObj = new GameObject("Pool");
            Object.DontDestroyOnLoad(poolObj);
            _poolParent = poolObj.transform;
            return _poolParent;
        }
    }
    private Transform _poolParent;
    
    private HashSet<GameObject> CreatedObjects { get; } = new HashSet<GameObject>();
    private Dictionary<string, Pool> Pools { get; } = new Dictionary<string, Pool>();
    
    private GameObject LoadPrefab(string name) {
        // 프로젝트에 맞는 리소스 로딩방식을 정의하세요.
        var prefab = Resources.Load<GameObject>(name);
        if (!prefab) {
            LogError("PoolManager.LoadPrefab", $"Cannot Find Object '{name}' in Resources!!");
        }

        return prefab;
    }

    private void LogError(string where, string message) {
        // 프로젝트에 맞는 에러 로그 핸들링 방식을 정의하세요.
        Debug.LogError($"[{where}] {message}");
    }
    
    /// <summary>
    /// 오브젝트를 풀에서 가져오거나 새로 생성합니다. 필요에 따라 부모 Transform을 설정합니다.
    /// </summary>
    /// <param name="stayTransform">true일 경우 localPosition, localScale, localRotation을 zero, one, zero로 초기화합니다.</param>
    /// <returns>오브젝트 로딩에 실패하면 null이 반환됩니다.</returns>
    public GameObject Get(string name, Transform parent = null, bool stayTransform = false) {
        if (string.IsNullOrEmpty(name)) {
            LogError("PoolManager.Get", "Object Name Cannot null or Empty!!");
            return null;
        }
        
        GameObject item = null;
        if (Pools.TryGetValue(name, out var poolInfo)) {
            item = poolInfo.GetItem();
        }

        if (!item) {
            var prefab = LoadPrefab(name);
            if (!prefab) return null;

            poolInfo = new Pool(prefab, PoolParent, name);
            Pools.Add(name, poolInfo);
            item = poolInfo.GetItem();
        }

        if (parent) {
            item.transform.SetParent(parent);
            if (!stayTransform) item.transform.ResetTransform();
        }

        CreatedObjects.Add(item);

        return item;
    }
    
    /** 타입 이름을 기준으로 오브젝트를 가져와서 T 타입의 컴포넌트를 반환합니다. 내부적으로 Get(typeof(T).Name)를 호출합니다. */
    public T GetByType<T>(Transform parent = null, bool stayTransform = false) where T : Component {
        return Get<T>(typeof(T).Name, parent, stayTransform);
    }
    /** 오브젝트를 불러와 T타입으로 반환합니다. 해당 컴포넌트가 없으면 생성된 오브젝트를 다시 Pool에 넣고 null을 반환합니다. */
    public T Get<T>(string name, Transform parent = null, bool stayTransform = false) where T : class {
        var item = Get(name, parent, stayTransform);
        if (!item) return null;

        var component = item.GetComponent<T>();
        if (component == null) {
            LogError("PoolManager.Get<T>", $"Item '{name}' doesn't have {typeof(T).Name} Component!!");
            Abandon(item);
        }
        return component;
    }
    
    /** 오브젝트를 비활성화하고 풀에 반환합니다. */
    public void Abandon(GameObject item) {
        CreatedObjects.Remove(item);
        
        if (Pools.TryGetValue(item.name, out var poolInfo)) {
            poolInfo.ReleaseItem(item);
            return;
        }
        
        LogError("PoolManager.Abandon", $"Cannot Find PoolInfo of '{item.name}'!!");
        Object.Destroy(item);
    }
    
    /// <summary>
    /// 오브젝트가 지정된 개수만큼 풀에 생성되어 있게 합니다. 풀이 없으면 프리팹을 로드해서 새로 만들며, count개 이상이 이미 있다면 아무 동작도 하지 않습니다. 
    /// </summary>
    /// <returns>새로 생성된 오브젝트 개수를 반환합니다. 프리팹 로드에 실패했을 경우 -1을 반환합니다.</returns>
    public int WarmUp(string name, int count) {
        if (!Pools.TryGetValue(name, out var poolInfo)) {
            var prefab = LoadPrefab(name);
            if (!prefab) {
                LogError("PoolManager.WarmUp", $"Fail to WarmUp {name} Prefab!");
                return -1;
            }

            poolInfo = new Pool(prefab, PoolParent, name);
            Pools.Add(name, poolInfo);
        }
        
        return poolInfo.Warmup(count);
    }
    
    /// <summary>
    /// 풀에서 오브젝트를 파괴합니다. 
    /// </summary>
    /// <param name="remainCount">남겨둘 오브젝트 수</param>
    public void ClearPool(string name, int remainCount = 0) {
        if (Pools.TryGetValue(name, out var poolInfo)) {
            poolInfo.ClearPool(remainCount);
        }
    }
    
    /** 입력한 풀 정보를 제거합니다. 모든 인스턴스가 파괴되고 프리팹이 언로드됩니다.*/
    public void UnloadPool(string name) {
        if (Pools.TryGetValue(name, out var poolInfo)) {
            poolInfo.Dispose();
            Pools.Remove(name);
        }
    }

    /** 씬 전환 등의 상황에서, 활성화된 모든 오브젝트를 풀에 반환합니다. 이미 파괴된 오브젝트는 무시됩니다. */
    public void AbandonAll() {
        List<GameObject> targets = new List<GameObject>();
        CreatedObjects.ForEach(obj => {
            if (obj) targets.Add(obj);
        });
        
        targets.ForEach(Abandon);
        CreatedObjects.Clear();
    }
    
    /** 각각의 풀에 들어있는 모든 인스턴스를 파괴합니다. 풀에 반환되지 않은 오브젝트는 무시됩니다. */
    public void ClearAll() {
        foreach (var poolName in Pools.Keys) {
            ClearPool(poolName);
        }
    }
    
    /** 풀에 들어있는 모든 인스턴스를 파괴하고, 프리팹을 언로드하며 모든 풀 정보를 제거합니다. */
    public void UnloadAll() {
        var poolNames = Pools.Keys.ToList();
        foreach (var poolName in poolNames) {
            UnloadPool(poolName);
        }
        Pools.Clear();
    }
}
