#pragma once
// TOTK 1.2.1  BID 9B4E43650501A4D4
// Offsets are NSO .text (same numbers as pchtxt with offset_shift 0x100).

#include <cstdint>

namespace uh42::addr {

constexpr std::uint32_t kBuildIdLo = 0x9B4E4365u;
constexpr char kBuildId[] = "9B4E43650501A4D4";
constexpr std::uint64_t kTitleId = 0x0100F2C0115B6000ull;

// CombinedActorInfo / Autobuild
constexpr std::uintptr_t SerializeCAI          = 0x00D5D024;
constexpr std::uintptr_t SerializeEarlyCmp     = 0x00D5D188; // CMP W0,#21 then B.LE — history reject
constexpr std::uintptr_t LoadCAI               = 0x00D594D4;
constexpr std::uintptr_t CreateActors          = 0x0174F0E4;
constexpr std::uintptr_t CreateBonds           = 0x0174FC18;
constexpr std::uintptr_t InitializeActors      = 0x00D5A5E0;
constexpr std::uintptr_t SetActorTransforms    = 0x01755BB4;
constexpr std::uintptr_t ResetCAI              = 0x017508A0;
constexpr std::uintptr_t ValidateCAI           = 0x00D5B90C;
constexpr std::uintptr_t GetNumCombinedActor   = 0x013C4200;
constexpr std::uintptr_t SetStructBinary       = 0x016D6168;
constexpr std::uintptr_t GetStructBinary       = 0x01750758;

// SpecialPowerModule::calc — the favorite/history write
constexpr std::uintptr_t SaveSizeImm           = 0x00D5700C; // MOV W2, #0x1A20

// Live 42 (already in the IPS; plugin re-applies if IPS is missing)
constexpr std::uintptr_t SoftConstraintCmp     = 0x0176FE80;
constexpr std::uintptr_t SearchBindPointsCmp   = 0x01655A4C;
constexpr std::uintptr_t PostCalcUltraHandCmp  = 0x00AF0DAC;
constexpr std::uintptr_t CombineModuleCalcCmp  = 0x00FF4CB8;
constexpr std::uintptr_t CombineModuleSub21    = 0x00FF5578;
constexpr std::uintptr_t ConstraintMgrDouble   = 0x010F9FAC;

// Autobuild loop clamps (patched only after a fat CAI is installed)
constexpr std::uintptr_t CreateBondsCmp0       = 0x0174FDF0;
constexpr std::uintptr_t CreateBondsCmp1       = 0x01750000;
constexpr std::uintptr_t CreateBondsBondBase   = 0x0174FFE8; // MOV W8, #0x1704
constexpr std::uintptr_t CreateActorsCmp0      = 0x0174F2B4;
constexpr std::uintptr_t CreateActorsCmp1      = 0x0174F8E4;
constexpr std::uintptr_t InitActorsCmp0        = 0x00D5A678;
constexpr std::uintptr_t InitActorsCmp1        = 0x00D5ADF0;
constexpr std::uintptr_t SetXformCmp           = 0x01755C38;
constexpr std::uintptr_t LoadCmp0              = 0x00D596F0;
constexpr std::uintptr_t LoadCmp1              = 0x00D59944;
constexpr std::uintptr_t SerializeClamp0       = 0x00D5D6E0;
constexpr std::uintptr_t SerializeClamp1       = 0x00D5D714;
constexpr std::uintptr_t SerializeClamp2       = 0x00D5D880;
constexpr std::uintptr_t SerializeClamp3       = 0x00D5D9D0;
constexpr std::uintptr_t SerializeClamp4       = 0x00D5DF5C;
constexpr std::uintptr_t SerializeBondBase     = 0x00D5DA2C; // MOV W24, #0x1704
constexpr std::uintptr_t ValidateCmp0          = 0x00D5B958;
constexpr std::uintptr_t ValidateCmp1          = 0x00D5B960;

// BuildingInfo
constexpr std::ptrdiff_t kBuildingInfoIndex    = 0x4;
constexpr std::ptrdiff_t kBuildingInfoInline   = 0xC50;   // 0x3000 vanilla dest
constexpr std::ptrdiff_t kBuildingInfoCaiPtr   = 0x3C50;

// SpecialPowerModule
constexpr std::ptrdiff_t kSpModuleCai          = 0x52E0;

// GameData hash used for Autobuild.Draft CombinedActorInfo
constexpr std::uint32_t kCaiGmdHash            = 0x98C8E990u;

} // namespace uh42::addr
