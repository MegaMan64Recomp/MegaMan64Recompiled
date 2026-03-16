#include "ovl_patches.hpp"
//#include "../../RecompiledPatches/patches_bin.h"
//#include "../../RecompiledPatches/recomp_overlays.inl"

#include "librecomp/overlays.hpp"
#include "librecomp/game.hpp"

// Patch registration is disabled because the recompiled patch binaries
// (RecompiledPatches/patches_bin.h and recomp_overlays.inl) are not yet
// included in this repository. Re-enable once the patch build pipeline is
// set up and the generated files are available.
//void zelda64::register_patches() {
//    recomp::overlays::register_patches(mm_patches_bin, sizeof(mm_patches_bin), section_table, ARRLEN(section_table));
//    recomp::overlays::register_base_exports(export_table);
//    recomp::overlays::register_base_events(event_names);
//    recomp::overlays::register_manual_patch_symbols(manual_patch_symbols);
//}
