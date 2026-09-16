# Offline OSM maps (libosmscout)

The map screen always shows GPS position, a route polyline, and turn-by-turn chrome.

When **libosmscout** is found at CMake time (`osmscout`, `osmscoutmap`, optionally `osmscoutmapqt`), the HMI:

- Renders a local vector database through `ScoutMapItem` (`MapPainterQt`)
- Geocodes search and calculates routes with `SimpleRoutingService`
- Still falls back to the simulated router if the database cannot be opened

If CMake does not find the library, you get the canvas map plus simulated routing. That is expected on a desktop without map data.

## Install libosmscout

```bash
git clone https://github.com/Framstag/libosmscout.git
cmake -S libosmscout -B libosmscout/build \
    -DCMAKE_BUILD_TYPE=Release \
    -DOSMSCOUT_BUILD_TOOL_IMPORT=ON \
    -DOSMSCOUT_BUILD_CLIENT_QT=ON \
    -DOSMSCOUT_BUILD_MAP_QT=ON
cmake --build libosmscout/build -j
sudo cmake --install libosmscout/build
```

Reconfigure evo-hmi so `LibOsmScout_FOUND` is true.

## Import a regional extract

Do **not** import the whole planet on i.MX6. Use a state/metro extract from [Geofabrik](https://download.geofabrik.de/).

```bash
wget https://download.geofabrik.de/north-america/us/california-latest.osm.pbf
mkdir -p ~/.local/share/evo-hmi/maps
Import \
    --typefile <libosmscout>/stylesheets/map.ost \
    --destinationDirectory ~/.local/share/evo-hmi/maps \
    california-latest.osm.pbf
cp <libosmscout>/stylesheets/standard.oss ~/.local/share/evo-hmi/maps/
```

Point the HMI at that directory:

```bash
./evo-hmi --map-dir ~/.local/share/evo-hmi/maps
# or: export EVO_HMI_MAP_DIR=$HOME/.local/share/evo-hmi/maps
```

Keep RAM in mind: a California extract is already large for i.MX6. Prefer a metro extract (Los Angeles, etc.) for the car.

## gpsd

```text
gpsd -N /dev/ttyACM0
# HMI default: 127.0.0.1:2947 JSON WATCH
```
