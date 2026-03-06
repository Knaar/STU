import json
import os
import unreal


def main():
    project_dir = unreal.Paths.project_dir()
    batch_dir = os.path.join(project_dir, "Builded", "TextureResizeBatch")
    manifest_path = os.path.join(batch_dir, "texture_resize_manifest.json")

    if not os.path.exists(manifest_path):
        raise RuntimeError(f"Manifest not found: {manifest_path}")

    with open(manifest_path, "r", encoding="utf-8") as f:
        manifest = json.load(f)

    asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
    imported = 0
    failed = []

    for entry in manifest:
        resized_file = entry["resized_file"]
        if not os.path.exists(resized_file):
            failed.append({"asset": entry["asset_path"], "reason": "resized_file_missing"})
            continue

        task = unreal.AssetImportTask()
        task.filename = resized_file
        task.destination_path = entry["destination_path"]
        task.destination_name = entry["destination_name"]
        task.automated = True
        task.replace_existing = True
        # Keep existing texture settings (compression, groups, etc.)
        task.replace_existing_settings = False
        task.save = True

        try:
            asset_tools.import_asset_tasks([task])
            texture = unreal.EditorAssetLibrary.load_asset(entry["asset_path"])
            if texture:
                unreal.EditorAssetLibrary.save_loaded_asset(texture)
            imported += 1
        except Exception as exc:
            failed.append({"asset": entry["asset_path"], "reason": str(exc)})

    failed_path = os.path.join(batch_dir, "texture_reimport_failures.json")
    with open(failed_path, "w", encoding="utf-8") as f:
        json.dump(failed, f, ensure_ascii=False, indent=2)

    unreal.log(f"[TextureResize] Reimported: {imported}")
    unreal.log(f"[TextureResize] Failed: {len(failed)}")
    unreal.log(f"[TextureResize] Failure report: {failed_path}")


if __name__ == "__main__":
    main()
