#include "memory.h"
#include "Vector.h"
#include "SDK/sdk.h"
#include "SDK/utils.h"
class CPlayer {
public:
	CPlayer(int index){
		this->index = index;
		uintptr_t list_entry = interfaces::GetEntityListEntry();
		player = interfaces::mem.Read<uintptr_t>(list_entry + 120 * index);
		playerPawn = interfaces::mem.Read<uintptr_t>(player + offsets::m_hPlayerPawn);
		pCSPlayerPawn = interfaces::mem.Read<uintptr_t>(list_entry + 120 * (playerPawn & 0x1FF));
		m_entitySpottedState = interfaces::mem.Read<EntitySpottedState_t>(pCSPlayerPawn + offsets::m_entitySpottedState);
	}
	uintptr_t GetPawn() {
		return pCSPlayerPawn;
	}
	int GetIndex() {
		return this->index;
	}
	int GetTeam() {
		return interfaces::mem.Read<int>(player + offsets::m_iTeamNum);
	}
	int GetHealth() {
		return interfaces::mem.Read<int>(pCSPlayerPawn + offsets::m_iHealth);
	}
	bool IsValid() {
		return player != 0 && playerPawn != 0 && pCSPlayerPawn != 0;
	}
	Vector_t GetOrigin() {
		return interfaces::mem.Read<Vector_t>(pCSPlayerPawn + offsets::m_vOldOrigin);
	}
	Vector_t BoneOrigin(int index) {
		uintptr_t gamescene = interfaces::mem.Read<uintptr_t>(pCSPlayerPawn + offsets::m_pGameSceneNode);
		uintptr_t bonearray = interfaces::mem.Read<uintptr_t>(gamescene + offsets::m_modelState + offsets::m_boneArray);
		Vector_t position = interfaces::mem.Read<Vector_t>(bonearray + index * 32);
		return position;
	}
	uintptr_t Entity() {
		return this->player;
	}
	uint32_t SpotedState() {
		return m_entitySpottedState.m_bSpottedByMask[0];
	}
private:
	EntitySpottedState_t m_entitySpottedState;
	uintptr_t player;
	uintptr_t playerPawn;
	uintptr_t pCSPlayerPawn;
	int index;
};

class LocalPlayer {
public:
	LocalPlayer() {
		player = interfaces::mem.Read<uintptr_t>(interfaces::client + offsets::dwLocalPlayerController);
		playerPawn = interfaces::mem.Read<uintptr_t>(player + offsets::m_hPlayerPawn);
		uintptr_t list_entry2 = interfaces::GetEntityListEntry();
		pCSPlayerPawn = interfaces::mem.Read<uintptr_t>(list_entry2 + 120 * (playerPawn & 0x1FF));
		m_entitySpottedState = interfaces::mem.Read<EntitySpottedState_t>(pCSPlayerPawn + offsets::m_entitySpottedState);
		for (int i = 0; i < 64; i++) {
			CPlayer* new_player = new CPlayer(i);
			if (new_player->GetPawn() == pCSPlayerPawn) {
				index = new_player->GetIndex();
				delete new_player; 
				break;
			}
			else {
				delete new_player;
				continue;
			}
		}

	}
	int GetTeam() {
		return interfaces::mem.Read<int>(player + offsets::m_iTeamNum);
	}
	int GetHealth() {
		return interfaces::mem.Read<int>(pCSPlayerPawn + offsets::m_iHealth);
	}
	int GetIdx() {
		return index;
	}
	uintptr_t Entity() {
		return this->player;
	}
	uint32_t SpotedState() {
		return m_entitySpottedState.m_bSpottedByMask[0];
	}
private:
	int index;
	EntitySpottedState_t m_entitySpottedState;
	uintptr_t player;
	uintptr_t playerPawn;
	uintptr_t pCSPlayerPawn;
};