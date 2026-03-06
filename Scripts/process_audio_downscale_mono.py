#!/usr/bin/env python3
"""
Обработка всех звуков в папке: даунскейл (понижение частоты дискретизации) и конвертация в моно.
Результат сохраняется на место исходных файлов (перезапись). После запуска можно сделать
Reimport в Unreal Editor для обновления ассетов.

Использование:
  python process_audio_downscale_mono.py [путь_к_папке] [--rate 22050] [--dry-run]

  путь_к_папке  — корень, в котором искать все .wav (по умолчанию: Content проекта)
  --rate 22050   — целевая частота дискретизации (по умолчанию 22050 Гц)
  --dry-run      — только показать список файлов, не изменять

Требования: pip install soundfile numpy scipy
"""

from pathlib import Path
import sys
import argparse

try:
    import numpy as np
    import soundfile as sf
    from scipy.io import wavfile as scipy_wav
    from scipy.signal import resample_poly
except ImportError as e:
    print("Установите зависимости: pip install soundfile numpy scipy")
    raise SystemExit(1) from e


def _read_wav_ue_style(path: Path):
    """Читает WAV с двумя чанками 'data' (как при экспорте из Unreal): первый пустой, второй — сэмплы."""
    raw = path.read_bytes()
    n = len(raw)
    # Найти "fmt " и считать sr, channels, bits
    fmt_pos = raw.find(b"fmt ")
    if fmt_pos < 0:
        raise ValueError("No 'fmt ' chunk")
    channels = int.from_bytes(raw[fmt_pos + 10 : fmt_pos + 12], "little")
    sr = int.from_bytes(raw[fmt_pos + 12 : fmt_pos + 16], "little")
    bits = int.from_bytes(raw[fmt_pos + 22 : fmt_pos + 24], "little")
    # Найти чанк "data" с ненулевым размером (UE иногда пишет пустой data, потом полный)
    pos = 0
    while True:
        pos = raw.find(b"data", pos)
        if pos < 0:
            raise ValueError("No 'data' chunk with payload")
        size = int.from_bytes(raw[pos + 4 : pos + 8], "little")
        if size > 0:
            payload = raw[pos + 8 : pos + 8 + size]
            break
        pos += 4
    # Декод PCM
    if bits == 16:
        samples = np.frombuffer(payload, dtype=np.int16)
    elif bits == 32:
        samples = np.frombuffer(payload, dtype=np.int32)
    else:
        raise ValueError(f"Unsupported bits per sample: {bits}")
    samples = samples.astype(np.float32) / (32768.0 if bits == 16 else 2147483648.0)
    if channels > 1:
        samples = samples.reshape(-1, channels)
    return samples, sr


def _read_wav(path: Path):
    """Читает WAV, поддерживая стандартный формат и экспорт Unreal (два чанка data)."""
    try:
        data, sr = sf.read(path, dtype="float32")
        return data, sr
    except Exception:
        pass
    try:
        sr, data = scipy_wav.read(path)
        if data.dtype == np.int16:
            data = data.astype(np.float32) / 32768.0
        elif data.dtype == np.int32:
            data = data.astype(np.float32) / 2147483648.0
        else:
            data = data.astype(np.float32)
        return data, sr
    except Exception:
        pass
    return _read_wav_ue_style(path)


def process_file(path: Path, target_sr: int, dry_run: bool) -> bool:
    """Обработать один WAV: моно + ресэмплинг. Сохранить на место."""
    try:
        data, sr = _read_wav(path)
    except Exception as e:
        print(f"  Ошибка чтения {path}: {e}")
        return False

    # Моно: усреднение каналов
    if data.ndim > 1:
        data = data.mean(axis=1)

    # Ресэмплинг при необходимости
    if sr != target_sr:
        # resample_poly(data, up, down) => new_sr = sr * up / down
        # target_sr = sr * up / down  => up/down = target_sr / sr
        g = np.gcd(int(sr), int(target_sr))
        up = target_sr // g
        down = sr // g
        data = resample_poly(data, up, down).astype(np.float32)

    if dry_run:
        ch = "mono" if data.ndim == 1 else f"{data.shape[1]} ch"
        print(f"  [dry-run] {path}  {sr} Hz -> {target_sr} Hz, {ch}")
        return True

    try:
        sf.write(path, data, target_sr, subtype="PCM_16")
    except Exception as e:
        print(f"  Ошибка записи {path}: {e}")
        return False
    return True


def main():
    script_dir = Path(__file__).resolve().parent
    default_root = script_dir.parent / "Content"

    ap = argparse.ArgumentParser(description="Downscale + mono для всех WAV в папке")
    ap.add_argument("path", nargs="?", type=Path, default=default_root, help="Корневая папка для поиска .wav")
    ap.add_argument("--rate", type=int, default=22050, help="Целевая частота дискретизации (Гц)")
    ap.add_argument("--dry-run", action="store_true", help="Только показать файлы, не изменять")
    args = ap.parse_args()

    root = args.path.resolve()
    if not root.is_dir():
        print(f"Папка не найдена: {root}")
        return 1

    wavs = list(root.rglob("*.wav"))
    if not wavs:
        print(f"В папке {root} не найдено ни одного .wav файла.")
        print("Экспортируйте звуки из Unreal (Content Browser -> папка Sounds -> ПКМ -> Asset Actions -> Export),")
        print("укажите эту скрипту папку с экспортированными файлами и запустите снова.")
        return 1

    print(f"Найдено .wav: {len(wavs)}. Целевой sample rate: {args.rate} Hz. Dry-run: {args.dry_run}")
    ok = 0
    for p in wavs:
        if process_file(p, args.rate, args.dry_run):
            ok += 1
        if not args.dry_run and ok % 20 == 0 and ok:
            print(f"  обработано {ok}...")

    print(f"Готово: {ok}/{len(wavs)} файлов.")
    if not args.dry_run and ok:
        print("В Unreal: выберите ассеты или папку и выполните Reimport (ПКМ -> Reimport).")
    return 0


if __name__ == "__main__":
    sys.exit(main())
