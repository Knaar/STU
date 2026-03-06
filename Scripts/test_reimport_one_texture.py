import unreal


ASSET_PATH = "/Game/Characters/Mannequins/Textures/Manny/T_Manny_02_BN.T_Manny_02_BN"
IMPORT_FILE = r"E:\UE_Projects\STU\Builded\TextureExportTest\T_Manny_02_BN_resized.png"

RESTORE_PROPS = [
    "compression_settings",
    "compression_no_alpha",
    "compression_quality",
    "compression_ycbcr",
    "srgb",
    "lod_group",
    "mip_gen_settings",
    "never_stream",
    "virtual_texture_streaming",
    "filter",
    "address_x",
    "address_y",
    "max_texture_size",
]


def snapshot_props(texture):
    values = {}
    for prop in RESTORE_PROPS:
        try:
            values[prop] = texture.get_editor_property(prop)
        except Exception:
            pass
    return values


def restore_props(texture, values):
    for prop, value in values.items():
        try:
            texture.set_editor_property(prop, value)
        except Exception:
            pass


def main():
    texture = unreal.EditorAssetLibrary.load_asset(ASSET_PATH)
    if not texture:
        raise RuntimeError(f"Failed to load {ASSET_PATH}")

    before_disk, before_mem = texture.blueprint_get_texture_source_disk_and_memory_size()
    before_w = texture.blueprint_get_size_x()
    before_h = texture.blueprint_get_size_y()
    props = snapshot_props(texture)

    package_path, asset_name = ASSET_PATH.rsplit(".", 1)[0].rsplit("/", 1)
    import_task = unreal.AssetImportTask()
    import_task.filename = IMPORT_FILE
    import_task.destination_path = package_path
    import_task.destination_name = asset_name
    import_task.automated = True
    import_task.replace_existing = True
    import_task.replace_existing_settings = True
    import_task.save = True

    asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
    asset_tools.import_asset_tasks([import_task])

    texture = unreal.EditorAssetLibrary.load_asset(ASSET_PATH)
    restore_props(texture, props)
    unreal.EditorAssetLibrary.save_loaded_asset(texture)

    after_disk, after_mem = texture.blueprint_get_texture_source_disk_and_memory_size()
    after_w = texture.blueprint_get_size_x()
    after_h = texture.blueprint_get_size_y()

    unreal.log(f"Before: {before_w}x{before_h}, disk={before_disk}, mem={before_mem}")
    unreal.log(f"After:  {after_w}x{after_h}, disk={after_disk}, mem={after_mem}")


if __name__ == "__main__":
    main()
