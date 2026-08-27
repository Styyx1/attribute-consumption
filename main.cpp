#include <RE/Skyrim.h>
#include <REX/REX.h>
#include <SKSE/SKSE.h>

namespace AATC::CONF::CONST {

inline constexpr auto TOML_P_D = "Data/SKSE/Plugins/attribute-consume.toml";
inline constexpr auto TOML_P_C =
    "Data/SKSE/Plugins/attribute-consume_custom.toml";
inline constexpr auto S = "Settings";

} // namespace AATC::CONF::CONST

namespace AATC {

namespace CONF {

using namespace CONST;
inline REX::TOML::Bool use_stamina{S, "bUseStamina", true};
inline REX::TOML::Bool use_magicka{S, "bUseMagicka", true};
inline REX::TOML::Bool use_horse{S, "bUseHorse", true};

inline REX::TOML::F32 stam_drain{S, "fStaminaDrain", 0.0};
inline REX::TOML::F32 mag_drain{S, "fMagickaDrain", 0.0};
inline REX::TOML::F32 horse_drain{S, "fHorseDrain", 0.0};

inline void READ() {
  auto t = REX::TSingleton<REX::FTomlSettingStore>::GetSingleton();
  t->Init(TOML_P_D, TOML_P_C);
  t->Load();
}
} // namespace CONF

namespace HOOK {
struct AV_HOOK {

  static inline void Call(RE::Actor *a_actor, RE::ACTOR_VALUE_MODIFIER av_modi,
                          RE::ActorValue a_av, float a_value) {

    auto player = RE::PlayerCharacter::GetSingleton();
    RE::NiPointer<RE::Actor> horse{};
    player->GetMount(horse);

    auto dmg = RE::ACTOR_VALUE_MODIFIER::kDamage;
    const auto stm = RE::ActorValue::kStamina;
    const auto mag = RE::ActorValue::kMagicka;

    if (a_actor == horse.get() && CONF::use_horse.GetValue() &&
        av_modi == dmg && a_av == stm &&
        !player->IsInCombat() == a_value < 0.f) {
      a_value *= CONF::horse_drain.GetValue();
    }
    if (a_actor->IsPlayerRef() && av_modi == dmg && !player->IsInCombat() &&
        a_value < 0.f) {
      switch (a_av) {
      case stm:
        a_value *= CONF::stam_drain.GetValue();
        break;
      case mag:
        a_value *= CONF::mag_drain.GetValue();
        break;
      default:
        break;
      }
    }

    func(a_actor, av_modi, a_av, a_value);
  }
  static inline REL::THook func{REL::ID(38467), 0x14, Call};
};
} // namespace HOOK

} // namespace AATC

SKSE_PLUGIN_LOAD(const SKSE::LoadInterface *a_skse) {
  SKSE::Init(a_skse, {.trampoline = true});
  AATC::CONF::READ();
  return true;
}