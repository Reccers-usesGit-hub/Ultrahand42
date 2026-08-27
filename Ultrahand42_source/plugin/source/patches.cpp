#include "patches.hpp"
#include "addrs_121.hpp"

#include <cstring>

namespace uh42 {

static void icache_flush(void* p, std::size_t n) {
    __builtin___clear_cache(static_cast<char*>(p), static_cast<char*>(p) + n);
}

std::uint32_t read_insn(std::uintptr_t addr) {
    std::uint32_t v = 0;
    std::memcpy(&v, reinterpret_cast<const void*>(addr), 4);
    return v;
}

bool write_insn(std::uintptr_t addr, std::uint32_t insn) {
    std::memcpy(reinterpret_cast<void*>(addr), &insn, 4);
    icache_flush(reinterpret_cast<void*>(addr), 4);
    return true;
}

static void patch_if(std::uintptr_t base, std::uintptr_t off,
                     std::uint32_t expect, std::uint32_t next) {
    const auto a = base + off;
    if (read_insn(a) == expect)
        write_insn(a, next);
}

void apply_live_patches(std::uintptr_t main) {
    using namespace addr;
    patch_if(main, SoftConstraintCmp,    0x7100591F, 0x7100AD1F);
    patch_if(main, SearchBindPointsCmp,  0x7100591F, 0x7100AD1F);
    patch_if(main, PostCalcUltraHandCmp, 0x7100591F, 0x7100AD1F);
    patch_if(main, CombineModuleCalcCmp, 0x7100593F, 0x7100AD3F);
    patch_if(main, CombineModuleSub21,   0x51005535, 0x5100A935);
    patch_if(main, 0x00D5D188,           0x7100541F, 0x7100A81F);
}

void apply_fat_loop_patches(std::uintptr_t main) {
    using namespace addr;
    patch_if(main, CreateBondsCmp0, 0x710056FF, 0x7100AAFF);
    patch_if(main, CreateBondsCmp1, 0xF100579F, 0xF100AB9F);
    patch_if(main, CreateActorsCmp0, 0xF100577F, 0xF100AB7F);
    patch_if(main, CreateActorsCmp1, 0xF100577F, 0xF100AB7F);
    patch_if(main, InitActorsCmp0,  0x7100579F, 0x7100AB9F);
    patch_if(main, InitActorsCmp1,  0x7100571F, 0x7100AB1F);
    patch_if(main, SetXformCmp,     0xF100571F, 0xF100AB1F);
    patch_if(main, LoadCmp0,        0x710056DF, 0x7100AADF);
    patch_if(main, LoadCmp1,        0xF10056DF, 0xF100AADF);
    patch_if(main, SerializeClamp0, 0x7100575F, 0x7100AB5F);
    patch_if(main, SerializeClamp1, 0x7100575F, 0x7100AB5F);
    patch_if(main, SerializeClamp2, 0x7100575F, 0x7100AB5F);
    patch_if(main, SerializeClamp3, 0x7100575F, 0x7100AB5F);
    patch_if(main, SerializeClamp4, 0x7100575F, 0x7100AB5F);
    patch_if(main, CreateBondsBondBase, 0x5282E088, 0x5285BF88);
    patch_if(main, SerializeBondBase,   0x5282E098, 0x5285BF98);
    patch_if(main, ValidateCmp0, 0x7100593F, 0x7100AD3F);
    patch_if(main, ValidateCmp1, 0x7100591F, 0x7100AD1F);
}

void restore_fat_loop_patches(std::uintptr_t main) {
    using namespace addr;
    patch_if(main, CreateBondsCmp0, 0x7100AAFF, 0x710056FF);
    patch_if(main, CreateBondsCmp1, 0xF100AB9F, 0xF100579F);
    patch_if(main, CreateActorsCmp0, 0xF100AB7F, 0xF100577F);
    patch_if(main, CreateActorsCmp1, 0xF100AB7F, 0xF100577F);
    patch_if(main, InitActorsCmp0,  0x7100AB9F, 0x7100579F);
    patch_if(main, InitActorsCmp1,  0x7100AB1F, 0x7100571F);
    patch_if(main, SetXformCmp,     0xF100AB1F, 0xF100571F);
    patch_if(main, LoadCmp0,        0x7100AADF, 0x710056DF);
    patch_if(main, LoadCmp1,        0xF100AADF, 0xF10056DF);
    patch_if(main, SerializeClamp0, 0x7100AB5F, 0x7100575F);
    patch_if(main, SerializeClamp1, 0x7100AB5F, 0x7100575F);
    patch_if(main, SerializeClamp2, 0x7100AB5F, 0x7100575F);
    patch_if(main, SerializeClamp3, 0x7100AB5F, 0x7100575F);
    patch_if(main, SerializeClamp4, 0x7100AB5F, 0x7100575F);
    patch_if(main, CreateBondsBondBase, 0x5285BF88, 0x5282E088);
    patch_if(main, SerializeBondBase,   0x5285BF98, 0x5282E098);
    patch_if(main, ValidateCmp0, 0x7100AD3F, 0x7100593F);
    patch_if(main, ValidateCmp1, 0x7100AD1F, 0x7100591F);
}

} // namespace uh42
