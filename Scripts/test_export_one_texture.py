import os
import unreal


ASSET = "/Game/Characters/Mannequins/Textures/Manny/T_Manny_02_BN.T_Manny_02_BN"
OUT_DIR = r"E:\UE_Projects\STU\Builded\TextureExportTest"
OUT_FILE = os.path.join(OUT_DIR, "T_Manny_02_BN.png")


def main():
    os.makedirs(OUT_DIR, exist_ok=True)
    tex = unreal.EditorAssetLibrary.load_asset(ASSET)
    if not tex:
        raise RuntimeError(f"Failed to load asset: {ASSET}")

    task = unreal.AssetExportTask()
    task.object = tex
    task.filename = OUT_FILE
    task.automated = True
    task.prompt = False
    task.replace_identical = True
    task.use_file_archive = False
    task.write_empty_files = False

    ok = unreal.Exporter.run_asset_export_task(task)
    unreal.log(f"Export result: {ok}")
    unreal.log(f"Export file exists: {os.path.exists(OUT_FILE)} -> {OUT_FILE}")


if __name__ == "__main__":
    main()
