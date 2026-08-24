#include"pch.h"
#include"Game/Scene/FieldScene.h"

#include"Game/Maths/Collisionall.h"
#include"Game/Party/Monster.h"
#include"Game/Party/Party.h"
#include"Game/Battle/Battle.h"
#include"Game/Enemy/BossManager.h"


static std::vector<Battle::UsedAttackInfo>MakeFieldEffect(Monster::CharacteRistics element, const wchar_t* name)
{
	Battle::UsedAttackInfo info;

	info.element = element;
	info.attackName = name;

	return{ info };
}


FieldScene::FieldScene(BossManager& bossManager, Party& party)
	:m_hitEnemy{ nullptr }
	, m_isBattleRequested{ false }
	, m_isMapActive{ false }
	, m_isMenuActive{ false }
	, m_menuList{ MenuList::Empty }
	, m_menuListSelect{ 0 }
	, m_isCooperatDetailActive{ false }
	, m_CooperatDetailSelect{ 0 }
	, m_cooperatList{ CooperatList::Empty }
	, m_isTreasureOpen{ false }
	, m_breakLevel{}
	, m_bossManager{ bossManager }
	, m_party{ &party }
{

}


FieldScene::~FieldScene()
{
}


void FieldScene::Initialize(InputManager& inputmanager, PlayerManager& playerManager, Map& map)
{
	playerManager.SetImage(m_image);

	//初回だけ登録
	if (m_unlockedSkills.empty())
	{
		//初期スライムの技
		m_unlockedSkills.insert(CooperatList::None);
		m_unlockedSkills.insert(CooperatList::Water);
	}

	m_isMapActive = false;
	m_isMenuActive = false;

	m_isCooperatDetailActive = false;

	if (map.GetCurrentMap() == 0 && map.GetStageStartMap() == 0)
	{
		map.ChangeStage(0);
	}

	m_hitEnemy = nullptr;
	m_isBattleRequested = false;

	//画像配置
	Mposition.x = 10;
	Mposition.y = 600;

	Nposition.x = 50;
	Nposition.y = 650;

	size.x = 50;
	size.y = 50;

	drawMenuBoxPosition.x = 50;
	drawMenuBoxPosition.y = 50;

	drawMenuBoxSize.x = 200;
	drawMenuBoxSize.y = 350;

	drawMenuBoxPosition_1.x = 200;
	drawMenuBoxPosition_1.y = 50;

	drawMenuBoxSize_1.x = 1030;
	drawMenuBoxSize_1.y = 630;
}


void FieldScene::Update(InputManager&inputManager,PlayerManager&playerManager,EnemyManager&enemyManager,Map&map,Battle&battle,Accessory&accessory,Party&party)
{
inputManager.Update();

if(m_isMonsterReplaceSelect)
{
UpdateMonsterReplaceSelect(inputManager);
return;
}
map.Update(inputManager,playerManager);

m_breakLevel=map.GetBreakLevel();

//マップ表示・メニュー表示状態管理

if(inputManager.IsTrigger(KEY_INPUT_Z)&&!m_isMapActive&&!m_isMenuActive)
{
m_isMapActive=true;
}
else if(inputManager.IsTrigger(KEY_INPUT_Z)&&m_isMapActive)
{
m_isMapActive=false;
}


if(inputManager.IsTrigger(KEY_INPUT_X)&&!m_isMenuActive&&!m_isMapActive)
{
m_menuListSelect=0;
m_CooperatDetailSelect=0;

m_menuList=MenuList::Empty;
m_cooperatList=CooperatList::Empty;

m_isMenuActive=true;
}
else if(inputManager.IsTrigger(KEY_INPUT_X)&&m_isMenuActive)
{
m_isMenuActive=false;
}


if(m_isMenuActive)
{
if(inputManager.IsTrigger(KEY_INPUT_UP))
{
if(m_isCooperatDetailActive)
{
m_CooperatDetailSelect--;
}
else 
{
m_menuListSelect--;
}
}


if(inputManager.IsTrigger(KEY_INPUT_DOWN))
{
if(m_isCooperatDetailActive)
{
m_CooperatDetailSelect++;
}
else 
{
m_menuListSelect++;
}
}


if(inputManager.IsTrigger(KEY_INPUT_RETURN))
{
if(m_isCooperatDetailActive)
{
if(m_CooperatDetailSelect>=0&&m_CooperatDetailSelect<static_cast<int>(m_visibleSkills.size()))
{
m_cooperatList=m_visibleSkills[m_CooperatDetailSelect];
}
}
else 
{
switch(m_menuListSelect)
{
case 0:
m_menuList=MenuList::CooperativeMove;
m_isCooperatDetailActive=true;
break;

case 1:
m_menuList=MenuList::PartyCheck;
break;

case 2:
m_menuList=MenuList::ToolCheck;
break;

case 3:
m_menuList=MenuList::OperationInstructions;
break;
}
}
}
else if(inputManager.IsTrigger(KEY_INPUT_BACK))
{
m_menuList=MenuList::Empty;
m_menuListSelect=0;
m_isCooperatDetailActive=false;
}


switch(m_menuList)
{
case MenuList::CooperativeMove:
UpdateCooperativeMove();
break;

case MenuList::PartyCheck:
UpdatePartyCheck();
break;

case MenuList::ToolCheck:
UpdateToolCheck();
break;

case MenuList::OperationInstructions:
UpdateOperationInstructions();
break;

case MenuList::Empty:
break;
}


//メニューから技使用可能

switch(m_cooperatList)
{
case CooperatList::None:
map.NormalBreak(playerManager);

SetAttackEffects(MakeFieldEffect(Monster::CharacteRistics::None,L""));

m_isMenuActive=false;
m_isCooperatDetailActive=false;
m_cooperatList=CooperatList::Empty;
break;


case CooperatList::Fire:
map.FireBreak(playerManager);

SetAttackEffects(MakeFieldEffect(Monster::CharacteRistics::Fire,L""));

m_isMenuActive=false;
m_isCooperatDetailActive=false;
m_cooperatList=CooperatList::Empty;
break;


case CooperatList::Water:
map.WaterBreak(playerManager);

SetAttackEffects(MakeFieldEffect(Monster::CharacteRistics::Water,L""));

m_isMenuActive=false;
m_isCooperatDetailActive=false;
m_cooperatList=CooperatList::Empty;
break;


case CooperatList::Grass:
map.GrassBreak(playerManager);

SetAttackEffects(MakeFieldEffect(Monster::CharacteRistics::Grass,L""));
m_isMenuActive=false;
m_isCooperatDetailActive=false;
m_cooperatList=CooperatList::Empty;
break;


case CooperatList::Soil:
map.SoilBreak(playerManager);

SetAttackEffects(MakeFieldEffect(Monster::CharacteRistics::Soil,L""));

m_isMenuActive=false;
m_isCooperatDetailActive=false;
m_cooperatList=CooperatList::Empty;
break;


case CooperatList::Thunder:
map.ThunderBreak(playerManager);

SetAttackEffects(MakeFieldEffect(Monster::CharacteRistics::Thunder,L""));

m_isMenuActive=false;
m_isCooperatDetailActive=false;
m_cooperatList=CooperatList::Empty;
break;


case CooperatList::Wind:
map.WindBreak(playerManager);

SetAttackEffects(MakeFieldEffect(Monster::CharacteRistics::Wind,L""));

m_isMenuActive=false;
m_isCooperatDetailActive=false;
m_cooperatList=CooperatList::Empty;
break;


case CooperatList::Empty:
break;
}
}


//プレイヤー管理

if(!m_isMapActive&&!m_isMenuActive)
{
playerManager.Update(this,&map,&accessory);
}


if(playerManager.m_oldposition!=playerManager.m_position)
{
playerManager.m_invicible=false;
}


//エネミー管理

enemyManager.Update(map);

Enemy*enemy=
enemyManager.CheckHit(playerManager);


if(!playerManager.m_invicible)
{
if(enemy)
{
playerManager.m_currentposition=playerManager.m_position;

m_hitEnemy=enemy;
m_isBattleRequested=true;

playerManager.m_invicible=true;
}
}


//エフェクト管理

if(m_playEffect)
{
m_effectTimer++;

if(m_effectTimer>=30)
{
m_effectTimer=0;
m_effectIndex++;

if(m_effectIndex>=static_cast<int>(m_attackEffects.size()))
{
m_playEffect=false;
}
}
}


//ブレイクレベル管理

int m_level=m_breakLevel/10;

switch(m_level)
{
case 0:
Level5(enemyManager,map);
break;

case 1:
Level2();
break;

case 2:
Level3();
break;

case 3:
Level4();
break;

case 4:
Level5(enemyManager,map);
break;
}


//LastBoss状態管理

if(m_bossManager.IsAllBossDefeated())
{
}

if(m_bossManager.IsBossDefeated(3))
{
LastBossDefeat();
}
}


void FieldScene::Render(PlayerManager&playerManager,EnemyManager&enemyManager,Map&map,Accessory&accessory,Party&party)
{
map.Render();
//モンスター交換画面
if(m_isMonsterReplaceSelect)
{
RenderMonsterReplaceSelect();
return;
}
playerManager.Render(this,&map,&accessory);

if(!m_isTreasureOpen)
{
enemyManager.Render();
}


//フィールド技エフェクト

if(m_playEffect&&m_effectIndex<static_cast<int>(m_attackEffects.size()))
{
const auto&info=m_attackEffects[m_effectIndex];

SetDrawBlendMode(DX_BLENDMODE_ALPHA,20);

switch(info.element)
{
case Monster::CharacteRistics::Fire:
DrawBox(0,0,1280,720,GetColor(255,80,0),TRUE);
break;

case Monster::CharacteRistics::Water:
DrawBox(0,0,1280,720,GetColor(0,120,255),TRUE);
break;

case Monster::CharacteRistics::Grass:
DrawBox(0,0,1280,720,GetColor(0,200,0),TRUE);
break;

case Monster::CharacteRistics::Thunder:
DrawBox(0,0,1280,720,GetColor(255,255,0),TRUE);
break;

case Monster::CharacteRistics::Wind:
DrawBox(0,0,1280,720,GetColor(180,180,180),TRUE);
break;

default:
break;
}
SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);

DrawString(20,20,info.attackName.c_str(),GetColor(255,255,255));
}


if(m_isMapActive)
{
DrawBox(200,200,900,900,GetColor(0,0,0),TRUE);

DrawString(500,500,L"マップオープン",GetColor(255,255,255),TRUE);
}


if(m_isMenuActive)
{
m_image->DrawCommandbox1(drawMenuBoxPosition,drawMenuBoxSize);
DrawString(500,500,L"メニューオープン",GetColor(0,0,0),TRUE);


float positionx=60.0f;
float positiony=65.0f;
float sizex=100.0f;
float sizey=50.0f;

float cursorY=positiony+50.0f*m_menuListSelect;

DrawBoxAA(positionx,cursorY,positionx+sizex,cursorY+sizey,GetColor(0,255,255),FALSE);


SetFontSize(30);

for(int i=0;i<static_cast<int>(MenuList::Empty);i++)
{
DrawString(80,70+i*50,m_menuText[i],GetColor(255,255,255),TRUE);
}

SetFontSize(10);


switch(m_menuList)
{
case MenuList::CooperativeMove:
RenderCooperativeMove();
break;

case MenuList::PartyCheck:
RenderPartyCheck(party);
break;

case MenuList::ToolCheck:
RenderToolCheck();
break;

case MenuList::OperationInstructions:
RenderOperationInstructions();
break;

case MenuList::Empty:
break;
}
}


if(m_image==nullptr)
{
printfDx(L"m_imageisnullptr!");
return;
}


DrawFormatString(10,200,GetColor(255,255,255),L"BreakLevel:%d",m_breakLevel,TRUE);


m_image->DrawM(Mposition,size);
m_image->DrawN(Nposition,size);


//LastBoss状態管理
if(m_bossManager.IsAllBossDefeated())
{
DrawString(10,30,L"OPEN",GetColor(255,255,255),TRUE);
}
}


void FieldScene::Finalize()
{
}


void FieldScene::UpdateCooperativeMove()
{
}


void FieldScene::UpdatePartyCheck()
{
}


void FieldScene::UpdateToolCheck()
{
}


void FieldScene::UpdateOperationInstructions()
{
}


void FieldScene::RenderCooperativeMove()
{
m_image->DrawCommandbox1(drawMenuBoxPosition_1,drawMenuBoxSize_1);

DrawString(500,500,L"連携技選択オープン",GetColor(0,0,0),TRUE);


float positionx=250.0f;
float positiony=150.0f;
float sizex=200.0f;
float sizey=50.0f;


//表示中の技一覧を作り直す

m_visibleSkills.clear();


auto AddSkill=[&](CooperatList skill)
{
if(HasSkill(skill))
{
m_visibleSkills.push_back(skill);
}
};


AddSkill(CooperatList::None);
AddSkill(CooperatList::Fire);
AddSkill(CooperatList::Water);
AddSkill(CooperatList::Grass);
AddSkill(CooperatList::Soil);
AddSkill(CooperatList::Wind);
AddSkill(CooperatList::Thunder);


if(m_visibleSkills.empty())
{
return;
}


//カーソル補正

if(m_CooperatDetailSelect<0)
{
m_CooperatDetailSelect=static_cast<int>(m_visibleSkills.size())-1;
}


if(m_CooperatDetailSelect>=static_cast<int>(m_visibleSkills.size()))
{
m_CooperatDetailSelect=0;
}


float cursorY=positiony+50.0f*m_CooperatDetailSelect;


DrawBoxAA(positionx,cursorY,positionx+sizex,cursorY+sizey,GetColor(0,0,0),FALSE);


SetFontSize(50);

DrawString(250,50,L"技一覧",GetColor(0,0,0),TRUE);


int y=150;


for(auto skill:m_visibleSkills)
{
const wchar_t*name=L"";


switch(skill)
{
case CooperatList::None:
name=L"無属性";
break;

case CooperatList::Fire:
name=L"火属性";
break;

case CooperatList::Water:
name=L"水属性";
break;

case CooperatList::Grass:
name=L"草属性";
break;

case CooperatList::Soil:
name=L"土属性";
break;

case CooperatList::Wind:
name=L"風属性";
break;

case CooperatList::Thunder:
name=L"雷属性";
break;

default:
break;
}


DrawString(250,y,name,GetColor(0,0,0),TRUE);
y+=50;
}


SetFontSize(10);
}


void FieldScene::RenderPartyCheck(Party&party)
{
m_image->DrawCommandbox1(drawMenuBoxPosition_1,drawMenuBoxSize_1);
SetFontSize(40);


DrawString(250,80,L"仲間",GetColor(0,0,0),TRUE);


int y=150;


for(int i=0;i<party.GetMonsterCount();i++)
{
Monster*monster=party.GetMonster(i);


if(monster==nullptr)
{
continue;
}


//名前
DrawString(300,y,monster->GetName().c_str(),GetColor(0,0,0),TRUE);


//HP
DrawFormatString(600,y,GetColor(0,0,0),L"HP%d/%d",monster->GetCurrentHitPoint(),monster->GetMaxHitPoint());
y+=80;
}


SetFontSize(10);
}


void FieldScene::RenderToolCheck()
{
m_image->DrawCommandbox1(drawMenuBoxPosition_1,drawMenuBoxSize_1);
DrawString(500,500,L"道具一覧オープン",GetColor(0,0,0),TRUE);
}


void FieldScene::RenderOperationInstructions()
{
m_image->DrawCommandbox1(drawMenuBoxPosition_1,drawMenuBoxSize_1);
DrawString(500,500,L"操作説明オープン",GetColor(0,0,0),TRUE);
}


void FieldScene::Level1()
{
}


void FieldScene::Level2()
{
}


void FieldScene::Level3()
{
}


void FieldScene::Level4()
{
}


void FieldScene::Level5(EnemyManager&enemyManager,Map&map)
{
enemyManager.Update(map);
}


//技属性を1つ取得
void FieldScene::LearnSkill(CooperatList skill)
{
m_unlockedSkills.insert(skill);
}


//Monsterが持っている技属性をすべて取得
void FieldScene::LearnMonsterSkills(const Monster&monster)
{
const std::vector<Monster::Attack>&attacks=monster.GetAttacks();


for(const Monster::Attack&attack:attacks)
{
switch(attack.element)
{
case Monster::CharacteRistics::None:
m_unlockedSkills.insert(CooperatList::None);
break;

case Monster::CharacteRistics::Normal:
m_unlockedSkills.insert(CooperatList::None);
break;

case Monster::CharacteRistics::Fire:
m_unlockedSkills.insert(CooperatList::Fire);
break;

case Monster::CharacteRistics::Water:
m_unlockedSkills.insert(CooperatList::Water);
break;

case Monster::CharacteRistics::Grass:
m_unlockedSkills.insert(CooperatList::Grass);
break;

case Monster::CharacteRistics::Soil:
m_unlockedSkills.insert(CooperatList::Soil);
break;

case Monster::CharacteRistics::Thunder:
m_unlockedSkills.insert(CooperatList::Thunder);
break;

case Monster::CharacteRistics::Wind:
m_unlockedSkills.insert(CooperatList::Wind);
break;

case Monster::CharacteRistics::Defense:
//防御は連携技属性として取得しない
break;
}
}
}


bool FieldScene::HasSkill(CooperatList skill)const
{
return m_unlockedSkills.count(skill)>0;
}


void FieldScene::LastBossDefeat()
{
printfDx(L"CollLastBossDefeat");
}


//宝箱
void FieldScene::UpdateTreasureOpen(InputManager&inputManager,PlayerManager&playerManager,Map&map,Accessory&accessory)
{
if(!m_isTreasureOpen)
{
m_isTreasureOpen=true;

result.clear();

int a=3;

std::random_device rd;
std::mt19937 gen(rd());

std::shuffle(accessory.elementTypes.begin(),accessory.elementTypes.end(),gen);


result.assign(accessory.elementTypes.begin(),accessory.elementTypes.begin()+std::min<std::size_t>(a,accessory.elementTypes.size()));
}


if(m_isTreasureOpen)
{
accessory.GetAccessory(Accessory::NOMAL);


if(inputManager.IsTrigger(KEY_INPUT_BACK))
{
accessory.Upgrade(Accessory::NOMAL);

accessory.Upgrade(Accessory::FIRE);

accessory.Upgrade(Accessory::WATER);

accessory.Upgrade(Accessory::GRASS);

accessory.Upgrade(Accessory::SOIL);

accessory.Upgrade(Accessory::THUNDER);

accessory.Upgrade(Accessory::WIND);


m_isTreasureOpen=false;

map.UsedTreasure(playerManager);

playerManager.m_position=playerManager.m_oldposition;
}
}
}


void FieldScene::RenderTreasureOpen(Accessory&accessory)
{
if(m_isTreasureOpen)
{
DrawBox(30,30,1250,690,GetColor(255,255,255),TRUE);


DrawString(200,200,L"TreasureOpen!",GetColor(255,255,0));

for(int i=0;i<static_cast<int>(result.size());i++)
{
DrawFormatString(150+i*400,200,GetColor(0,0,0),L"%d%ls",i+1,accessory.GetElementName(static_cast<Accessory::ElementType>(result[i])));


DrawFormatString(150+i*400,250,GetColor(0,0,0),L"%d",accessory.GetAccessory(static_cast<Accessory::ElementType>(result[i])).level);
}
}
}


//各種
void FieldScene::SetImage(ImageManager*image)
{
m_image=image;
}


bool FieldScene::IsBattleRequested()const
{
return m_isBattleRequested;
}


Enemy*FieldScene::GetHitEnemy()const
{
return m_hitEnemy;
}


void FieldScene::ResetBattleRequest()
{
m_isBattleRequested=false;
m_hitEnemy=nullptr;
}


void FieldScene::SetAttackEffects(const std::vector<Battle::UsedAttackInfo>&effects)
{
m_attackEffects=effects;

m_effectIndex=0;
m_effectTimer=0;

m_playEffect=!effects.empty();
}
void FieldScene::ReceiveJoinedMonster(std::unique_ptr<Monster>monster)
{
if(monster==nullptr)
{
return;
}

//パーティに空きがある
if(m_party->GetMonsterCount()<4)
{
LearnMonsterSkills(*monster);

m_party->AddMonster(std::move(monster));

return;
}

//パーティが4体なら交換画面へ
m_pendingJoinedMonster=std::move(monster);

m_replaceSelect=0;
m_isMonsterReplaceSelect=true;
}

void FieldScene::UpdateMonsterReplaceSelect(InputManager&inputManager)
{
if(m_pendingJoinedMonster==nullptr)
{
m_isMonsterReplaceSelect=false;
return;
}

int count=m_party->GetMonsterCount();

if(count<=0)
{
m_isMonsterReplaceSelect=false;

m_party->AddMonster(std::move(m_pendingJoinedMonster));

return;
}

//上
if(inputManager.IsTrigger(KEY_INPUT_UP))
{
m_replaceSelect--;

if(m_replaceSelect<0)
{
m_replaceSelect=count-1;
}
}

//下
if(inputManager.IsTrigger(KEY_INPUT_DOWN))
{
m_replaceSelect++;

if(m_replaceSelect>=count)
{
m_replaceSelect=0;
}
}

//決定
if(inputManager.IsTrigger(KEY_INPUT_RETURN))
{
Monster*oldMonster=m_party->GetMonster(m_replaceSelect);

if(oldMonster==nullptr)
{
return;
}

//新しいモンスターの技を解放
LearnMonsterSkills(*m_pendingJoinedMonster);

//古いモンスターを削除
m_party->RemoveMonster(m_replaceSelect);

//新しいモンスターを追加
m_party->AddMonster(std::move(m_pendingJoinedMonster));

m_isMonsterReplaceSelect=false;
m_replaceSelect=0;
}

//キャンセル
if(inputManager.IsTrigger(KEY_INPUT_BACK))
{
//仲間にするのをキャンセル
m_pendingJoinedMonster.reset();

m_isMonsterReplaceSelect=false;
m_replaceSelect=0;
}
}

void FieldScene::RenderMonsterReplaceSelect()
{
DrawBox(100,80,1180,650,GetColor(255,255,255),TRUE);

DrawString(180,120,L"パーティがいっぱいです",GetColor(0,0,0),TRUE);

DrawString(180,170,L"入れ替える仲間を選んでください",GetColor(0,0,0),TRUE);

//現在の4体
for(int i=0;i<m_party->GetMonsterCount();i++)
{
Monster*monster=m_party->GetMonster(i);

if(monster==nullptr)
{
continue;
}

int y=250+i*70;

if(i==m_replaceSelect)
{
DrawString(150,y,L"▶",GetColor(255,0,0),TRUE);
}

DrawString(200,y,monster->GetName().c_str(),GetColor(0,0,0),TRUE);
}

//新しく加入するモンスター
if(m_pendingJoinedMonster!=nullptr)
{
DrawString(700,250,L"加入するモンスター",GetColor(0,0,0),TRUE);

DrawString(700,320,m_pendingJoinedMonster->GetName().c_str(),GetColor(0,0,255),TRUE);
}

DrawString(180,580,L"↑↓：選択　Enter：交換　Back：やめる",GetColor(0,0,0),TRUE);
}
