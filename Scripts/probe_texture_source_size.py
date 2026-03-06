import unreal


def main():
    registry = unreal.AssetRegistryHelpers.get_asset_registry()
    texture_class_path = unreal.TopLevelAssetPath("/Script/Engine", "Texture2D")
    asset_filter = unreal.ARFilter(package_paths=["/Game"], class_paths=[texture_class_path], recursive_paths=True)
    assets = registry.get_assets(asset_filter)
    if not assets:
        unreal.log("No textures found")
        return
    tex = assets[0].get_asset()
    info = tex.blueprint_get_texture_source_disk_and_memory_size()
    unreal.log(f"Texture: {tex.get_path_name()}")
    unreal.log(f"Source size info type: {type(info)}")
    unreal.log(f"Source size info value: {info}")


if __name__ == "__main__":
    main()
