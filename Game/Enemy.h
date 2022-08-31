#pragma once

//クラス宣言。
class Player;

class EnemyState;

/// <summary>
/// エネミー。
/// </summary>
class Enemy : public IGameObject
{
public:
	enum EnAnimationClip {						//アニメーション。
		enAnimationClip_Idle,					//待機アニメーション。
		enAnimationClip_Walk,					//歩きアニメーション。
		enAnimationClip_Run,					//走りアニメーション。
		enAnimationClip_Attack,					//攻撃アニメーション。
		enAnimationClip_MagicAttack,			//魔法攻撃アニメーション。
		enAnimationClip_Damage,					//被ダメージアニメーション。
		enAnimationClip_Down,					//ダウンアニメーション。
		enAnimationClip_Num,					//アニメーションの数。
	};
public:
	Enemy();
	~Enemy();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	/// <summary>
	/// 座標を設定する。
	/// </summary>
	/// <param name="position">座標。</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// 座標を取得する。
	/// </summary>
	/// <returns>座標。</returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}
	/// <summary>
	/// HPを設定する。
	/// </summary>
	/// <param name="hp">HP。</param>
	void SetHP(const int hp)
	{
		m_hp = hp;
	}
	/// <summary>
	/// 回転を設定する。
	/// </summary>
	/// <param name="rotation">回転。</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}
	/// <summary>
	/// 大きさを設定する。
	/// </summary>
	/// <param name="scale">大きさ。</param>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}
	/// <summary>
	/// 移動速度を設定する。
	/// </summary>
	/// <param name="velocity">移動速度。</param>
	void SetVelocity(const Vector3& velocity)
	{
		m_velocity = velocity;
	}
	/// <summary>
	/// プレイヤーを探索する。
	/// </summary>
	/// <returns>プレイヤーが見つかったらtrue。</returns>
	const bool IsSearchPlayer() const;
	/// <summary>
	/// アニメーションを再生する。
	/// </summary>
	/// <param name="enAnimationClip">アニメーションクリップ。</param>
	/// <param name="animationSpeed">再生速度。</param>
	void SetPlayAnimation(const EnAnimationClip enAnimationClip,const float animationSpeed);
	const bool IsRangeAttackPlayer()const;
	void MakeAttackCollision();
	const bool GetIsUnderAttack() const
	{
		return m_isUnderAttack;
	}
	void SetIsUnderAttack(const bool isUnderAttack)
	{
		m_isUnderAttack = isUnderAttack;
	}
	const bool IsDamaged();
	void Death();
	const bool HPIsBelowZero() const
	{
		return m_hp <= 0;
	}
	const bool IsPlayAnimation() const
	{
		return m_modelRender.IsPlayingAnimation();
	}
private:
	void ProcessState();
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	void Move();
	void Rotation();
private:
	
	AnimationClip				m_animationClips[enAnimationClip_Num];		//アニメーションクリップ。
	ModelRender					m_modelRender;								//モデルレンダー。
	Vector3						m_position;									//座標。
	Vector3						m_forward = Vector3::AxisZ;					//エネミーの正面ベクトル。
	Quaternion					m_rotation;									//回転。
	Vector3						m_scale = Vector3::One;						//大きさ。
	CharacterController			m_charaCon;									//キャラコン。
	int							m_swordBoneId = -1;							//剣のボーンのID。
	int							m_hp = 0;									//HP。
	Player*						m_player = nullptr;							//プレイヤー。
	Vector3						m_velocity = Vector3::Zero;
	bool						m_isUnderAttack;							//攻撃中。
	std::unique_ptr<EnemyState> m_enemyState = nullptr;
};


