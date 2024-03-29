#include "SpongebobMissile.h"
#include "Input.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/ColliderOBB3D.h"
#include "Component/TargetArm.h"
#include "Component/CameraComponent.h"
#include "Component/BillboardComponent.h"
//#include "Component/RigidBody.h"
#include "Scene/Scene.h"
#include "Scene/NavigationManager3D.h"

CSpongebobMissile::CSpongebobMissile()
	: m_Speed(1000.f)
	, m_Angle(0.f)
{
	SetTypeID<CSpongebobMissile>();
	m_ObjectTypeName = "SpongebobMissile";
}

CSpongebobMissile::CSpongebobMissile(const CSpongebobMissile& obj)
	: CGameObject(obj)
	, m_Speed(obj.m_Speed)
	, m_Angle(0.f)
{
	m_Mesh = (CAnimationMeshComponent*)FindComponent("Mesh");
	m_Body = (CColliderOBB3D*)FindComponent("Body");
	m_Arm = (CTargetArm*)FindComponent("Arm");
	m_Camera = (CCameraComponent*)FindComponent("Camera");
	m_Crosshair = (CBillboardComponent*)FindComponent("Crosshair");
}

CSpongebobMissile::~CSpongebobMissile()
{
}

void CSpongebobMissile::Destroy()
{
	CGameObject::Destroy();
	m_Scene->GetPlayerObject()->Reset();
}

void CSpongebobMissile::Start()
{
	CGameObject::Start();
	m_Scene->GetCameraManager()->SetCurrentCamera(m_Camera);
}

bool CSpongebobMissile::Init()
{
	CGameObject::Init();
	SetLifeTime(10.f);

	m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");
	m_Body = CreateComponent<CColliderOBB3D>("Body");
	m_Arm = CreateComponent<CTargetArm>("Arm");
	m_Camera = CreateComponent<CCameraComponent>("Camera");
	m_Crosshair=CreateComponent<CBillboardComponent>("Crosshair");

	m_Mesh->AddChild(m_Body);
	m_Mesh->AddChild(m_Arm);
	m_Arm->AddChild(m_Camera);
	m_Mesh->AddChild(m_Crosshair);

	m_Mesh->SetMesh("SpongebobMissile");
	m_Mesh->SetPivot(0.5f, 0.5f);
	m_Mesh->SetMaterial(0, "Bubble");
	m_Mesh->GetMaterial(0)->SetRenderState("AlphaBlend");

	m_Body->SetCollisionProfile("PlayerAttack");
	m_Body->SetCollisionCallback<CSpongebobMissile>(ECollision_Result::Collision, this, &CSpongebobMissile::CollisionOn);

	m_Camera->SetInheritRotX(true);
	m_Camera->SetInheritRotY(true);

	m_Crosshair->SetRelativePosition(-100.f, 0.f, 0.f);
	m_Crosshair->SetInheritRotX(true);
	//m_Crosshair->SetInheritRotY(true);
	//m_Crosshair->SetInheritRotZ(true);
	m_Crosshair->GetMaterial(0)->SetTexture(0, 0, (int)EShaderBufferType::Pixel, "Crosshair"
		, TEXT("UI\\cruise_missle_crosshair.tga"));
	m_Crosshair->SetRelativeScale(100.f, 100.f);

	m_Anim = m_Mesh->SetAnimation<CAnimation>("SpongebobMissileAnimation");
	m_Anim->AddAnimation("MissileStart", "SpongebobMissile_Start", 1.f, 1.f, false);
	m_Anim->SetCurrentEndFunction<CSpongebobMissile>("MissileStart", this, &CSpongebobMissile::ResetIdle);
	m_Anim->AddAnimation("MissileIdle", "SpongebobMissile_Idle", 1.f, 1.f, true);
	return true;
}

void CSpongebobMissile::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
	Vector2 mouseMove = CInput::GetInst()->GetMouseMove() * m_Camera->GetCameraSpeed() * g_DeltaTime*0.2f;
	mouseMove.x = m_Camera->GetCameraHorizon() ? mouseMove.x : -mouseMove.x;
	mouseMove.y = m_Camera->GetCameraVertical() ? mouseMove.y : -mouseMove.y;
	m_Arm->AddRelativeRotationY(mouseMove.x);
	m_Arm->AddRelativeRotationX(mouseMove.y);
	if (m_Arm->GetRelativeRot().x > 45.f)
	{
		m_Arm->SetRelativeRotationX(45.f);
	}
	else if (m_Arm->GetRelativeRot().x < -45.f)
	{
		m_Arm->SetRelativeRotationX(-45.f);
	}
	m_Angle = m_Arm->GetRelativeRot().y;

	CNavigationManager3D* Nav = (CNavigationManager3D*)m_Scene->GetNavigationManager();
	float Y = Nav->GetHeight(GetWorldPos());
	if (Y != FLT_MAX && GetWorldPos().y - Y < 0.f)
	{
		Destroy();
	}
}

void CSpongebobMissile::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
	if (m_Anim->GetCurrentAnimationName() == "MissileIdle")
	{
		AddWorldPosition(sinf(DegreeToRadian(m_Angle)) * m_Speed * DeltaTime,
						 tanh(DegreeToRadian(m_Arm->GetRelativeRot().x)) * -m_Speed *2.f* DeltaTime,
						 cosf(DegreeToRadian(m_Angle)) * m_Speed * DeltaTime);
		SetRelativeRotationY(m_Angle - 270.f);
	}
}

CSpongebobMissile* CSpongebobMissile::Clone() const
{
	return new CSpongebobMissile(*this);
}

void CSpongebobMissile::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CSpongebobMissile::Load(FILE* File)
{
	CGameObject::Load(File);
}

void CSpongebobMissile::SetAngle(float f)
{
	SetWorldRotationY(f - 90.f);
	AddWorldPosition(sinf(DegreeToRadian(f - 135.f)) * 100.f, 50.f, cosf(DegreeToRadian(f - 135.f)) * 100.f);
	m_Angle = f - 180.f;
	m_Arm->AddRelativeRotationY(m_Angle);
}

void CSpongebobMissile::ResetIdle()
{
	m_Anim->ChangeAnimation("MissileIdle");
}

void CSpongebobMissile::CollisionOn(const CollisionResult& result)
{
	std::string name = result.Dest->GetCollisionProfile()->Name;
	if (name == "Monster")
	{
		result.Dest->GetOwner()->InflictDamage(1);
	}
	Destroy();
}