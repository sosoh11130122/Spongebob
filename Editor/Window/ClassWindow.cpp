
#include "ClassWindow.h"
#include "Editor/EditorButton.h"
#include "Editor/EditorSameLine.h"
#include "Editor/EditorLabel.h"
#include "Editor/EditorImage.h"
#include "Editor/EditorText.h"
#include "Editor/EditorInput.h"
#include "Editor/EditorListBox.h"
#include "Editor/EditorComboBox.h"
#include "Editor/EditorTree.h"
#include "PathManager.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "../GameObject\Player.h"
#include "../GameObject\Bullet.h"
#include "../GameObject\Monster.h"
#include "../GameObject\Weapon.h"
#include "ObjectWindow.h"
#include "ComponentWindow.h"
#include "Editor/EditorGUIManager.h"

#include "Component/SpriteComponent.h"
#include "Component/CameraComponent.h"
#include "Component/TargetArm.h"
#include "Component/TileMapComponent.h"
#include "Component/ColliderBox2d.h"
#include "Component/ColliderOBB2d.h"
#include "Component/ColliderSphere2d.h"
#include "Component/ColliderPixel.h"
#include "Component/ColliderCube.h"
#include "Component/ColliderOBB3d.h"
#include "Component/ColliderSphere3d.h"
#include "Component/RigidBody.h"
#include "Component/LightComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/StaticMeshComponent.h"
#include "Component/BillboardComponent.h"
#include "Component/TerrainComponent.h"
#include "Component/ParticleComponent.h"
#include "Component/DecalComponent.h"
#include "Component/NavigationAgent.h"
#include "Component/NavigationAgent3D.h"
#include "Component/BehaviorTree.h"

CClassWindow::CClassWindow()
{
}

CClassWindow::~CClassWindow()
{
}

bool CClassWindow::Init()
{
	CEditorLabel* Label = CreateWidget<CEditorLabel>("ObjectClass");

	Label->SetColor(255, 0, 0, 255);
	Label->SetAlign(0.5f, 0.5f);
	Label->SetSizeY(30.f);

	CEditorSameLine* Line = CreateWidget<CEditorSameLine>("Line");

	CEditorButton* Button = CreateWidget<CEditorButton>("오브젝트생성", 150.f, 30.f);

	Button->SetColor(29, 47, 73, 255);
	Button->SetClickCallback<CClassWindow>(this, &CClassWindow::ObjectCreateCallback);

	m_ObjectList = CreateWidget<CEditorListBox>("ObjectListBox");

	m_ObjectList->SetHideName("ObjectListBox");

	m_ObjectList->SetSize(200.f, 300.f);
	m_ObjectList->SetPageItemCount(6);

	Line = CreateWidget<CEditorSameLine>("Line");

	m_ObjectSelectName = CreateWidget<CEditorInput>("ObjectName");

	m_ObjectSelectName->SetHideName("ObjectName");
	m_ObjectSelectName->SetSize(150.f, 30.f);
	m_ObjectSelectName->AddFlag(ImGuiInputTextFlags_ReadOnly);

	Label = CreateWidget<CEditorLabel>("ComponentClass");

	Label->SetColor(255, 0, 0, 255);
	Label->SetAlign(0.5f, 0.5f);
	Label->SetSizeY(30.f);
	Label->SetSizeX(150.f);

	Line = CreateWidget<CEditorSameLine>("Line");

	Button = CreateWidget<CEditorButton>("컴포넌트생성", 150.f, 30.f);

	Button->SetColor(29, 47, 73, 255);
	Button->SetClickCallback<CClassWindow>(this, &CClassWindow::ComponentCreateCallback);

	m_ObjectList->SetSelectCallback<CClassWindow>(this, &CClassWindow::ObjectClickCallback);


	m_ComponentList = CreateWidget<CEditorListBox>("ComponentListBox");

	m_ComponentList->SetHideName("ComponentListBox");

	m_ComponentList->SetSize(200.f, 300.f);
	m_ComponentList->SetPageItemCount(6);


	m_ComponentList->SetSelectCallback<CClassWindow>(this, &CClassWindow::ComponentClickCallback);

	Line = CreateWidget<CEditorSameLine>("Line");

	m_ComponentSelectName = CreateWidget<CEditorInput>("ComponentName");

	m_ComponentSelectName->SetHideName("ComponentName");
	m_ComponentSelectName->SetSize(150.f, 30.f);
	m_ComponentSelectName->AddFlag(ImGuiInputTextFlags_ReadOnly);

	LoadGameObjectName();

	LoadComponentName();

	return true;
}

void CClassWindow::Update(float DeltaTime)
{
	CEditorWindow::Update(DeltaTime);
}

void CClassWindow::ObjectClickCallback(int Index, const std::string& Item)
{
	m_SelectObjectItem = Item;

	m_ObjectSelectName->SetText(Item.c_str());
}

void CClassWindow::ComponentClickCallback(int Index, const std::string& Item)
{
	m_SelectComponentItem = Item;

	m_ComponentSelectName->SetText(Item.c_str());
}

void CClassWindow::ObjectCreateCallback()
{
	if (m_SelectObjectItem == "")
	{
		return;
	}
	CGameObject* obj = nullptr;
	CObjectWindow* window = CEditorGUIManager::GetInst()->FindEditorWindow<CObjectWindow>("ObjectWindow");
	if (window)
	{
		window->CreateObject(m_SelectObjectItem);
	}
}

void CClassWindow::ComponentCreateCallback()
{
	CScene* Scene = CSceneManager::GetInst()->GetScene();

	CObjectWindow* ObjectWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CObjectWindow>("ObjectWindow");
	CComponentWindow* ComponentWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CComponentWindow>("ComponentWindow");

	CGameObject* SelectObject = ObjectWindow->GetSelectObject();

	if (!SelectObject)
		return;

	CSceneComponent* SelectComponent = (CSceneComponent*)ComponentWindow->GetSelectComponent();

	if (!SelectComponent)
	{
		SelectComponent = SelectObject->GetRootComponent();
	}

	std::string	Name;

	CSceneComponent* NewComponent = nullptr;
	CObjectComponent* ObjectComponent = nullptr;

	if (m_SelectComponentItem == "SpriteComponent")
	{
		Name = "SpriteComponent(SpriteComponent)";
		NewComponent = (CSceneComponent*)SelectObject->CreateComponent<CSpriteComponent>("SpriteComponent");
	}

	else if (m_SelectComponentItem == "SceneComponent")
	{
		Name = "SceneComponent(SceneComponent)";
		NewComponent = SelectObject->CreateComponent<CSceneComponent>("SceneComponent");
	}

	else if (m_SelectComponentItem == "TargetArm")
	{
		Name = "TargetArm(TargetArm)";
		NewComponent = (CSceneComponent*)SelectObject->CreateComponent<CTargetArm>("TargetArm");
	}

	else if (m_SelectComponentItem == "CameraComponent")
	{
		Name = "CameraComponent(CameraComponent)";
		NewComponent = (CSceneComponent*)SelectObject->CreateComponent<CCameraComponent>("CameraComponent");
	}

	else if (m_SelectComponentItem == "TileMapComponent")
	{
		Name = "TileMapComponent(TileMapComponent)";
		NewComponent = (CSceneComponent*)SelectObject->CreateComponent<CTileMapComponent>("TileMapComponent");
	}

	else if (m_SelectComponentItem == "AnimationMeshComponent")
	{
		Name = "AnimationMeshComponent(AnimationMeshComponent)";
		NewComponent = (CSceneComponent*)SelectObject->CreateComponent<CAnimationMeshComponent>("AnimationMeshComponent");
	}

	else if (m_SelectComponentItem == "StaticMeshComponent")
	{
		Name = "StaticMeshComponent(StaticMeshComponent)";
		NewComponent = (CSceneComponent*)SelectObject->CreateComponent<CStaticMeshComponent>("StaticMeshComponent");
	}

	else if (m_SelectComponentItem == "ParticleComponent")
	{
		Name = "ParticleComponent(ParticleComponent)";
		NewComponent = (CSceneComponent*)SelectObject->CreateComponent<CParticleComponent>("ParticleComponent");
	}

	else if (m_SelectComponentItem == "ColliderBox2D")
	{
		Name = "ColliderBox2D(ColliderBox2D)";
		NewComponent = (CSceneComponent*)SelectObject->CreateComponent<CColliderBox2D>("ColliderBox2D");
	}
	else if (m_SelectComponentItem == "ColliderOBB2D")
	{
		Name = "ColliderOBB2D(ColliderOBB2D)";
		NewComponent = (CSceneComponent*)SelectObject->CreateComponent<CColliderOBB2D>("ColliderOBB2D");
	}
	else if (m_SelectComponentItem == "ColliderSphere2D")
	{
		Name = "ColliderSphere2D(ColliderSphere2D)";
		NewComponent = (CSceneComponent*)SelectObject->CreateComponent<CColliderSphere2D>("ColliderSphere2D");
	}
	else if (m_SelectComponentItem == "ColliderPixel")
	{
		Name = "ColliderPixel(ColliderPixel)";
		NewComponent = (CSceneComponent*)SelectObject->CreateComponent<CColliderPixel>("ColliderPixel");
	}
	else if (m_SelectComponentItem == "ColliderCube")
	{
		Name = "ColliderCube(ColliderCube)";
		NewComponent = (CSceneComponent*)SelectObject->CreateComponent<CColliderCube>("ColliderCube");
	}
	else if (m_SelectComponentItem == "ColliderOBB3D")
	{
		Name = "ColliderOBB3D(ColliderOBB3D)";
		NewComponent = (CSceneComponent*)SelectObject->CreateComponent<CColliderOBB3D>("ColliderOBB3D");
	}
	else if (m_SelectComponentItem == "ColliderSphere3D")
	{
		Name = "ColliderSphere3D(ColliderSphere3D)";
		NewComponent = (CSceneComponent*)SelectObject->CreateComponent<CColliderSphere3D>("ColliderSphere3D");
	}
	else if (m_SelectComponentItem == "RigidBody")
	{
		Name = "RigidBody(RigidBody)";
		NewComponent = (CSceneComponent*)SelectObject->CreateComponent<CRigidBody>("RigidBody");
	}

	else if (m_SelectComponentItem == "LightComponent")
	{
		Name = "LightComponent(LightComponent)";
		NewComponent = (CSceneComponent*)SelectObject->CreateComponent<CLightComponent>("LightComponent");
	}

	else if (m_SelectComponentItem == "TerrainComponent")
	{
		Name = "TerrainComponent(TerrainComponent)";
		NewComponent = (CSceneComponent*)SelectObject->CreateComponent<CTerrainComponent>("TerrainComponent");
	}
	else if (m_SelectComponentItem == "BillboardComponent")
	{
		Name = "BillboardComponent(BillboardComponent)";
		NewComponent = (CSceneComponent*)SelectObject->CreateComponent<CBillboardComponent>("BillboardComponent");
	}
	else if (m_SelectComponentItem == "DecalComponent")
	{
		Name = "DecalComponent(DecalComponent)";
		NewComponent = (CSceneComponent*)SelectObject->CreateComponent<CDecalComponent>("DecalComponent");
	}
	else if (m_SelectComponentItem == "NavigationAgent")
	{
		Name = "NavigationAgent(NavigationAgent)";
		ObjectComponent = (CNavigationAgent*)SelectObject->CreateComponent<CNavigationAgent>("NavigationAgent");
	}
	else if (m_SelectComponentItem == "NavigationAgent3D")
	{
		Name = "NavigationAgent3D(NavigationAgent3D)";
		ObjectComponent = (CNavigationAgent3D*)SelectObject->CreateComponent<CNavigationAgent3D>("NavigationAgent3D");
	}

	if (ObjectComponent)
	{
		std::string	ObjName = SelectObject->GetName() + "(" + SelectObject->GetObjectTypeName() + ")";
		ComponentWindow->AddItem((CComponent*)ObjectComponent, Name, ObjName);
	}
	else if (SelectComponent)
	{
		std::string	ParentName = SelectComponent->GetName() + "(" + SelectComponent->GetComponentTypeName() + ")";
		
		SelectComponent->AddChild(NewComponent);

		ComponentWindow->AddItem((CComponent*)NewComponent, Name, ParentName);
	}
	else
	{
		std::string	ObjName = SelectObject->GetName() + "(" + SelectObject->GetObjectTypeName() + ")";

		ComponentWindow->AddItem((CComponent*)NewComponent, Name, ObjName);
	}
}

void CClassWindow::LoadGameObjectName()
{
	const PathInfo* Info = CPathManager::GetInst()->FindPath(ROOT_PATH);

	char	Path[MAX_PATH] = {};

	strcpy_s(Path, Info->PathMultibyte);

	int	Length = (int)strlen(Path);

	// Root Path에서 Bin\을 지워준다.
	for (int i = Length - 2; i >= 0; --i)
	{
		if (Path[i] == '/' || Path[i] == '\\')
		{
			memset(&Path[i + 1], 0, sizeof(TCHAR) * (Length - i - 1));
			break;
		}
	}

	char	Directory[MAX_PATH] = {};

	strcpy_s(Directory, Path);

	// Editor의 GameObject폴더에 있는 파일을 읽어온다.
	strcat_s(Directory, "Include/GameObject/");

	for (const auto& file : std::filesystem::recursive_directory_iterator(Directory))
	//for (const auto& file : std::filesystem::directory_iterator(Directory))
	{
		char	Name[64] = {};
		char	FullPath[MAX_PATH] = {};
		char	Ext[_MAX_EXT] = {};

		strcpy_s(FullPath, file.path().generic_string().c_str());

		_splitpath_s(FullPath, nullptr, 0, nullptr, 0, Name, 64, Ext, _MAX_EXT);

		if (strcmp(Ext, ".cpp") == 0|| Ext[0] == '\0')
			continue;

		m_ObjectList->AddItem(Name);
	}

	Length = (int)strlen(Path);

	// Root Path에서 Bin\을 지워준다.
	for (int i = Length - 2; i >= 0; --i)
	{
		if (Path[i] == '/' || Path[i] == '\\')
		{
			memset(&Path[i + 1], 0, sizeof(TCHAR) * (Length - i - 1));
			break;
		}
	}

	memset(Directory, 0, MAX_PATH);
	strcpy_s(Directory, Path);

	// Editor의 GameObject폴더에 있는 파일을 읽어온다.
	strcat_s(Directory, "Engine/Include/GameObject/");

	for (const auto& file : std::filesystem::directory_iterator(Directory))
	{
		char	Name[64] = {};
		char	FullPath[MAX_PATH] = {};
		char	Ext[_MAX_EXT] = {};

		strcpy_s(FullPath, file.path().generic_string().c_str());

		_splitpath_s(FullPath, nullptr, 0, nullptr, 0, Name, 64, Ext, _MAX_EXT);

		if (strcmp(Ext, ".cpp") == 0)
			continue;

		m_ObjectList->AddItem(Name);
	}
}

void CClassWindow::LoadComponentName()
{
	const PathInfo* Info = CPathManager::GetInst()->FindPath(ROOT_PATH);

	char	Path[MAX_PATH] = {};

	strcpy_s(Path, Info->PathMultibyte);

	int	Length = (int)strlen(Path);

	// Root Path에서 Bin\을 지워준다.
	for (int i = Length - 2; i >= 0; --i)
	{
		if (Path[i] == '/' || Path[i] == '\\')
		{
			memset(&Path[i + 1], 0, sizeof(TCHAR) * (Length - i - 1));
			break;
		}
	}
	Length = (int)strlen(Path) - 3;
	for (size_t i = Length; i > 0; --i)
	{
		if (Path[i] == '/' || Path[i] == '\\')
		{
			memset(&Path[i + 1], 0, sizeof(TCHAR) * (Length - i - 1));
			break;
		}
	}
	char	Directory[MAX_PATH] = {};

	strcpy_s(Directory, Path);

	// Editor의 GameObject폴더에 있는 파일을 읽어온다.
	strcat_s(Directory, "Engine/Include/Component/");

	for (const auto& file : std::filesystem::directory_iterator(Directory))
	{
		char	Name[64] = {};
		char	FullPath[MAX_PATH] = {};
		char	Ext[_MAX_EXT] = {};

		strcpy_s(FullPath, file.path().generic_string().c_str());

		_splitpath_s(FullPath, nullptr, 0, nullptr, 0, Name, 64, Ext, _MAX_EXT);

		if (strcmp(Ext, ".cpp") == 0)
			continue;

		else if (strcmp(Name, "Transform") == 0 ||
				 strcmp(Name, "Transform2D") == 0 ||
				 strcmp(Name, "Collider") == 0 ||
				 strcmp(Name, "Collider2D") == 0 ||
				 strcmp(Name, "Collider3D") == 0 ||
				 strcmp(Name, "Component") == 0 ||
				 strcmp(Name, "PrimitiveComponent") == 0 ||
				 strcmp(Name, "ObjectComponent") == 0 ||
				 strcmp(Name, "Tile") == 0)
		{
			continue;
		}
		m_ComponentList->AddItem(Name);
	}
}
