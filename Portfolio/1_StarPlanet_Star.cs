using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Sirenix.OdinInspector;
using DG.Tweening;

public class Star : Player
{

    [TabGroup("Status"), SerializeField] float originOrbitalRadius; //시작 시 공전 궤도 반지름
    [TabGroup("Status"), SerializeField] float radiusChangeSpeed; //프레임당 목표 궤도에 가까워지는 비율 (mix:0 ~ max:1)
    [TabGroup("Status"), SerializeField] float maxAngulerSpeed; //초당 최대 회전각
    [TabGroup("Status"), SerializeField] float minRadius;
    [TabGroup("Status"), SerializeField] float speedReduction = 1f; //각속도 감소 계수. 높을수록 거리에 따른 이동속도 감소폭이 높다.

    [TabGroup("Child Component"), SerializeField] GameObject trailParticle;
    [TabGroup("Child Component"), SerializeField] GameObject boosterTrailParticle;
    [TabGroup("Child Component"), SerializeField] GameObject feverParticle;

    [SerializeField, ReadOnly] float orbitalSpeedFactor = 1f; //각속도 계수. 평상시엔 1이며, 가속 모드일 경우에만 올라간다.
    [SerializeField] float boosterFactor = 1.8f;

    private float currentAngle;
    private float currentRadius;
    private float currentAngulerSpeed;
    private float originOrbitalSpeedFactor;

    private float _targetRadius;
    private float targetRadius
    {
        get => _targetRadius;
        set
        {
            _targetRadius = Mathf.Max(minRadius, value);
        }
    }

    public event Action<float> EventRadiusChange;

    private void Awake()
    {
        targetRadius = originOrbitalRadius;
        if(!trailParticle.activeSelf) trailParticle.SetActive(true);
        if(boosterTrailParticle.activeSelf) boosterTrailParticle.SetActive(false);
        if (feverParticle.activeSelf) feverParticle.SetActive(false);
        
        EventRadiusChange += AngularSpeedChange;

        originOrbitalSpeedFactor = 1f;
        orbitalSpeedFactor = originOrbitalSpeedFactor;
    }

    private void OnDestroy()
    {
        EventRadiusChange -= AngularSpeedChange;
    }

    protected override void Start()
    {
        base.Start();
        EventRadiusChange(Vector3.Distance(transform.position, Vector3.zero));
    }

    public override void Processing()
    {
        RotateMove();
    }

    void RotateMove()
    {
        currentRadius = Vector3.Distance(transform.position, Vector3.zero);
        currentAngle = Mathf.Atan2(transform.position.z, transform.position.x);

        if (targetRadius == currentRadius)
            return;

        //회전속도에 따라 변화할 각도를, 원래의 반지름으로 돌아가는 속도에 따라 변화할 반지름 길이를 연산
        float _targetAngle = currentAngle + (currentAngulerSpeed * orbitalSpeedFactor) * Mathf.Deg2Rad * Time.deltaTime;

        //목표 반지름이 다를 경우 반지름 변화 및 이벤트 발동
        float _targetRadius = currentRadius;
        if (Mathf.Abs(targetRadius - currentRadius) < 0.01f)
        {
            _targetRadius = targetRadius;
            EventRadiusChange(_targetRadius);
        }
        else
        {
            _targetRadius = Mathf.Lerp(currentRadius, targetRadius, radiusChangeSpeed);
            EventRadiusChange(_targetRadius);
        }

        //다음 프레임에 이동할 위치 출력
        float _nextPosX = _targetRadius * Mathf.Cos(_targetAngle);
        float _nextPosY = _targetRadius * Mathf.Sin(_targetAngle);
        Vector3 _nextPos = new Vector3(_nextPosX, transform.position.y, _nextPosY);

        transform.position = _nextPos;
    }

    public void TargetRadiusChange(Vector3 mousePos)
    {
        targetRadius = Vector3.Distance(mousePos, Vector3.zero);
    }

    void AngularSpeedChange(float radius)
    {
        currentAngulerSpeed = ( maxAngulerSpeed * minRadius ) / (minRadius + (radius - minRadius) / speedReduction);
    }

    public void Accelerate()
    {
        orbitalSpeedFactor *= boosterFactor;
        boosterTrailParticle.SetActive(true);
        trailParticle.SetActive(false);
    }
    public void CancelAccelerate()
    {
        orbitalSpeedFactor = originOrbitalSpeedFactor;
        boosterTrailParticle.SetActive(false);
        trailParticle.SetActive(true);
    }

    public void OnFeverTime()
    {
        gameObject.tag = "Fever";
        originOrbitalSpeedFactor = 1.5f;
        orbitalSpeedFactor = 1.5f;
        Accelerate();
        feverParticle.SetActive(true);
        feverParticle.transform.localScale = Vector3.one;
    }
    public void ExitFeverTime()
    {
        DOTween.To(() => originOrbitalSpeedFactor, x => originOrbitalSpeedFactor = x, 1f, 0.5f);
        DOTween.To(() => orbitalSpeedFactor, x => orbitalSpeedFactor = x, 1f, 0.5f)
            .OnComplete(() =>
            {
                CancelAccelerate();
                gameObject.tag = "Star";
            });
        feverParticle.transform.DOScale(0f, 0.5f)
            .OnComplete(() => feverParticle.SetActive(false));
    }
}
