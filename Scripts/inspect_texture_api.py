import unreal


def main():
    registry = unreal.AssetRegistryHelpers.get_asset_registry()
    texture_class_path = unreal.TopLevelAssetPath("/Script/Engine", "Texture2D")
    asset_filter = unreal.ARFilter(
        package_paths=["/Game"],
        class_paths=[texture_class_path],
        recursive_paths=True,
    )
    assets = registry.get_assets(asset_filter)
    if not assets:
        unreal.log("No Texture2D assets found.")
        return

    tex = assets[0].get_asset()
    unreal.log(f"Inspecting texture: {tex.get_path_name()}")

    names = sorted([name for name in dir(tex) if ("source" in name.lower() or "resize" in name.lower() or "mip" in name.lower())])
    for name in names:
        unreal.log(f"Texture API: {name}")

    source = tex.get_editor_property("source")
    unreal.log(f"Source object type: {type(source)}")
    source_names = sorted([name for name in dir(source) if ("size" in name.lower() or "resize" in name.lower() or "data" in name.lower() or "set" in name.lower() or "init" in name.lower())])
    for name in source_names:
        unreal.log(f"Source API: {name}")


if __name__ == "__main__":
    main()
