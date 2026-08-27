#include "fat_cai.hpp"

#include <algorithm>
#include <cstring>

namespace uh42 {

void read_actor(const void* cai, int i, bool fat, ActorView* out) {
    const auto* p = actor_ptr(cai, i, fat);
    std::memcpy(out->m0, p + 0x00, 48);
    std::memcpy(out->m1, p + 0x30, 48);
    std::memcpy(out->m2, p + 0x60, 48);
    std::memcpy(&out->fuse,  p + 0x90, 4);
    std::memcpy(&out->flags, p + 0x94, 4);
    std::memcpy(out->name,   p + 0x98, 64);
    std::memcpy(out->sub,    p + 0xD8, 64);
    out->name[63] = 0;
    out->sub[63]  = 0;
}

void write_actor(void* cai, int i, bool fat, const ActorView& in) {
    auto* p = actor_ptr(cai, i, fat);
    std::memcpy(p + 0x00, in.m0, 48);
    std::memcpy(p + 0x30, in.m1, 48);
    std::memcpy(p + 0x60, in.m2, 48);
    std::memcpy(p + 0x90, &in.fuse,  4);
    std::memcpy(p + 0x94, &in.flags, 4);
    std::memset(p + 0x98, 0, 64);
    std::memset(p + 0xD8, 0, 64);
    std::memcpy(p + 0x98, in.name, 63);
    std::memcpy(p + 0xD8, in.sub,  63);
}

void read_bond(const void* cai, int i, bool fat, BondView* out) {
    const auto* p = bond_ptr(cai, i, fat);
    std::memcpy(out, p, sizeof(BondView));
}

void write_bond(void* cai, int i, bool fat, const BondView& in) {
    auto* p = bond_ptr(cai, i, fat);
    std::memcpy(p, &in, sizeof(BondView));
}

void init_header(void* cai) {
    auto* b = static_cast<std::uint8_t*>(cai);
    std::memset(b, 0, 12);
    std::memcpy(b, "CmbAct", 6);
    b[8] = 1; // version seen in the wild
}

void clear_fat(void* fat) {
    std::memset(fat, 0, kFatSize);
    init_header(fat);
}

void vanilla_to_fat(const void* vanilla, void* fat) {
    clear_fat(fat);
    if (!vanilla || !has_magic(vanilla))
        return;
    const int na = std::min(actor_count(vanilla), kVanillaSlots);
    const int nb = std::min(bond_count(vanilla),  kVanillaSlots);
    set_counts(fat, na, nb);
    for (int i = 0; i < na; ++i) {
        ActorView a{};
        read_actor(vanilla, i, false, &a);
        write_actor(fat, i, true, a);
    }
    for (int i = 0; i < nb; ++i) {
        BondView b{};
        read_bond(vanilla, i, false, &b);
        write_bond(fat, i, true, b);
    }
    // bbox + flag live at the vanilla tail; copy into the fat tail
    const auto* src = static_cast<const std::uint8_t*>(vanilla);
    auto* dst = static_cast<std::uint8_t*>(fat);
    std::memcpy(dst + kFatBondsEnd, src + kVanillaBondsEnd, 24);
    dst[kFatFlag] = src[kVanillaFlag];
}

void fat_to_vanilla(const void* fat, void* vanilla) {
    if (!vanilla)
        return;
    std::memset(vanilla, 0, kVanillaSize);
    init_header(vanilla);
    if (!fat || !has_magic(fat))
        return;
    const int na = std::min(actor_count(fat), kVanillaSlots);
    const int nb = std::min(bond_count(fat),  kVanillaSlots);
    set_counts(vanilla, na, nb);
    for (int i = 0; i < na; ++i) {
        ActorView a{};
        read_actor(fat, i, true, &a);
        write_actor(vanilla, i, false, a);
    }
    for (int i = 0; i < nb; ++i) {
        BondView b{};
        read_bond(fat, i, true, &b);
        write_bond(vanilla, i, false, b);
    }
    const auto* src = static_cast<const std::uint8_t*>(fat);
    auto* dst = static_cast<std::uint8_t*>(vanilla);
    std::memcpy(dst + kVanillaBondsEnd, src + kFatBondsEnd, 24);
    dst[kVanillaFlag] = src[kFatFlag];
}

} // namespace uh42
