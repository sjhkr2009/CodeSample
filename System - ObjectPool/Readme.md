# Simple Object Pooling

Unity 게임 개발을 위한 기본적인 오브젝트 풀링 시스템을 구현합니다.

---
## PoolManager API 목록

- Pool은 내부적으로 관리됩니다. PoolManager를 통해 사용하세요.  
- 커스텀 초기화가 필요하다면 IPoolHandler를 컴포넌트에 상속시켜서 구현하세요.
  - IPoolHandler가 있는 오브젝트는 생성/반환 시 `IPoolHandler.OnInitialize()`, `IPoolHandler.OnRelease()`가 호출됩니다.

### Properties

- `Instance`: `PoolManager`의 싱글톤 인스턴스에 접근합니다. 전역에서 쉽게 오브젝트 풀을 관리할 수 있게 해줍니다.
- `PoolParent`: 모든 오브젝트 풀이 공통으로 사용할 부모 `Transform`을 가져오거나 설정합니다. 이는 오브젝트 풀의 게임 오브젝트를 계층적으로 구성하여 관리하는 데 사용됩니다.

### Methods

#### 기본 기능

| 함수명            | 파라미터 | 반환 타입 | 설명                                                                  |
|----------------|------------------------------------------------------------------------------|-----------|---------------------------------------------------------------------|
| `Get`          | `string name`<br/>`Transform parent = null`<br/>`bool stayTransform = false` | `GameObject` | 지정된 이름의 오브젝트를 풀에서 가져오거나 새로 생성합니다.                                   |
| `Get<T>`       | `string name`<br/>`Transform parent = null`<br/>`bool stayTransform = false` | `T` | Get() 에서 T타입을 GetComponent 해서 반환합니다.                                |
| `Abandon`      | `GameObject item`                                                            | `void` | 사용이 끝난 오브젝트를 풀에 반환합니다.                                              |
| `AbandonAll`   | -                                                                            | `void` | 씬 전환 등의 상황에서, 현재 활성화되어 있는 모든 오브젝트를 각각의 풀에 반환합니다.                    |


#### 확장 기능
| 함수명            | 파라미터 | 반환 타입 | 설명                                                                  |
|----------------|------------------------------------------------------------------------------|-----------|---------------------------------------------------------------------|
| `GetByType<T>` | `Transform parent = null`<br/>`bool stayTransform = false`                   | `T` | Get<T>() 에서 name을 타입명으로 대체합니다. 타입명이 프리팹명과 동일할 때 편리하게 호출하기 위한 함수입니다. |
| `WarmUp`       | `string name`<br/>`int count`                                                     | `int` | 오브젝트가 지정된 개수만큼 풀에 생성되어 있는 것을 보장합니다. 부족할 경우 필요한 만큼 새로 생성합니다.         |
| `ClearPool`    | `string name`<br/>`int remainCount = 0`                                           | `void` | 풀에 들어있는 오브젝트 인스턴스를 파괴합니다. 로드된 프리팹은 유지됩니다.                           |
| `UnloadPool`   | `string name`                                                                | `void` | 풀에 들어있는 모든 인스턴스를 파괴하고, 프리팹을 언로드한 후 풀 정보를 제거합니다.                     |
| `ClearAll`     | -                                                                            | `void` | 모든 풀에 대해 `ClearPool()`을 호출합니다.                                      |
| `UnloadAll`    | -                                                                            | `void` | 모든 풀에 대해 `UnloadPool()`을 호출합니다.                                     |

---
## 사용 예시

```csharp
// 사용을 원하는 곳에서 오브젝트 생성
GameObject obj = PoolManager.Instance.Get("MyPrefab");

// 사용 후 오브젝트 반환
PoolManager.Instance.Abandon(obj);

// 씬 전환 시
PoolManager.Instance.AbandonAll();
```