using System;
using System.Collections.Generic;
using UnityEngine;
using Object = UnityEngine.Object;

class Pool : IDisposable {
    public GameObject Origin { get; }
    public string Name { get; }
    private Queue<GameObject> ObjectPool { get; }
    
    /** 풀에 반환된 오브젝트들의 부모로 지정할 오브젝트입니다. */
    public Transform PoolParent { get; set; }
    /** true인 경우 활성화/비활성화 시 IPoolHandler 컴포넌트에 대해 OnInitialize, OnRelease가 호출됩니다. */
    private bool HasHandler { get; }
    
    public Pool(GameObject originPrefab, Transform poolParent, string name) {
        Origin = originPrefab;
        HasHandler = originPrefab.GetComponent<IPoolHandler>() != null;
        ObjectPool = new Queue<GameObject>();
        Name = name;
        PoolParent = poolParent;
    }
    
    /** 프리팹을 인스턴스화하고 이름을 지정합니다. 활성화된 상태로 반환합니다.  */
    private GameObject CreateItem() {
        var obj = Object.Instantiate(Origin, PoolParent);
        obj.name = Name;

        return obj;
    }
    
    /// <summary>
    /// 지정된 개수만큼의 오브젝트가 확보될 때까지 프리팹을 인스턴스화하고, 비활성화 후 풀에 넣습니다.
    /// </summary>
    /// <returns>생성된 오브젝트 개수를 반환합니다.</returns>
    public int Warmup(int count) {
        int createCount = 0;
        while (ObjectPool.Count < count) {
            var obj = CreateItem();
            ReleaseItem(obj);
            createCount++;
        }

        return createCount;
    }
    
    /** 오브젝트를 풀에서 꺼내 가져옵니다. 없을 경우 새로 생성합니다. */
    public GameObject GetItem() {
        GameObject item = (ObjectPool.Count > 0) ? ObjectPool.Dequeue() : CreateItem();
        item.SetActive(true);
        if (HasHandler) item.GetComponent<IPoolHandler>()?.OnInitialize();
        return item;
    }
    
    /** 사용이 끝난 오브젝트를 풀에 반환합니다. */
    public void ReleaseItem(GameObject item) {
        if (HasHandler) item.GetComponent<IPoolHandler>()?.OnRelease();
        item.SetActive(false);
        item.transform.SetParent(PoolParent);
        ObjectPool.Enqueue(item);
    }
    
    /// <summary>
    /// 풀에 들어있는 오브젝트의 인스턴스를 파괴합니다.
    /// </summary>
    /// <param name="remainCount">남겨둘 인스턴스 수를 지정합니다. 기본적으로 모두 제거합니다.</param>
    public int ClearPool(int remainCount = 0) {
        remainCount = remainCount.ClampMin(0);
        int clearCount = 0;
        while (ObjectPool.Count > remainCount) {
            clearCount++;
            
            var obj = ObjectPool.Dequeue();
            if (!obj) continue;
            
            Object.Destroy(obj);
        }
        
        return clearCount;
    }

    public void Dispose() {
        ClearPool(0);
        Resources.UnloadAsset(Origin);
    }
}