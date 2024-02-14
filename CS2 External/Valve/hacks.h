#pragma once
#include <vector>
#include <algorithm>
#include <thread>
#include <iostream>

struct g_hacks {
private:
	Vector_t screen_center;

    std::unique_ptr<CPlayer> GetClosestPlayer() {
        const int maxPlayers = 64;
        std::vector<CPlayer> players;
        std::vector<float> dists;

        for (int i = 0; i < maxPlayers; i++) {
            CPlayer player(i);
            if (player.IsValid()) {
                players.push_back(player);
                Vector_t headpos = player.BoneOrigin(6).W2S();
                float dist = headpos.Dist(screen_center);
                dists.push_back(dist);
            }
        }

        if (!dists.empty()) {
            auto minDist = std::min_element(dists.begin(), dists.end());
            size_t minDistIdx = std::distance(dists.begin(), minDist);

            // Check if players is empty before accessing element
            if (!players.empty()) {
                return std::make_unique<CPlayer>(players[minDistIdx]);
            }
        }

        return nullptr;
    }

public:
	g_hacks(Vector_t screen_center) : screen_center(screen_center) {}
    void DoEsp(CPlayer player, LocalPlayer lp) {
        if (player.Entity() == lp.Entity())
            return;
        if (lp.GetTeam() == player.GetTeam())
            return;
        if (player.GetHealth() <= 0)
            return;

        Vector_t FeetPos = (player.GetOrigin()-Vector_t(0,0,5.f)).W2S();
        Vector_t HeadPos = (player.BoneOrigin(6) + Vector_t(0, 0, 5.f)).W2S();
        if (FeetPos.z <= 0.001f)
            return;
        float dist = std::abs(FeetPos.y - HeadPos.y);
        float widht = dist * 0.3;
        ImGui::GetBackgroundDrawList()->AddRect({ FeetPos.x - widht,FeetPos.y },
            { HeadPos.x + widht,HeadPos.y }, ImColor(255, 255, 255, 255));
    };

    void Aimbot(CPlayer player, LocalPlayer lp, float fov, float smoothness, int Key) {
        std::unique_ptr<CPlayer> closest = GetClosestPlayer();
        if (closest->Entity() != player.Entity())
            return;
        if (player.Entity() == lp.Entity())
            return;
        if (lp.GetTeam() == player.GetTeam())
            return;
        if (player.GetHealth() <= 0)
            return;
        if (lp.GetHealth() <= 0)
            return;

        Vector_t HeadPos = player.BoneOrigin(6).W2S();
        float dist = HeadPos.Dist(screen_center);
        bool spotted = lp.SpotedState() & (1 << player.GetIndex()) || player.SpotedState() & (1 << lp.GetIdx());
        if (GetAsyncKeyState(Key)) {
            if (dist < fov && spotted) {
                mouse_event(MOUSEEVENTF_MOVE,
                    (HeadPos.x - screen_center.x) / smoothness,
                    (HeadPos.y - screen_center.y) / smoothness,
                    0, 0);
            }
            else
                return;
        }
        else
            return;
        // did it like this because wpm = bad
    };

    void TriggerBot(CPlayer player, LocalPlayer lp, int Key) {
        if (player.Entity() == lp.Entity())
            return;
        if (lp.GetTeam() == player.GetTeam())
            return;
        if (player.GetHealth() <= 0)
            return;
        if (lp.GetHealth() <= 0)
            return;

        Vector_t HeadPos = player.BoneOrigin(6).W2S();
        float dist = HeadPos.Dist(screen_center);

        if (GetAsyncKeyState(Key)) {
            if (dist <= 10) {
                mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
                mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            else
                return;
        }
        else
            return;
        // did it like this because wpm = bad
    };
};