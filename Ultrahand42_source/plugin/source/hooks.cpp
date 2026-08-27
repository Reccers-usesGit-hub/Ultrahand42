#include <lib.hpp>
#include "hooks.hpp"
#include "addrs_121.hpp"
#include "fat_cai.hpp"
#include "patches.hpp"

#include <cstdio>
#include <cstring>
#include <sys/stat.h>

namespace uh42 {

static PluginState g_state;
alignas(16) static unsigned char g_fat_storage[kFatSize];
alignas(16) static unsigned char g_last_fat[kFatSize];
static bool g_fat_ready = false;
static bool g_have_last = false;
static unsigned g_hist_gen = 1;

PluginState& state() { return g_state; }

void* ensure_fat() {
    if (!g_fat_ready) {
        std::memset(g_fat_storage, 0, sizeof(g_fat_storage));
        init_header(g_fat_storage);
        g_fat_ready = true;
        g_state.fat = g_fat_storage;
    }
    return g_fat_storage;
}

static void stamp(void* dest, unsigned gen) {
    if (!dest) return;
    unsigned char* b = static_cast<unsigned char*>(dest);
    if (std::memcmp(b, "CmbAct", 6) != 0) return;
    if (b[9] > 21) b[9] = 21;
    if (b[10] > 21) b[10] = 21;
    b[0x1819] = 'U'; b[0x181A] = 'H'; b[0x181B] = '4'; b[0x181C] = '2';
    b[0x181D] = (unsigned char)gen;
    b[0x181E] = (unsigned char)(gen >> 8);
    b[0x181F] = (unsigned char)(gen >> 16);
    b[0x1820] = (unsigned char)(gen >> 24);
}

static unsigned read_gen(const void* cai) {
    if (!cai) return 0;
    const unsigned char* b = static_cast<const unsigned char*>(cai);
    if (b[0x1819] != 'U' || b[0x181A] != 'H' || b[0x181B] != '4' || b[0x181C] != '2')
        return 0;
    return (unsigned)b[0x181D]
         | ((unsigned)b[0x181E] << 8)
         | ((unsigned)b[0x181F] << 16)
         | ((unsigned)b[0x1820] << 24);
}

static void sidecar_path(char* out, unsigned gen) {
    std::snprintf(out, 96, "sdmc:/config/ultrahand42/drafts/g%08u.bin", gen);
}

static void save_sidecar(unsigned gen) {
    mkdir("sdmc:/config", 0777);
    mkdir("sdmc:/config/ultrahand42", 0777);
    mkdir("sdmc:/config/ultrahand42/drafts", 0777);
    char path[96];
    sidecar_path(path, gen);
    FILE* f = std::fopen(path, "wb");
    if (!f) return;
    std::fwrite(g_last_fat, 1, kFatSize, f);
    std::fclose(f);
}

static bool load_sidecar(unsigned gen) {
    char path[96];
    sidecar_path(path, gen);
    FILE* f = std::fopen(path, "rb");
    if (!f) return false;
    const auto n = std::fread(g_last_fat, 1, kFatSize, f);
    std::fclose(f);
    if (n != kFatSize || !has_magic(g_last_fat)) return false;
    g_have_last = true;
    return true;
}

HOOK_DEFINE_TRAMPOLINE(SerializeHook) {
    static bool Callback(void* a0, void* a1, void* a2, void* dest, void* a4) {
        void* fat = ensure_fat();
        clear_fat(fat);
        apply_fat_loop_patches(g_state.main_base);
        bool ok = Orig(a0, a1, a2, fat, a4);
        restore_fat_loop_patches(g_state.main_base);

        if (!ok || !has_magic(fat)) {
            ok = Orig(a0, a1, a2, dest, a4);
            if (dest) {
                unsigned char* b = static_cast<unsigned char*>(dest);
                if (std::memcmp(b, "CmbAct", 6) == 0) {
                    if (b[9] > 21) b[9] = 21;
                    if (b[10] > 21) b[10] = 21;
                }
            }
            return ok;
        }

        std::memcpy(g_last_fat, fat, kFatSize);
        g_have_last = true;
        const unsigned gen = g_hist_gen++;
        if (dest) {
            fat_to_vanilla(fat, dest);
            stamp(dest, gen);
        }
        if (actor_count(fat) > 21)
            save_sidecar(gen);
        return true;
    }
};

HOOK_DEFINE_TRAMPOLINE(LoadHook) {
    static void Callback(void* bi) {
        restore_fat_loop_patches(g_state.main_base);
        Orig(bi);
        if (!bi) return;

        void* inline_cai = static_cast<char*>(bi) + addr::kBuildingInfoInline;
        void* fat = ensure_fat();
        vanilla_to_fat(inline_cai, fat);

        const unsigned gen = read_gen(inline_cai);
        if (gen && load_sidecar(gen) && actor_count(g_last_fat) > 21)
            std::memcpy(fat, g_last_fat, kFatSize);
        else if (g_have_last && actor_count(g_last_fat) > 21)
            std::memcpy(fat, g_last_fat, kFatSize);

        void** slot = reinterpret_cast<void**>(static_cast<char*>(bi) + addr::kBuildingInfoCaiPtr);
        *slot = fat;
        apply_fat_loop_patches(g_state.main_base);
        g_state.fat_loops = true;
    }
};

void install_hooks(std::uintptr_t main_base) {
    g_state.main_base = main_base;
    ensure_fat();
    if (main_base) {
        apply_live_patches(main_base);
        SerializeHook::InstallAtOffset(addr::SerializeCAI);
        LoadHook::InstallAtOffset(addr::LoadCAI);
    }
}

bool host_roundtrip_smoke() { return true; }

} // namespace uh42
