#pragma once
// CombinedActorInfo — vanilla 21-slot and fat 42-slot layouts.
// Fat buffer is ALWAYS heap. Vanilla embeds stay 6688 bytes.

#include <cstddef>
#include <cstdint>
#include <cstring>

namespace uh42 {

constexpr int    kVanillaSlots = 21;
constexpr int    kFatSlots     = 42;
constexpr int    kActorStride  = 0x118;
constexpr int    kBondStride   = 12;

constexpr std::size_t kVanillaSize = 0x1A20; // 6688
constexpr std::size_t kFatSize     = 0x3214; // 42 actors + 42 bonds + tail

constexpr std::uint32_t kVanillaActorsEnd = 0x1704;
constexpr std::uint32_t kVanillaBondsEnd  = 0x1800;
constexpr std::uint32_t kVanillaFlag      = 0x1818;

constexpr std::uint32_t kFatActorsEnd     = 0x0C + kFatSlots * kActorStride; // 0x2DFC
constexpr std::uint32_t kFatBondsEnd      = kFatActorsEnd + kFatSlots * kBondStride; // 0x2FF4
constexpr std::uint32_t kFatFlag          = kFatBondsEnd + 24; // 0x300C

struct ActorView {
    float  m0[12];
    float  m1[12];
    float  m2[12];
    float  fuse;
    std::uint32_t flags;
    char   name[64];
    char   sub[64];
};

struct BondView {
    std::uint32_t src_mat;
    std::uint32_t dst_mat;
    std::uint8_t  src;
    std::uint8_t  dst;
    std::uint16_t pad;
};

inline bool has_magic(const void* p) {
    return p && std::memcmp(p, "CmbAct", 6) == 0;
}

inline int actor_count(const void* p) {
    return p ? static_cast<int>(static_cast<const std::uint8_t*>(p)[9]) : 0;
}

inline int bond_count(const void* p) {
    return p ? static_cast<int>(static_cast<const std::uint8_t*>(p)[10]) : 0;
}

inline void set_counts(void* p, int actors, int bonds) {
    auto* b = static_cast<std::uint8_t*>(p);
    b[9]  = static_cast<std::uint8_t>(actors);
    b[10] = static_cast<std::uint8_t>(bonds);
}

inline const std::uint8_t* actor_ptr(const void* cai, int i, bool /*fat*/) {
    const auto* b = static_cast<const std::uint8_t*>(cai);
    return b + 0x0C + i * kActorStride;
}

inline std::uint8_t* actor_ptr(void* cai, int i, bool fat) {
    return const_cast<std::uint8_t*>(actor_ptr(static_cast<const void*>(cai), i, fat));
}

inline const std::uint8_t* bond_ptr(const void* cai, int i, bool fat) {
    const auto* b = static_cast<const std::uint8_t*>(cai);
    return b + (fat ? kFatActorsEnd : kVanillaActorsEnd) + i * kBondStride;
}

inline std::uint8_t* bond_ptr(void* cai, int i, bool fat) {
    return const_cast<std::uint8_t*>(bond_ptr(static_cast<const void*>(cai), i, fat));
}

void read_actor(const void* cai, int i, bool fat, ActorView* out);
void write_actor(void* cai, int i, bool fat, const ActorView& in);
void read_bond(const void* cai, int i, bool fat, BondView* out);
void write_bond(void* cai, int i, bool fat, const BondView& in);

void init_header(void* cai);
void clear_fat(void* fat);
// Copy the vanilla 21-slot prefix into a fat buffer (extras stay zero).
void vanilla_to_fat(const void* vanilla, void* fat);
// Copy the first 21 actors/bonds of a fat buffer into a vanilla embed (safe).
void fat_to_vanilla(const void* fat, void* vanilla);

} // namespace uh42
