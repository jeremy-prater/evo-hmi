# Maps and routing

The map screen always shows GPS position, a route polyline, and turn-by-turn chrome.

**Paint and search** are libosmscout (seed extract on the SD card). **Routing** is Valhalla, from a `tile_dir` that grows as zones are installed. This process does not download OSM, does not run `valhalla_build_tiles`, and does not call [evo-nav-server](../../evo-nav-server/README.md). A separate on-box maps agent does that over the phone hotspot.

Until Valhalla is linked, `OsmScoutNavEngine` still routes with `SimpleRoutingService`. Canvas + simulated routing is the fallback when neither backend has data.

## Split

| Piece | Owns |
|---|---|
| evo-hmi | Paint, geocode, `actor_t` route/guidance, watch local catalog |
| maps-agent | Lookahead zone ids, HTTP to the server, pack install onto SD |
| evo-nav-server | Cache-or-build: OSM download + `valhalla_build_tiles` |

A zone is a Valhalla level-2 cell (`2:{tile_id}`, 0.25°) plus parent level-0/1 `.gph` files. First-time builds take minutes; the agent requests **lookahead** (current cell + heading/route), not the cell just entered. Zones already on the card are never requested again.

libosmscout paint does **not** grow with Valhalla zones. Outside the seed extract the map is canvas; routing can still follow installed graph tiles.

## SD card (32GB)

```text
/media/sd/evo-hmi/
  osmscout/                 # --map-dir  (paint + search seed)
  valhalla/
    valhalla.json           # tile_dir, LRU 64–128 MB, no tile_extract tar
    tiles/0/ …              # merged graph; grows per zone
    tiles/1/
    tiles/2/
  catalog.json              # installed / downloading / wanted zone ids
  staging/                  # agent in-flight packs
```

Desktop without a card: `--map-dir` / `EVO_HMI_MAP_DIR` for osmscout; `--valhalla-dir` / `EVO_HMI_VALHALLA_DIR` for the Valhalla directory (json + `tiles/`).

Open Valhalla as a **tile directory** only. Do not mmap a `.tar`. Do not import the planet on the i.MX6. LRU cache ~64–128 MB. If a destination is outside installed zones, fail the route and let the agent request the missing id — do not let A\* walk a hole.

`catalog.json` is what the HMI watches (`QFileSystemWatcher`). New `.gph` files are used on the next `navigateTo`.

## libosmscout (paint + search)

When CMake finds `osmscout`, `osmscoutmap`, optionally `osmscoutmapqt`:

- `ScoutMapItem` paints the local vector database (`MapPainterQt`)
- Search uses `LocationService`
- If the database cannot be opened, paint falls back to canvas

### Install

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

### Seed extract

Use a metro extract from [Geofabrik](https://download.geofabrik.de/), not California-wide, for the car.

```bash
wget https://download.geofabrik.de/north-america/us/california-latest.osm.pbf
mkdir -p ~/.local/share/evo-hmi/maps
Import \
    --typefile <libosmscout>/stylesheets/map.ost \
    --destinationDirectory ~/.local/share/evo-hmi/maps \
    california-latest.osm.pbf
cp <libosmscout>/stylesheets/standard.oss ~/.local/share/evo-hmi/maps/
```

```bash
./evo-hmi --map-dir ~/.local/share/evo-hmi/maps
# or: export EVO_HMI_MAP_DIR=$HOME/.local/share/evo-hmi/maps
```

On the box the same files live under `/media/sd/evo-hmi/osmscout/`.

## Valhalla (routing)

Tiles are built on a large server, never on the i.MX6. Cross-compile libvalhalla with services and data tools off (`-DENABLE_SERVICES=OFF -DENABLE_DATA_TOOLS=OFF`). `graph_version` on each pack must match that library.

Seed: copy one metro pack onto a blank card so first boot routes with no network. After that the card is the cache.

Device config (`valhalla.json`): `mjolnir.tile_dir` set, `tile_extract` unset, LRU on, auto costing only, regional `service_limits`.

## Zone packs (agent + evo-nav-server)

HTTP contract, pack format, and mjolnir pipeline: [evo-nav-server README](../../evo-nav-server/README.md).

While the hotspot is up, the agent `POST`s missing zone ids. Cache hit → pack is `ready` immediately. Miss → server downloads OSM, runs `valhalla_build_tiles` into a `tile_dir`, packs `zone-<id>.tar.zst`. The agent verifies sha256, merges into `tiles/{0,1,2}/`, updates `catalog.json`. Offline, the HMI keeps using the card.

## gpsd

```text
gpsd -N /dev/ttyACM0
# HMI default: 127.0.0.1:2947 JSON WATCH
```
