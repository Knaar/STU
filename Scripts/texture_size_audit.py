import csv
import os
import unreal

LIMIT_BYTES = 5 * 1024 * 1024


def main():
    registry = unreal.AssetRegistryHelpers.get_asset_registry()
    texture_class_path = unreal.TopLevelAssetPath("/Script/Engine", "Texture2D")
    asset_filter = unreal.ARFilter(package_paths=["/Game"], class_paths=[texture_class_path], recursive_paths=True)
    assets = registry.get_assets(asset_filter)

    rows = []
    over_disk = 0
    max_disk = 0
    max_mem = 0

    for asset_data in assets:
        texture = asset_data.get_asset()
        if not texture:
            continue
        disk_size, mem_size = texture.blueprint_get_texture_source_disk_and_memory_size()
        disk_size = int(disk_size)
        mem_size = int(mem_size)
        if disk_size > LIMIT_BYTES:
            over_disk += 1
        if disk_size > max_disk:
            max_disk = disk_size
        if mem_size > max_mem:
            max_mem = mem_size

        rows.append(
            {
                "asset": texture.get_path_name(),
                "disk_size": disk_size,
                "memory_size": mem_size,
                "max_texture_size": int(texture.get_editor_property("max_texture_size")),
                "size_x": int(texture.blueprint_get_size_x()),
                "size_y": int(texture.blueprint_get_size_y()),
            }
        )

    rows.sort(key=lambda x: x["disk_size"], reverse=True)

    report_dir = os.path.join(unreal.Paths.project_dir(), "Builded")
    os.makedirs(report_dir, exist_ok=True)
    report_path = os.path.join(report_dir, "texture_size_audit.csv")
    with open(report_path, "w", newline="", encoding="utf-8") as f:
        writer = csv.DictWriter(
            f,
            fieldnames=["asset", "disk_size", "memory_size", "max_texture_size", "size_x", "size_y"],
        )
        writer.writeheader()
        writer.writerows(rows)

    unreal.log(f"[TextureAudit] Textures scanned: {len(rows)}")
    unreal.log(f"[TextureAudit] Source disk size > 5MB: {over_disk}")
    unreal.log(f"[TextureAudit] Max source disk size: {max_disk}")
    unreal.log(f"[TextureAudit] Max source memory size: {max_mem}")
    unreal.log(f"[TextureAudit] Report: {report_path}")


if __name__ == "__main__":
    main()
