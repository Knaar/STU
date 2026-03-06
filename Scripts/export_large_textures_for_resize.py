import json
import math
import os
import unreal

LIMIT_BYTES = 5 * 1024 * 1024
SAFETY = 0.95
BYTES_PER_PIXEL = 4.0
AREA_LIMIT = int((LIMIT_BYTES * SAFETY) / BYTES_PER_PIXEL)


def compute_target_size(width: int, height: int):
    area = width * height
    if area <= AREA_LIMIT:
        return width, height

    scale = math.sqrt(float(AREA_LIMIT) / float(area))
    new_w = max(4, int(width * scale))
    new_h = max(4, int(height * scale))

    # Texture imports are generally happier on 4-byte aligned dimensions.
    new_w = max(4, (new_w // 4) * 4)
    new_h = max(4, (new_h // 4) * 4)
    return new_w, new_h


def main():
    project_dir = unreal.Paths.project_dir()
    batch_dir = os.path.join(project_dir, "Builded", "TextureResizeBatch")
    export_dir = os.path.join(batch_dir, "exports")
    resized_dir = os.path.join(batch_dir, "resized")
    os.makedirs(export_dir, exist_ok=True)
    os.makedirs(resized_dir, exist_ok=True)

    registry = unreal.AssetRegistryHelpers.get_asset_registry()
    texture_class_path = unreal.TopLevelAssetPath("/Script/Engine", "Texture2D")
    asset_filter = unreal.ARFilter(package_paths=["/Game"], class_paths=[texture_class_path], recursive_paths=True)
    assets = registry.get_assets(asset_filter)

    manifest = []
    failures = []
    index = 0

    for asset_data in assets:
        texture = asset_data.get_asset()
        if not texture:
            continue

        disk_size, _ = texture.blueprint_get_texture_source_disk_and_memory_size()
        disk_size = int(disk_size)
        if disk_size <= LIMIT_BYTES:
            continue

        width = int(texture.blueprint_get_size_x())
        height = int(texture.blueprint_get_size_y())
        new_w, new_h = compute_target_size(width, height)

        object_path = texture.get_path_name()
        package_path = object_path.split(".")[0]
        destination_path = package_path.rsplit("/", 1)[0]
        destination_name = package_path.rsplit("/", 1)[1]

        safe_name = destination_name.replace(":", "_").replace("/", "_")
        export_file = os.path.join(export_dir, f"{index:04d}_{safe_name}.png")
        resized_file = os.path.join(resized_dir, f"{index:04d}_{safe_name}.png")

        task = unreal.AssetExportTask()
        task.object = texture
        task.filename = export_file
        task.automated = True
        task.prompt = False
        task.replace_identical = True
        task.use_file_archive = False
        task.write_empty_files = False

        try:
            ok = unreal.Exporter.run_asset_export_task(task)
            if not ok or not os.path.exists(export_file):
                failures.append({"asset": object_path, "reason": "export_failed"})
                continue
        except Exception as exc:
            failures.append({"asset": object_path, "reason": str(exc)})
            continue

        manifest.append(
            {
                "asset_path": object_path,
                "destination_path": destination_path,
                "destination_name": destination_name,
                "before_width": width,
                "before_height": height,
                "before_disk_size": disk_size,
                "new_width": new_w,
                "new_height": new_h,
                "export_file": export_file,
                "resized_file": resized_file,
            }
        )
        index += 1

    manifest_path = os.path.join(batch_dir, "texture_resize_manifest.json")
    with open(manifest_path, "w", encoding="utf-8") as f:
        json.dump(manifest, f, ensure_ascii=False, indent=2)

    failures_path = os.path.join(batch_dir, "texture_resize_failures.json")
    with open(failures_path, "w", encoding="utf-8") as f:
        json.dump(failures, f, ensure_ascii=False, indent=2)

    unreal.log(f"[TextureResize] Manifest entries: {len(manifest)}")
    unreal.log(f"[TextureResize] Failures: {len(failures)}")
    unreal.log(f"[TextureResize] Manifest: {manifest_path}")
    unreal.log(f"[TextureResize] Failures: {failures_path}")


if __name__ == "__main__":
    main()
