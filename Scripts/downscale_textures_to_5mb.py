import csv
import math
import os
import time
import unreal

# Conservative estimate for BC-compressed texture data including mips:
# bytes ~= width * height * BYTES_PER_PIXEL * MIP_CHAIN_FACTOR
MAX_TEXTURE_BYTES = 5 * 1024 * 1024
BYTES_PER_PIXEL = 1.0
MIP_CHAIN_FACTOR = 4.0 / 3.0
SAFETY_MARGIN = 0.9
AREA_LIMIT = int((MAX_TEXTURE_BYTES * SAFETY_MARGIN) / (BYTES_PER_PIXEL * MIP_CHAIN_FACTOR))
MIN_CAP = 256


def floor_pow2(value: int) -> int:
    power = 1
    while power * 2 <= value:
        power *= 2
    return power


def estimate_bytes(width: int, height: int) -> int:
    return int(width * height * BYTES_PER_PIXEL * MIP_CHAIN_FACTOR)


def compute_target_cap(width: int, height: int) -> int:
    area = width * height
    if area <= AREA_LIMIT:
        return 0

    scale = math.sqrt(float(AREA_LIMIT) / float(area))
    scaled_max_side = max(1, int(max(width, height) * scale))
    cap = floor_pow2(scaled_max_side)
    return max(MIN_CAP, cap)


def main() -> None:
    start = time.time()
    registry = unreal.AssetRegistryHelpers.get_asset_registry()
    texture_class_path = unreal.TopLevelAssetPath("/Script/Engine", "Texture2D")
    asset_filter = unreal.ARFilter(
        package_paths=["/Game"],
        class_paths=[texture_class_path],
        recursive_paths=True,
    )
    texture_assets = registry.get_assets(asset_filter)

    changed_rows = []
    scanned = 0
    changed = 0
    skipped = 0

    unreal.log(f"[TextureDownscale] Found {len(texture_assets)} Texture2D assets in /Game")
    for asset_data in texture_assets:
        scanned += 1
        texture = asset_data.get_asset()
        if not texture:
            skipped += 1
            continue

        width = int(texture.blueprint_get_size_x())
        height = int(texture.blueprint_get_size_y())
        if width <= 0 or height <= 0:
            skipped += 1
            continue

        current_cap = int(texture.get_editor_property("max_texture_size"))
        current_effective_max = current_cap if current_cap > 0 else max(width, height)
        target_cap = compute_target_cap(width, height)
        if target_cap == 0:
            continue

        if current_effective_max <= target_cap:
            continue

        texture.set_editor_property("max_texture_size", target_cap)
        unreal.EditorAssetLibrary.save_loaded_asset(texture)
        changed += 1

        old_w = min(width, current_effective_max)
        old_h = min(height, current_effective_max)
        new_w = min(width, target_cap)
        new_h = min(height, target_cap)
        changed_rows.append(
            {
                "asset": texture.get_path_name(),
                "size_x": width,
                "size_y": height,
                "old_cap": current_cap,
                "new_cap": target_cap,
                "old_est_bytes": estimate_bytes(old_w, old_h),
                "new_est_bytes": estimate_bytes(new_w, new_h),
            }
        )

        if changed % 100 == 0:
            unreal.log(f"[TextureDownscale] Changed {changed} textures...")

    project_dir = unreal.Paths.project_dir()
    report_dir = os.path.join(project_dir, "Builded")
    os.makedirs(report_dir, exist_ok=True)

    report_csv = os.path.join(report_dir, "texture_downscale_report.csv")
    with open(report_csv, "w", newline="", encoding="utf-8") as csv_file:
        writer = csv.DictWriter(
            csv_file,
            fieldnames=[
                "asset",
                "size_x",
                "size_y",
                "old_cap",
                "new_cap",
                "old_est_bytes",
                "new_est_bytes",
            ],
        )
        writer.writeheader()
        writer.writerows(changed_rows)

    elapsed = time.time() - start
    unreal.log(f"[TextureDownscale] Scanned: {scanned}")
    unreal.log(f"[TextureDownscale] Changed: {changed}")
    unreal.log(f"[TextureDownscale] Skipped: {skipped}")
    unreal.log(f"[TextureDownscale] Report: {report_csv}")
    unreal.log(f"[TextureDownscale] Done in {elapsed:.1f}s")


if __name__ == "__main__":
    main()
