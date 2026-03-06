# Обработка текстур: даунскейл

Скрипт `process_textures_downscale.py` находит все изображения (PNG, TGA, JPG, BMP) в указанной папке, уменьшает разрешение и **перезаписывает файлы на место**. После этого в Unreal Editor нужно сделать Reimport для обновления ассетов (или использовать `ue_reimport_selected.py` для выбранных текстур — Interchange поддерживает реимпорт текстур).

## Шаги

### 1. Зависимости

```bash
pip install Pillow
# или
pip install -r requirements.txt
```

### 2. Экспорт текстур из Unreal Editor

- В **Content Browser** выдели папки/текстуры.
- **ПКМ → Asset Actions → Export...**
- Укажи папку, например: `e:\UE_Projects\STU\ExportedTextures`.

### 3. Запуск скрипта

**Ограничить максимальную сторону (например 1024 px):**

```bash
python process_textures_downscale.py "e:\UE_Projects\STU\ExportedTextures" --max-size 1024
```

**Уменьшить в 2 раза по каждой оси:**

```bash
python process_textures_downscale.py "e:\UE_Projects\STU\ExportedTextures" --scale 0.5
```

**Только посмотреть список (без изменений):**

```bash
python process_textures_downscale.py "e:\UE_Projects\STU\ExportedTextures" --max-size 1024 --dry-run
```

Если путь не указан, скрипт ищет изображения в `Content` проекта.

### 4. Reimport в Unreal

- Выдели экспортированные текстуры в Content Browser → **ПКМ → Reimport**.

Или запусти в редакторе (Output Log → Cmd):

```text
py "E:/UE_Projects/STU/Scripts/ue_reimport_selected.py"
```

(предварительно выделив текстуры; для текстур Interchange обычно поддерживает Reimport).
