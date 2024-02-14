#pragma once
#include <cstdint>
#include "../memory.h"
#include "sdk.h"

namespace offsets {
	constexpr std::ptrdiff_t dwEntityList = 0x18B1FE8;
	constexpr std::ptrdiff_t dwLocalPlayerController = 0x1901948;
	constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x17272E8;
	constexpr std::ptrdiff_t dwViewMatrix = 0x19112D0;
	constexpr std::ptrdiff_t m_hPlayerPawn = 0x7E4;
	constexpr std::ptrdiff_t m_iHealth = 0x334;
	constexpr std::ptrdiff_t m_iTeamNum = 0x3CB;
	constexpr std::ptrdiff_t m_vOldOrigin = 0x127C;
	constexpr std::ptrdiff_t m_entitySpottedState = 0x1698;
	constexpr std::ptrdiff_t m_pGameSceneNode = 0x318;
	constexpr std::ptrdiff_t m_modelState = 0x160;
	constexpr std::ptrdiff_t m_boneArray = 0x80;
}

struct CMatrix4x4 {
	float* operator[](int i) {
		return data[i];
	}
	const float* operator[](int i) const noexcept {
		return data[i];
	}

	float data[4][4];
};

namespace interfaces {
	Memory mem("cs2.exe");
	uintptr_t client = mem.GetModuleAddress("client.dll");
	uintptr_t dwEntityList = mem.Read<uintptr_t>(client + offsets::dwEntityList);
	CMatrix4x4 dwViewMatrix = mem.Read<CMatrix4x4>(client + offsets::dwViewMatrix);
	void UpdateMatrix() {
		dwViewMatrix = mem.Read<CMatrix4x4>(client + offsets::dwViewMatrix);
	}
	void UpdateEntityList() {
		dwEntityList = mem.Read<uintptr_t>(client + offsets::dwEntityList);
	}
	uintptr_t GetEntityListEntry() {
		return mem.Read<uintptr_t>(dwEntityList + 0x10);
	}
}