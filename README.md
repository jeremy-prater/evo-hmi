# evo-hmi

Qt 6 Quick infotainment shell for the Evo head unit (i.MX6, `eglfs`).

Screens: engine gauges, OSM navigation, A2DP media, settings.

`evo-ecu-rs` stays a separate process. This app talks to it later over a length-prefixed protobuf Unix socket (`/tmp/evo-ecu.sock`, `/run/evo-ecu.sock` on the box). The ECU client is **fake** until that API settles.

## Build

Needs Qt 6.5+ (`Core`, `Gui`, `Qml`, `Quick`, `Network`). Optional: `DBus` (BlueZ on Linux), libosmscout (map paint + search), Valhalla (routing tiles).

```bash
cmake -S evo-hmi -B evo-hmi/build -DCMAKE_BUILD_TYPE=Release \
    -DQt6_DIR="$(brew --prefix qtbase)/lib/cmake/Qt6" \
    -DQt6Qml_DIR="$(brew --prefix qtdeclarative)/lib/cmake/Qt6Qml" \
    -DQt6Quick_DIR="$(brew --prefix qtdeclarative)/lib/cmake/Qt6Quick" \
    -DQt6QuickControls2_DIR="$(brew --prefix qtdeclarative)/lib/cmake/Qt6QuickControls2" \
    -DQt6Svg_DIR="$(brew --prefix qtsvg)/lib/cmake/Qt6Svg"
cmake --build evo-hmi/build
./evo-hmi/build/evo-hmi --fake-gps --fake-bt
```

## UI

Screens live in `qml/` (1024×600: splash → dashboard tiles → section screens). Edit them in Qt Design Studio:

```bash
open -a "Qt Design Studio" evo-hmi/EvoHmi.qmlproject
```

On Linux with a desktop session it uses xcb/wayland. On the i.MX6 with no `DISPLAY` / `WAYLAND_DISPLAY`, `main` selects `eglfs`.

```bash
./evo-hmi --platform eglfs
# vendor Vivante:  QT_QPA_EGLFS_INTEGRATION=eglfs_viv
# mainline etnaviv: QT_QPA_EGLFS_INTEGRATION=eglfs_kms
```

## Flags

| Flag | Meaning |
|---|---|
| `--fake-gps` | Ignore gpsd, crawl a simulated fix |
| `--fake-bt` | Simulated pairing + now-playing |
| `--gpsd-host` / `--gpsd-port` | Default `127.0.0.1:2947` |
| `--map-dir` | libosmscout database directory (paint + search) |

Without `--fake-gps`, the app connects to gpsd JSON (`?WATCH`) and falls back to simulation if the daemon is down.

Bluetooth: on Linux with Qt DBus it uses BlueZ `ObjectManager`, `Device1`, and `MediaPlayer1` (AVRCP). A2DP audio itself is PipeWire/PulseAudio, not this process.

Maps: see [docs/MAPS.md](docs/MAPS.md).
