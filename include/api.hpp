#pragma once

#include <string>
#include <vector>
#include <map>
#include <optional>

#include "nlohmann/json.hpp"
#include <stack>
#include <openssl/evp.h>

using json = nlohmann::ordered_json;

#if defined(__GNUC__) || defined(__clang__)
#define LWCLIENT_EXPORT __attribute__((visibility("default")))
#else
#define LWCLIENT_EXPORT
#endif

struct EventHandlerInfo {
    std::string event_name;
    void* handler_address;
    std::string module_id;
    long long timestamp;
};

namespace Config
{
    LWCLIENT_EXPORT std::string get(const std::string &key);
    LWCLIENT_EXPORT void set(const std::string &key, const std::string &value);
    LWCLIENT_EXPORT void set(const std::string &key, const json &value);
    LWCLIENT_EXPORT void set(const std::string &key, int value);
    LWCLIENT_EXPORT void set_device_id(const std::string &device_id);
    LWCLIENT_EXPORT json& UserData();
}

class LWCLIENT_EXPORT LwClient
{
public:
    LwClient();

    void Event(const std::string& event_name, std::function<json()> handler);
    void Event(const std::string& event_name, std::function<json()> handler, const std::string& module_id);
    json Event(const std::string& event_name);
    bool hasEvent(const std::string& event_name) const;
    std::vector<EventHandlerInfo> get_event_handlers_info() const;
    
    void removeEvent(const std::string& event_name, const std::string& module_id = "");
    void removeAllEventsForModule(const std::string& module_id);

    json GameInit(int platform_type = 1);
    json GameVersion(int platform_type = 1);
    json SwitchUserInfoGet(const std::string &switch_id = "", const std::string &password = "", int platform_type = 1);
    json SwitchUser(const std::string &switch_id = "", const std::string &password = "", int platform_type = 1);
    json GameLogin(const std::string &DA, std::optional<std::string> device_id = std::nullopt);
    json TranscribeKPI(int transcribe_value = 1);
    json MailTop();
    json MailReceive(const std::vector<int> &receive_id_list = {});
    json FriendSettingHelper(int friend_id = 0, int total_power = 0);
    json GoHome(int update_dispatch_cache = 1);
    json CheckConditionalPack();
    json AfkTop(int is_from_home = 1);
    json HomeSelectUnitUpdate(int select_unit_index = 1);
    json GetReplayInfo(int target_id = 0, int quest_type = 0, int difficulty = 0);
    json UpdateReplayInfo(int quest_type = 0, int target_id = 0, int difficulty = 0, const std::string &replay_info = "");
    json TowerUnitInfo();
    json TowerTop();
    json TowerUnitRecovery(const std::string &tower_unit_info = "");
    json TeamUpdate(int number = 100, const std::string &name = "", const std::vector<int> &user_unit_id_list = {0, 0, 0, 0, 0, 0}, int use_cost = 0, const std::vector<int> &season_enhancement_user_unit_id_list = {}, int use_cost_with_friend = 0);
    json TowerBattleStart(std::optional<std::string> validation_param = std::nullopt, int floor = 1, int team_number = 100);
    json TowerBattleEnd(std::optional<std::string> validation_param = std::nullopt, int floor = 1, const std::vector<int> &defeated_unit_id_list = {}, int result = 1, const std::string &tower_unit_info = "", const std::string &enemy_battle_info = "");
    json BattleStart(std::optional<std::string> validation_param = std::nullopt, int episode_id = 0, int daily_quest_stage_id = 0, int character_quest_id = 0, int relic_quest_id = 0, int team_number = 1, int difficulty = 0, const json &helper_info_dict = {{"unused_user_unit_id", 0}, {"helper_player_id", ""}, {"is_continuous_battle", 0}, {"is_friend", 0}});
    json BattleEnd(std::optional<std::string> validation_param = std::nullopt, int battle_id = 0, const std::vector<int> &defeated_unit_id_list = {}, int result = 1, int turns_to_clear = 0, int action_to_clear = 0, int damege_to_clear = 0, int reg_ranking = 0);
    json EpisodeEnd(int episode_id = 0);
    json WeeklyArenaTop(std::optional<std::string> validation_param = std::nullopt);
    json DailyArenaTop(std::optional<std::string> validation_param = std::nullopt);
    json ArenaBattleStart(std::optional<std::string> validation_param = std::nullopt, int category = 1, int custom_battle_id = 1, int enemy_level = 86, int difficulty = 5, const std::vector<int> &team_number = {1}, int battle_number = 0);
    json ArenaBattleEnd(std::optional<std::string> validation_param = std::nullopt, const std::vector<int> &defeated_unit_id_list1 = {}, const std::vector<int> &defeated_unit_id_list2 = {}, const std::vector<int> &defeated_unit_id_list3 = {}, int result = 1, int is_battle_change = 0, int is_battle_skill_use = 0, int is_battle_picture_use = 0, int battle_number = 0, const json &battle_score = {{"total_score", 0}, {"attack_count_score", 0}, {"graze_count_score", 0}, {"defeated_score", 0}}, int total_power = 0);
    json MultipleSpiritsWeeklyTop(std::optional<std::string> validation_param = std::nullopt);
    json GetMultipleSpiritsWeeklyReplayInfo(int multiple_spirits_weekly_master_id = 0);
    json MultipleSpiritsWeeklyBattleStart(std::optional<std::string> validation_param = std::nullopt, int multiple_spirits_weekly_master_id = 0, int cleared_level = 100, int final_cleared_level = 100, int team_number = 70, int unused_user_unit_id = 0, const std::string &helper_player_id = "", int is_continuous_battle = 0, int is_friend = 0);
    json MultipleSpiritsWeeklyBattleEnd(std::optional<std::string> validation_param = std::nullopt, int multiple_spirits_weekly_master_id = 0, int cleared_level = 100, int final_cleared_level = 100, const std::vector<int> &cleared_special_rules = {}, const std::vector<int> &defeated_unit_id_list = {}, int result = 1, int is_battle_change = 0, int is_battle_skill_use = 0, int is_battle_picture_use = 0, int action_to_clear = 0, int damege_to_clear = 0, int reg_ranking = 0);
    json GetSeriesBattleReplayInfo(int stage_id = 0, const std::vector<int> &area_index = {1, 2, 3});
    json SeriesBattleTeamUpdate(std::optional<std::string> validation_param = std::nullopt, const std::vector<int> &team_1 = {0, 0, 0, 0, 0, 0}, const std::vector<int> &team_2 = {0, 0, 0, 0, 0, 0}, const std::vector<int> &team_3 = {0, 0, 0, 0, 0, 0}, const std::vector<int> &team_4 = {0, 0, 0, 0, 0, 0}, const std::vector<int> &team_5 = {0, 0, 0, 0, 0, 0});
    json SeriesBattleStart(std::optional<std::string> validation_param = std::nullopt, int episode_id = 0, int difficulty = 2, const std::vector<int> &use_replays = {0, 0, 0, 0, 0});
    json SeriesBattleEnd(std::optional<std::string> validation_param = std::nullopt, const std::vector<int> &defeated_unit_id_list = {}, int result = 1, int turns_to_clear = 0, int action_to_clear = 0, int damege_to_clear = 0, int reg_ranking = 0, int score = 0, int over_damage = 0, int turn_num = 0);
    json UnitVoiceAutoSetUpdate(int autoset_flag = 1);
    json TutorialFirstGachaConfirm(std::optional<std::string> validation_param = std::nullopt);
    json TutorialUpdate(int progress_value = 1);
    json UserInfoUpdate(const std::string &player_name = "Z", const std::string &hero_name = "R");
    json GetSwitchCode();
    json MissionTop();
    json GetShopList();
    json BattlePassTop();
    json MissionEnd(int mission_id = 0, int is_all = 1, int mission_type = 0, int event_id = 0, int chapter_id = 0);
    json BattlePassMissionEnd(int mission_id = 0, int is_all = 1);
    json Exchange(int type = 0, int shop_item_id = 0, int count = 0);
    json GachaTop();
    json Gacha(int gacha_id = 0, int gacha_button_id = 0, int count = 0);
    json QuestClearedTeamInfo(int quest_type = 2, int target_id = 100100101, int difficulty = 2);
    json RogueliteTop();
    json AbilitySealInfo();
    json RogueliteMapStart(int map_id = 0, const std::vector<std::string> &roguelite_uuid_list = {}, const std::string &play_details = "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0");
    json RogueliteMapEnd();
    json RogueliteEventEnd(int area_id = 0);
    json RogueliteBattleStart(std::optional<std::string> validation_param = std::nullopt, const std::vector<int> &selected_seals = {}, int reroll_count = 0, int area_id = 0, const std::string &play_details = "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0");
    json RogueliteBattleEnd(std::optional<std::string> validation_param = std::nullopt, int area_id = 0, int result = 1, const std::vector<int> &defeated_unit_id_list = {}, int is_skip = 1, const std::string &play_details = "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0");
    json LectureRoomUnitTrustUp(int user_unit_id = 0);
    json GetAutoReplayInfo(int quest_type = 3, int target_id = 222800002, int difficulty = 0);
    json UpdateAutoReplayInfo(int quest_type = 3, int target_id = 222800002, int difficulty = 0, int win_rate = 10, int action_num = 0);
    json VotivePictureExchange(int votive_picture_id = 0, int votive_picture_exchange_id = 0);
    json WeeklyArenaBattleScoreReset(int battle_number = 1, std::optional<std::string> validation_param = std::nullopt);
    json PictureProtection(int user_picture_id = 0, int is_protection = 0);
    json PictureRecycle(std::vector<int> user_picture_id_list = {});

    struct UserCreationResult
    {
        std::string server;
        std::string device_id;
        std::string user_unique_id;
        std::string DA;
    };

    UserCreationResult UserCreate(int platform_type = 1, std::optional<std::string> user_unique_id = std::nullopt, const std::string &validation_param = "", const std::string &user_uuid = "");

    std::string GetDataAuth(
        std::optional<std::string> user_unique_id = std::nullopt,
        const std::string &country_code = "VN",
        int platform_type = 1,
        std::optional<std::string> device_info = std::nullopt,
        const std::string &os_info = "Android OS 14 / API-34 (UP1A.240805.015/U.Q1B1.a7c9f4d-2)",
        int login_from_title = 1,
        int idfa_opt_in = 0,
        std::optional<json> apps_flyer_info = std::nullopt,
        int Keyid = 0);
    std::string UpdateDA(const std::string &da);
    json api(const std::string &api, const json &body = {}, bool encryption = true, int key_id = -1);
private:
};