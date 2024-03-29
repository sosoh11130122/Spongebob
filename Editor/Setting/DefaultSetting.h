#pragma once

#include "EngineSetting.h"

class CDefaultSetting :
    public CEngineSetting
{
public:
    CDefaultSetting();
    virtual ~CDefaultSetting();

public:
    virtual void Init();

private:
    void CreateCDO();
    void LoadResource();
    void SetInput();
    void SetCollision();

    // Sound
    void LoadSound();
    void LoadSfx();

    // Player
    void LoadSpongebob();
    void LoadPatrick();
    void LoadSandy();

    // Monster
    void LoadRoboSponge();
    void LoadKingJellyfish();
    void LoadJellyfish();
    void LoadTikis();
    void LoadEnemies();

    // NPC
    void LoadMrKrabs();
    void LoadSquidward();
    void LoadPatric_Npc();
    void LoadBus();
    void LoadInfoSign();
    void LoadStaticNpcs();

    // Object
    void LoadCommonObj();
    void LoadCollectibleItems();
    void LoadBuildings();
    void LoadJellyfishFieldsObj();
    void LoadCBObjects();
    void LoadCBLabObjects();

    // Particle
    void LoadParticle();
};

