#!/usr/bin/env python3
"""
Скрипт для Unreal Editor: массовый Reimport выбранных ассетов (звуки, текстуры и т.д.).
Для SoundWave: переимпорт из папки NewExport (обработанные WAV) поверх существующих ассетов.

Запускать из Output Log -> Cmd (py) или через Window -> Python Console в редакторе UE.

Использование в редакторе:
  1. Открой проект в Unreal Editor
  2. В Content Browser выдели ассеты или папку с ассетами
  3. Window -> Developer Tools -> Output Log (или Ctrl+Shift+O)
  4. В поле Cmd введи: py "E:/UE_Projects/STU/Scripts/ue_reimport_selected.py"

  С указанием папки с обработанными WAV (по умолчанию NewExport в корне проекта):
  py "E:/UE_Projects/STU/Scripts/ue_reimport_selected.py" "E:/UE_Projects/STU/NewExport"
  py "E:/UE_Projects/STU/Scripts/ue_reimport_selected.py" /Game/ExternalContent/Sounds
"""

import unreal
import sys
import os


def get_selected_asset_paths():
    """Получить пути к выбранным ассетам в Content Browser (UE 5.5+: classmethod)."""
    asset_data_list = unreal.EditorUtilityLibrary.get_selected_asset_data()
    paths = []
    for ad in asset_data_list:
        # UE 5.6: AssetData имеет package_name и asset_name, object_path удалён
        pkg = getattr(ad, "package_name", None) or getattr(ad, "object_path", None)
        name = getattr(ad, "asset_name", None)
        if pkg is not None and name is not None:
            path = f"{pkg}.{name}"
        else:
            path = str(getattr(ad, "object_path", pkg))
        paths.append(str(path))
    return paths


def get_assets_in_path(content_path, recursive=True):
    """Получить все ассеты в указанной папке (например /Game/NewExport)."""
    registry = unreal.AssetRegistryHelpers.get_asset_registry()
    filter = unreal.ARFilter(
        package_paths=[unreal.Name(content_path)],
        recursive_paths=recursive,
        class_paths=[],
    )
    assets = registry.get_assets(filter)
    paths = []
    for a in assets:
        pkg = getattr(a, "package_name", None) or getattr(a, "object_path", None)
        name = getattr(a, "asset_name", None)
        if pkg is not None and name is not None:
            path = f"{pkg}.{name}"
        else:
            path = str(getattr(a, "object_path", pkg))
        paths.append(str(path))
    return paths


def _get_newexport_root():
    """Путь к папке с обработанными WAV (NewExport). Можно задать вторым аргументом: py script.py /Game/Path \"D:/NewExport\"."""
    if len(sys.argv) >= 3 and ("/" in sys.argv[2] or "\\" in sys.argv[2]):
        root = sys.argv[2].replace("/", os.sep).rstrip(os.sep)
        if os.path.isabs(root) or root.startswith("."):
            return root
    try:
        project_dir = unreal.Paths.project_dir()
        if project_dir:
            return os.path.join(project_dir.rstrip("/\\"), "NewExport")
    except Exception:
        pass
    return os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "NewExport")


def _reimport_soundwave_from_file(asset_tools, obj, path_str, wav_path):
    """Переимпорт одного SoundWave из файла WAV поверх существующего ассета (AssetImportTask)."""
    if not os.path.isfile(wav_path):
        return False
    # path_str = /Game/Folder/AssetName.AssetName -> destination_path = /Game/Folder, destination_name = AssetName
    if "." in path_str:
        package_path = path_str.rsplit(".", 1)[0]
    else:
        package_path = path_str
    folder = package_path.rsplit("/", 1)[0] if "/" in package_path else "/Game"
    name = package_path.rsplit("/", 1)[-1] if "/" in package_path else package_path
    task = unreal.AssetImportTask()
    task.filename = wav_path
    task.destination_path = folder
    task.destination_name = name
    task.replace_existing = True
    task.replace_existing_settings = True
    task.automated = True
    task.save = True
    asset_tools.import_asset_tasks([task])
    return len(task.imported_paths) > 0 if hasattr(task, "imported_paths") else True


def reimport_assets(paths, newexport_root=None):
    """Реимпорт: Interchange, затем AssetTools.reimport, для SoundWave — переимпорт из NewExport через AssetImportTask."""
    ok = 0
    fail = 0
    interchange = unreal.InterchangeManager.get_interchange_manager_scripted()
    asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
    if newexport_root is None:
        newexport_root = _get_newexport_root()

    for path in paths:
        obj = unreal.load_asset(path)
        if not obj:
            unreal.log_warning(f"Could not load: {path}")
            fail += 1
            continue

        done = False

        # 1) InterchangeManager
        try:
            if interchange.can_reimport(obj):
                params = unreal.ImportAssetParameters()
                params.is_automated = True
                interchange.reimport_asset(obj, params)
                unreal.log(f"Reimport OK (Interchange): {path}")
                ok += 1
                done = True
        except Exception as e:
            pass

        if done:
            continue

        # 2) AssetTools.reimport (если есть в API)
        try:
            if hasattr(asset_tools, "reimport") and asset_tools.reimport(obj):
                unreal.log(f"Reimport OK (AssetTools): {path}")
                ok += 1
                done = True
        except Exception:
            pass

        if done:
            continue

        # 3) SoundWave: переимпорт из NewExport (структура как при экспорте: NewExport/Game/.../Name.WAV)
        if isinstance(obj, unreal.SoundWave):
            package_path = path.rsplit(".", 1)[0] if "." in path else path
            relative = ("Game" + package_path[5:]) if package_path.startswith("/Game") else package_path.strip("/")
            relative = relative.replace("/", os.sep)
            wav_path = os.path.join(newexport_root, relative + ".WAV")
            if not os.path.isfile(wav_path):
                wav_path = os.path.join(newexport_root, relative + ".wav")
            if os.path.isfile(wav_path):
                try:
                    if _reimport_soundwave_from_file(asset_tools, obj, path, wav_path):
                        unreal.log(f"Reimport OK (Import from file): {path}")
                        ok += 1
                        done = True
                except Exception as e:
                    unreal.log_warning(f"Import from file failed {path}: {e}")
            else:
                unreal.log_warning(f"WAV not found in NewExport: {relative}.WAV")

        if not done:
            unreal.log_warning(f"Reimport not supported: {path}")
            fail += 1

    return ok, fail


def main():
    paths = []

    # Аргумент: путь к папке, напр. /Game/NewExport
    if len(sys.argv) >= 2:
        content_path = sys.argv[1]
        if not content_path.startswith("/Game"):
            content_path = "/Game/" + content_path.lstrip("/")
        paths = get_assets_in_path(content_path)
        unreal.log(f"Found {len(paths)} assets in {content_path}")
    else:
        paths = get_selected_asset_paths()
        unreal.log(f"Selected {len(paths)} assets")

    if not paths:
        unreal.log_error("No assets to reimport. Select assets in Content Browser or pass path: py ue_reimport_selected.py /Game/NewExport")
        return

    ok, fail = reimport_assets(paths)
    unreal.log(f"Reimport done: {ok} OK, {fail} failed/skipped")


if __name__ == "__main__":
    main()
