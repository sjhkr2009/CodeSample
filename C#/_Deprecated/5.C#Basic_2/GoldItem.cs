﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GoldItem : MonoBehaviour, IItem
{
    //플레이어의 골드를 증가시키는 함수. 

    public int goldAmount = 100;

    public void Use()
    {
        Debug.Log("골드를 얻었다!");

        Player player = FindObjectOfType<Player>(); //씬 상에 존재하는 모든 오브젝트를 검색해서 'Player' 컴포넌트가 있는 오브젝트를 찾아 가져온다.
        player.gold += goldAmount;

        gameObject.SetActive(false);
    }
}
