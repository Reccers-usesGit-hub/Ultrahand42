#include <lib.hpp>
#include "hooks.hpp"

extern "C" __attribute__((visibility("default"))) void exl_main(void* x0, void* x1) {
    (void)x0;
    (void)x1;
    exl::hook::Initialize();
    uh42::install_hooks(exl::util::modules::GetTargetStart());
}

extern "C" __attribute__((visibility("default"))) NORETURN void exl_exception_entry() {
    EXL_ABORT("uh42 exception");
}
