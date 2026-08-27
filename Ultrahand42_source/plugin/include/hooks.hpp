#pragma once

#include <cstdint>

namespace uh42 {

struct PluginState {
    std::uintptr_t main_base = 0;
    void*          fat       = nullptr;
    int            last_draft = 0;
    bool           fat_loops  = false;
};

PluginState& state();

// Allocate the 42-slot buffer on the system heap (once).
void* ensure_fat();

void install_hooks(std::uintptr_t main_base);

// Host test: run serialize/load logic against in-memory buffers.
bool host_roundtrip_smoke();

} // namespace uh42
