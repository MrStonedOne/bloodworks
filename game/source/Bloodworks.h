#pragma once

#include "cGame.h"
#include "cResources.h"
#include "cSound.h"

enum Depths
{
	BACKGROUND = -1000,
	GAME_OBJECTS = 1000,
	MONSTERS = 2000,
	PLAYER = 3000,
	BULLETS = 4000,
	OBJECT_GUI = 5000,
	FOREGROUND = 6000,
	GUI = 7000
};

class cParticle;
class cParticleTemplate;
class cTexturedQuadRenderable;
class Player;
class Monster;
class Bullet;
class Gun;
class MissionController;
class BloodRenderable;
class Bonus;
class Perk;
class cPostProcess;
class cTextRenderable;
class LevelUpPopup;
class ExplosionController;
class DropController;
class MissionController;
class MonsterController;
class BulletController;
class BloodworksLuaWorld;
class LaserTemplate;
class MainMenu;
class OneShotSoundManager;
class OptionsPopup;
class cPersistent;
class ModWindow;
class cSlaveWork;
class BloodworksConfig;
class BloodworksCheats;
class cAnimationTemplate;
class GroundRenderable;
class CollisionController;
class GameObjectTemplate;
class CrashReportWindow;
class CreditsWindow;
class CustomGameWindow;
class PlayerTemplate;
class BloodworksSteam;
class PauseMenu;
class TutorialMenu;

class Bloodworks : public cGame
{
	Player *player;

	CrashReportWindow *crashReporterWindow;

	MissionController *missionController;
	MonsterController *monsterController;
	BulletController *bulletController;
	CollisionController *collisionController;
	ExplosionController *explosionController;
	DropController *dropController;
	BloodworksLuaWorld *luaWorld;
	OneShotSoundManager *oneShotSoundManager;

	BloodRenderable *bloodRenderable;
	cPostProcess *pausePostProcess;
	LevelUpPopup *levelUpPopup;
	OptionsPopup *optionsPopup;
	ModWindow *modWindow;
	CreditsWindow *creditsWindow;
	CustomGameWindow *customGameWindow;
	PauseMenu *pauseMenu;
	TutorialMenu *tutorialMenu;

	cVector<Gun*> guns;
	cVector<Bonus*> bonuses;
	cVector<Bonus*> activeBonuses;
	cVector<Perk*> perks;
	cVector<Perk*> usedPerks;

	GroundRenderable *bg;
	cVector<cTexturedQuadRenderable*> fgs;

	std::unordered_map<std::string, cParticleTemplate*> particles;
	std::unordered_map<std::string, LaserTemplate*> laserTemplates;

	BloodworksConfig* config;

	int nextUniqueId;
	int nextGlobalUniqueId;

	Vec2 mapSize;

	AARect mapRect;

	Vec2 cameraCenterPos;

	float pauseSlowdown;

	bool paused;

	MainMenu *mainMenu;


	float soundSpeed;
	bool soundPaused;

	cVector<cSoundHandle> gameSounds;
	cVector<cParticle*> orphanParticles;

	std::map<std::string, cAnimationTemplate*> animationTemplates;
	std::map<std::string, GameObjectTemplate*> gameObjectTemplates;
	std::map<std::string, PlayerTemplate*> playerTemplates;

	void parseJson(nlohmann::json& j, DirentHelper::File& f, bool loadOnlyModData = false);

	void initImplementation();

	BloodworksSteam* bloodworksSteam;
protected:
	virtual void render() override;
	virtual void tick() override;
	virtual void init() override;

	void tickCamera();
	void tickGameSlowdown();
public:
	Bloodworks();
	virtual ~Bloodworks();

	MonsterController* getMonsterController() const
	{
		return monsterController;
	}

	DropController* getDropController() const
	{
		return dropController;
	}

	BulletController* getBulletController() const
	{
		return bulletController;
	}

	MissionController* getMissionController() const
	{
		return missionController;
	}

	const cVector<Gun*>& Bloodworks::getGuns() const
	{
		return guns;
	}

	const cVector<Bonus*>& Bloodworks::getBonuses() const
	{
		return bonuses;
	}

	const cVector<Perk*>& Bloodworks::getPerks() const
	{
		return perks;
	}

	const cVector<Perk*>& Bloodworks::getActivePerks() const
	{
		return usedPerks;
	}

	Player* getPlayer() const
	{
		return player;
	}

	BloodRenderable* getBloodRenderable();

	cParticleTemplate* getParticleTemplate(const std::string& name) const
	{
		return particles.at(name);
	}

	cAnimationTemplate* getAnimationTemplate(const std::string& name) const
	{
		return animationTemplates.at(name);
	}

	void addDrop(const Vec2& position);
	void addExplosion(const Vec2& pos, float maxScale, float scaleSpeed, int minDamage, int maxDamage, float startTime = 0.0f, bool damagePlayer = false, sol::function onHit = nullptr);

	int getUniqueId()
	{
		if (nextUniqueId < 1000)
		{
			nextUniqueId = 1000;
		}
		return nextUniqueId++;
	}	
	
	int getGlobalUniqueId()
	{
		assert(nextGlobalUniqueId < 1000);
		return nextGlobalUniqueId++;
	}

	const Vec2& getMapSize() const
	{
		return mapSize;
	}

	const AARect& getMapLimits() const
	{
		return mapRect;
	}

	bool isCoorOutside(const Vec2& pos, float radius) const;
	bool isCoorOutside(const Vec2& pos) const;

	void onAddedGunBullet(Gun *gun, Bullet *bullet);

	float getPauseSlowdown() const
	{
		return pauseSlowdown;
	}

	void doPause();
	void doUnpause( bool instant = false );
	cVector<Perk*> getAvailablePerks() const;
	void onPerkUsed(Perk *levelupPerks);
	int onPlayerDamaged(int damage, float dir, sol::table& params);
	void addLaserTemplate(LaserTemplate * laserTemplate);
	bool isLevelUpPopupVisible() const;
	bool isPaused() const
	{
		return paused;
	}

	BloodworksConfig* getConfig()
	{
		return config;
	}

	BloodworksSteam* getSteam() const
	{
		return bloodworksSteam;
	}

	virtual void windowResized(int width, int height);
	void clearMission();
	bool gotoMainMenu();
	bool loadMission(const std::string& mission);
	void onPlayerDied();
	void playSoundAtMap(const Vec2& pos, cSoundSampleShr s, float volume = 1.0f);
	void playSoundAtMap(const Vec2& pos, cSoundSampleWithParams s);
	void playOneShotSound(sol::table& args);
	float getVolumeMultiplier(const Vec2& pos) const;
	void addGameSound(cSoundHandle& handle);
	void setSoundSpeed(float newSoundSpeed);
	float getSoundSpeed() const;
	bool isMissionLoaded() const;
	void onGunReloaded(Gun* gun);
	void onMonsterDied(Monster* monster, float dropChance);
	void showOptions();

	void addOrphanParticle(const cVector<cParticle*> particles)
	{
		orphanParticles.insert(orphanParticles.end(), particles.begin(), particles.end());
	}	

	void addOrphanParticle(cParticle* particle)
	{
		orphanParticles.push_back(particle);
	}
	bool isOptionsVisible() const;
	void onLevelUp();
	void addSlaveWork(cSlaveWork* work);
	void cancelSlaveWork(cSlaveWork* work);
	void showMods();
	void loadMod(const std::string& path, bool loadOnlyModData = false);
	void loadScript(const std::string& path);
	void updateVolume();
	void updateMusicVolume();
	void addToActiveBonuses(Bonus* bonus);
	void removeFromActiveBonuses(Bonus* bonus);
	void onPlayerPickedGun(Gun * gun);
	void onPlayerPickedBonus(Bonus * bonus, const Vec2& pos);
	int onMonsterDamaged(Monster* monster, int damage, const Vec2& dir, sol::table& args);
	CollisionController* getCollisionController() const;
	GameObjectTemplate* getGameObjectTemplate(const std::string& templateName);
	void clear();
	void reload();
	void showCredits();
	void setMainMenuVisible();
	void showCustomGames();
	void updateZoom();
	bool hasCheats();
	void HideGui(bool bHidden);
	bool IsGUIHidden() const;
	void restartMission();
	void startSurvival();
};