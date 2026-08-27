#pragma once

#include <cstdint>

namespace uh42 {

// main_base = runtime address of TOTK main (NSO .text).
void apply_live_patches(std::uintptr_t main_base);
void apply_fat_loop_patches(std::uintptr_t main_base);
void restore_fat_loop_patches(std::uintptr_t main_base);

bool write_insn(std::uintptr_t addr, std::uint32_t insn);
std::uint32_t read_insn(std::uintptr_t addr);

} // namespace uh42
