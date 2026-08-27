Ultrahand 42 — source (TOTK 1.2.1)

Title ID `0100F2C0115B6000`  
Build ID `9B4E43650501A4D4`

This is the code that is actually running in the working build.

## What it does

- Live Ultrahand attach cap **21 → 42** (CMP `#22` → `#43` on 1.2.1).
- Autobuild history/ghost **42** via a fat CombinedActorInfo in the plugin, plus sidecar files:
  `sdmc:/config/ultrahand42/drafts/gXXXXXXXX.bin`
- Nintendo `progress.sav` CombinedActorInfo stays **6688 bytes / 21 slots**.
- History list is still **30 slots** (game limit).
- The Phive `ConstraintMgr` ctor rewrite is **not** applied. It crashed on boot.

## Layout

```
plugin/source/main.cpp      exlaunch entry (exl_main)
plugin/source/hooks.cpp     serialize/load hooks + sidecar
plugin/source/patches.cpp   live CMPs + Autobuild fat-loop patches
plugin/source/fat_cai.cpp   21-slot ↔ 42-slot CAI copy
plugin/include/             addrs and headers for 1.2.1
patches/Ultrahand42.pchtxt  optional IPS-style live CMPs (see note)
```

## Notes

- Offsets are NSO `.text` for 1.2.1 only.
- `fopen("sdmc:/...")` is what the sidecar uses. Fine on Ryujinx; real hardware may need `nn::fs`.
- `patches/Ultrahand42.pchtxt` still lists the ConstraintMgr line. The **plugin does not apply that line**.
