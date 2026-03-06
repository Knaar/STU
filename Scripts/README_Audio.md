# Обработка звуков: даунскейл + моно

Скрипт находит все `.wav` в указанной папке, понижает частоту дискретизации (по умолчанию до 22050 Гц), переводит в моно и **перезаписывает файлы на место**. После этого в Unreal Editor нужно сделать Reimport для обновления ассетов.

## Почему так

В проекте звуки хранятся как `.uasset` (SoundWave/SoundCue). Исходных `.wav` в репозитории нет — они были импортированы извне (например, из пака). Чтобы «вернуть туда же с реимпортом», нужно:

1. Получить из проекта исходные WAV (экспорт из редактора).
2. Обработать их этим скриптом (даунскейл + моно).
3. Вернуть обработанные файлы в проект и выполнить Reimport в редакторе.

## Шаги

### 1. Установка зависимостей

```bash
cd e:\UE_Projects\STU\Scripts
pip install -r requirements.txt
```

### 2. Экспорт звуков из Unreal Editor

- Откройте проект в редакторе.
- В **Content Browser** перейдите в папку с звуками (например `Content/ExternalContent/Sounds` и/или `Content/Sounds`).
- Выделите нужные папки/ассеты (или всё дерево звуков).
- **ПКМ → Asset Actions → Export...** (или **File → Export...**).
- Укажите папку, например: `e:\UE_Projects\STU\ExportedSounds`.
- Дождитесь окончания экспорта (появится набор `.wav` и др.).

### 3. Запуск скрипта

Обработка **всех** `.wav` в папке экспорта (по умолчанию — моно, 22050 Гц, перезапись файлов):

```bash
python process_audio_downscale_mono.py "e:\UE_Projects\STU\ExportedSounds"
```

Только посмотреть, что будет обработано, без изменений:

```bash
python process_audio_downscale_mono.py "e:\UE_Projects\STU\ExportedSounds" --dry-run
```

Другая целевая частота (например 44100 Гц):

```bash
python process_audio_downscale_mono.py "e:\UE_Projects\STU\ExportedSounds" --rate 44100
```

Если не передать путь, скрипт ищет `.wav` в `Content` проекта:

```bash
python process_audio_downscale_mono.py
```

### 4. Reimport в Unreal Editor

- В **Content Browser** найдите ассеты, которые экспортировали и обработали.
- Выделите их (или родительские папки).
- **ПКМ → Reimport** (или через **File → Reimport** для выбранного ассета).

Редактор возьмёт файлы по путям исходников. Если вы экспортировали в `ExportedSounds`, а реимпорт ищет файлы «на месте», можно:

- **Вариант A:** после экспорта скопировать обработанные файлы из `ExportedSounds` в те же относительные пути, откуда редактор изначально импортировал (если они вам известны), и затем сделать Reimport.
- **Вариант B:** удалить старые звуковые ассеты в контенте и заново импортировать из `ExportedSounds` (**Import into /Game/...**), затем заново назначить Sound Cues / ссылки в синих принтах и т.д.

После Reimport звуки в проекте будут в моно и с пониженной частотой дискретизации.

---

## Массовый Reimport из Python (ue_reimport_selected.py)

Скрипт `ue_reimport_selected.py` выполняет Reimport выбранных ассетов или ассетов в указанной папке **внутри Unreal Editor** (Python API редактора).

**Важно:** InterchangeManager поддерживает не все типы ассетов. SoundWave может не поддерживаться — в таком случае используйте ручной Reimport (ПКМ -> Reimport).

### Запуск

1. Включите **Python Editor Script Plugin** (Edit -> Plugins -> Python Editor Script).
2. Откройте проект в Unreal Editor.
3. **Вариант A** — по выделенным ассетам:
   - В Content Browser выделите ассеты.
   - Window -> Developer Tools -> Output Log (Ctrl+Shift+O).
   - В поле Cmd введите: `py "E:/UE_Projects/STU/Scripts/ue_reimport_selected.py"`
4. **Вариант B** — по пути к папке:
   - В поле Cmd введите: `py "E:/UE_Projects/STU/Scripts/ue_reimport_selected.py" /Game/NewExport`
