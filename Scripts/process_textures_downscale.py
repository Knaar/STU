#!/usr/bin/env python3
"""
Обработка всех текстур в папке: даунскейл (уменьшение разрешения).
Результат сохраняется на место исходных файлов (перезапись). После запуска
сделай Reimport в Unreal Editor или используй ue_reimport_selected.py для выбранных ассетов.

Использование:
  python process_textures_downscale.py [путь_к_папке] [--max-size 1024] [--dry-run]
  python process_textures_downscale.py [путь_к_папке] [--scale 0.5] [--dry-run]

  путь_к_папке   — корень, в котором искать изображения (по умолчанию: Content проекта)
  --max-size N   — максимальная сторона (ширина или высота) в пикселях; пропорции сохраняются
  --scale F      — множитель разрешения (0.5 = половина по каждой оси). Игнорируется, если задан --max-size
  --dry-run      — только показать список файлов, не изменять

Поддерживаются: .png, .tga, .jpg, .jpeg, .bmp (и те же в верхнем регистре).

Требования: pip install Pillow
"""

from pathlib import Path
import sys
import argparse

try:
    from PIL import Image
except ImportError:
    print("Установите Pillow: pip install Pillow")
    raise SystemExit(1)


EXTENSIONS = {".png", ".tga", ".jpg", ".jpeg", ".bmp"}
EXTENSIONS |= {e.upper() for e in EXTENSIONS}


def downscale_image(path: Path, max_side: int | None, scale: float, dry_run: bool) -> bool:
    """Уменьшить изображение: по max_side или по scale. Сохранить на место."""
    try:
        img = Image.open(path)
    except Exception as e:
        print(f"  Ошибка чтения {path}: {e}")
        return False

    if img.mode in ("RGBA", "P"):
        img = img.convert("RGBA")
    elif img.mode != "RGB":
        img = img.convert("RGB")

    w, h = img.size
    if max_side is not None:
        if w <= max_side and h <= max_side:
            if dry_run:
                print(f"  [dry-run] {path}  {w}x{h} (already <= {max_side})")
            return True
        if w >= h:
            new_w = max_side
            new_h = max(1, round(h * max_side / w))
        else:
            new_h = max_side
            new_w = max(1, round(w * max_side / h))
    else:
        new_w = max(1, round(w * scale))
        new_h = max(1, round(h * scale))

    if dry_run:
        print(f"  [dry-run] {path}  {w}x{h} -> {new_w}x{new_h}")
        return True

    try:
        resized = img.resize((new_w, new_h), Image.Resampling.LANCZOS)
        kwargs = {}
        if path.suffix.lower() in (".jpg", ".jpeg"):
            kwargs["quality"] = 95
        resized.save(path, **kwargs)
    except Exception as e:
        print(f"  Ошибка записи {path}: {e}")
        return False
    return True


def main():
    script_dir = Path(__file__).resolve().parent
    default_root = script_dir.parent / "Content"

    ap = argparse.ArgumentParser(description="Downscale текстур в папке")
    ap.add_argument("path", nargs="?", type=Path, default=default_root, help="Корневая папка для поиска изображений")
    ap.add_argument("--max-size", type=int, default=None, help="Максимальная сторона в пикселях (например 1024)")
    ap.add_argument("--scale", type=float, default=0.5, help="Множитель разрешения (по умолчанию 0.5)")
    ap.add_argument("--dry-run", action="store_true", help="Только показать, не изменять")
    args = ap.parse_args()

    if args.max_size is None and (args.scale <= 0 or args.scale >= 1):
        print("Укажите --max-size N или --scale от 0 до 1 (например --scale 0.5)")
        return 1

    root = args.path.resolve()
    if not root.is_dir():
        print(f"Папка не найдена: {root}")
        return 1

    files = []
    for ext in EXTENSIONS:
        files.extend(root.rglob(f"*{ext}"))

    if not files:
        print(f"В папке {root} не найдено изображений (.png, .tga, .jpg, .jpeg, .bmp).")
        print("Экспортируйте текстуры из Unreal (Content Browser -> папка -> ПКМ -> Asset Actions -> Export),")
        print("укажите скрипту папку с экспортом и запустите снова.")
        return 1

    mode = f"max-size={args.max_size}" if args.max_size else f"scale={args.scale}"
    print(f"Найдено изображений: {len(files)}. Режим: {mode}. Dry-run: {args.dry_run}")
    ok = 0
    for p in files:
        if downscale_image(p, args.max_size, args.scale, args.dry_run):
            ok += 1
        if not args.dry_run and ok and ok % 20 == 0:
            print(f"  обработано {ok}...")

    print(f"Готово: {ok}/{len(files)} файлов.")
    if not args.dry_run and ok:
        print("В Unreal: выдели текстуры и Reimport (ПКМ -> Reimport) или запусти ue_reimport_selected.py.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
