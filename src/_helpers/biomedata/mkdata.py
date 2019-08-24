import javalang
import glob
import sys
import re
import json
import math

from PIL import Image

REG_BIOMES = r'public static final Biome (\S*) = register\((\d*), "([^"]*)", new ([^(]*)'
REG_FEATURE = r"addStructureFeature\(Feature.([^,]*)"
REG_DEFAULT_FEATURE = r"DefaultBiomeFeatures\.add([^(]*)\(this\);"
REG_SETTING = r"super\(\(new Biome.Settings\(\)\)\w*([^;]*);"
REG_SETTING_CHILD = r"/\* \d\d \*/\s*.([^(]*)"
REG_SPAWNS = r"addSpawn\(EntityCategory.([^,]*), new Biome.SpawnEntry\(EntityType.([^,]*), (\d*), (\d*), (\d*)"
REG_FOLIAGE = r"public int getFoliageColorAt\([^\)]*\) { return (\d*); }"
REG_GRASS = r"public int getGrassColorAt\([^\)]*\) { return (\d*); }"
REG_MIGRATE = r"\(ID::([^,]*),.*TempCat::([^,]*)"

OLD = """initBiomeAdd(ID::OCEAN,                   TypeCat::OCEAN,           TempCat::OCEANIC,  0.50f, -1.00f);
        initBiomeAdd(ID::PLAINS,                  TypeCat::PLAINS,          TempCat::LUSH,     0.80f,  0.10f);
        initBiomeAdd(ID::DESERT,                  TypeCat::DESERT,          TempCat::WARM,     2.00f,  0.12f);
        initBiomeAdd(ID::MOUNTAINS,               TypeCat::HILLS,           TempCat::LUSH,     0.20f,  1.00f);
        initBiomeAdd(ID::FOREST,                  TypeCat::FOREST,          TempCat::LUSH,     0.70f,  0.10f);
        initBiomeAdd(ID::TAIGA,                   TypeCat::TAIGA,           TempCat::LUSH,     0.25f,  0.20f);
        initBiomeAdd(ID::SWAMP,                   TypeCat::SWAMP,           TempCat::LUSH,     0.80f, -0.20f);
        initBiomeAdd(ID::RIVER,                   TypeCat::RIVER,           TempCat::LUSH,     0.50f, -0.50f);
        initBiomeAdd(ID::NETHER,                  TypeCat::NETHER,          TempCat::WARM,     2.00f,  0.10f);
        initBiomeAdd(ID::THE_END,                 TypeCat::THE_END,         TempCat::LUSH,     0.50f,  0.10f);
        initBiomeAdd(ID::FROZEN_OCEAN,            TypeCat::OCEAN,           TempCat::OCEANIC,  0.00f, -1.00f);
        initBiomeAdd(ID::FROZEN_RIVER,            TypeCat::RIVER,           TempCat::COLD,     0.00f, -0.50f);
        initBiomeAdd(ID::SNOWY_TUNDRA,            TypeCat::SNOW,            TempCat::COLD,     0.00f,  0.12f);
        initBiomeAdd(ID::SNOWY_MOUNTAINS,         TypeCat::SNOW,            TempCat::COLD,     0.00f,  0.45f);
        initBiomeAdd(ID::MUSHROOM_FIELDS,         TypeCat::MUSHROOM_ISLAND, TempCat::LUSH,     0.90f,  0.20f);
        initBiomeAdd(ID::MUSHROOM_FIELD_SHORE,    TypeCat::MUSHROOM_ISLAND, TempCat::LUSH,     0.90f,  0.00f);
        initBiomeAdd(ID::BEACH,                   TypeCat::BEACH,           TempCat::LUSH,     0.80f,  0.00f);
        initBiomeAdd(ID::DESERT_HILLS,            TypeCat::DESERT,          TempCat::WARM,     2.00f,  0.45f);
        initBiomeAdd(ID::WOODED_HILLS,            TypeCat::FOREST,          TempCat::LUSH,     0.70f,  0.45f);
        initBiomeAdd(ID::TAIGA_HILLS,             TypeCat::TAIGA,           TempCat::LUSH,     0.25f,  0.45f);
        initBiomeAdd(ID::MOUNTAIN_EDGE,           TypeCat::HILLS,           TempCat::LUSH,     0.20f,  1.00f);
        initBiomeAdd(ID::JUNGLE,                  TypeCat::JUNGLE,          TempCat::LUSH,     0.95f,  0.10f);
        initBiomeAdd(ID::JUNGLE_HILLS,            TypeCat::JUNGLE,          TempCat::LUSH,     0.95f,  0.45f);
        initBiomeAdd(ID::JUNGLE_EDGE,             TypeCat::JUNGLE,          TempCat::LUSH,     0.95f,  0.10f);
        initBiomeAdd(ID::DEEP_OCEAN,              TypeCat::OCEAN,           TempCat::OCEANIC,  0.50f, -1.80f);
        initBiomeAdd(ID::STONE_SHORE,             TypeCat::STONE_BEACH,     TempCat::LUSH,     0.20f,  0.10f);
        initBiomeAdd(ID::SNOWY_BEACH,             TypeCat::BEACH,           TempCat::COLD,     0.05f,  0.00f);
        initBiomeAdd(ID::BIRCH_FOREST,            TypeCat::FOREST,          TempCat::LUSH,     0.60f,  0.10f);
        initBiomeAdd(ID::BIRCH_FOREST_HILLS,      TypeCat::FOREST,          TempCat::LUSH,     0.60f,  0.45f);
        initBiomeAdd(ID::DARK_FOREST,             TypeCat::FOREST,          TempCat::LUSH,     0.70f,  0.10f);
        initBiomeAdd(ID::SNOWY_TAIGA,             TypeCat::TAIGA,           TempCat::COLD,    -0.50f,  0.20f);
        initBiomeAdd(ID::SNOWY_TAIGA_HILLS,       TypeCat::TAIGA,           TempCat::COLD,    -0.50f,  0.45f);
        initBiomeAdd(ID::GIANT_TREE_TAIGA,        TypeCat::TAIGA,           TempCat::LUSH,     0.30f,  0.20f);
        initBiomeAdd(ID::GIANT_TREE_TAIGA_HILLS,  TypeCat::TAIGA,           TempCat::LUSH,     0.30f,  0.45f);
        initBiomeAdd(ID::WOODED_MOUNTAINS,        TypeCat::HILLS,           TempCat::LUSH,     0.20f,  1.00f);
        initBiomeAdd(ID::SAVANNA,                 TypeCat::SAVANNA,         TempCat::WARM,     1.20f,  0.12f);
        initBiomeAdd(ID::SAVANNA_PLATEAU,         TypeCat::SAVANNA,         TempCat::WARM,     1.00f,  1.50f);
        initBiomeAdd(ID::BADLANDS,                TypeCat::BADLANDS,        TempCat::WARM,     2.00f,  0.10f);
        initBiomeAdd(ID::WOODED_BADLANDS_PLATEAU, TypeCat::BADLANDS,        TempCat::WARM,     2.00f,  1.50f);
        initBiomeAdd(ID::BADLANDS_PLATEAU,        TypeCat::BADLANDS,        TempCat::WARM,     2.00f,  1.50f);
        initBiomeAdd(ID::SMALL_END_ISLANDS,       TypeCat::THE_END,         TempCat::LUSH,     0.50f,  0.10f);
        initBiomeAdd(ID::END_MIDLANDS,            TypeCat::THE_END,         TempCat::LUSH,     0.50f,  0.10f);
        initBiomeAdd(ID::END_HIGHLANDS,           TypeCat::THE_END,         TempCat::LUSH,     0.50f,  0.10f);
        initBiomeAdd(ID::END_BARRENS,             TypeCat::THE_END,         TempCat::LUSH,     0.50f,  0.10f);
        initBiomeAdd(ID::WARM_OCEAN,              TypeCat::OCEAN,           TempCat::OCEANIC,  0.50f, -1.00f);
        initBiomeAdd(ID::LUKEWARM_OCEAN,          TypeCat::OCEAN,           TempCat::OCEANIC,  0.50f, -1.00f);
        initBiomeAdd(ID::COLD_OCEAN,              TypeCat::OCEAN,           TempCat::OCEANIC,  0.50f, -1.00f);
        initBiomeAdd(ID::DEEP_WARM_OCEAN,         TypeCat::OCEAN,           TempCat::OCEANIC,  0.50f, -1.80f);
        initBiomeAdd(ID::DEEP_LUKEWARM_OCEAN,     TypeCat::OCEAN,           TempCat::OCEANIC,  0.50f, -1.80f);
        initBiomeAdd(ID::DEEP_COLD_OCEAN,         TypeCat::OCEAN,           TempCat::OCEANIC,  0.50f, -1.80f);
        initBiomeAdd(ID::DEEP_FROZEN_OCEAN,       TypeCat::OCEAN,           TempCat::OCEANIC,  0.50f, -1.80f);
        initBiomeAdd(ID::BAMBOO_JUNGLE,           TypeCat::JUNGLE,          TempCat::LUSH,     0.95f,  0.10f);
        initBiomeAdd(ID::BAMBOO_JUNGLE_HILLS,     TypeCat::JUNGLE,          TempCat::LUSH,     0.95f,  0.45f);
        initBiomeAdd(ID::THE_VOID,                TypeCat::THE_VOID,        TempCat::THE_VOID, 0.50f,  0.00f);"""

AMIDST_COLORS = [
    [  0,   0, 112], [141, 179,  96], [250, 148,  24], [ 96,  96,  96], [  5, 102,  33], [ 11, 102,  89], [  7, 249, 178], [  0,   0, 255], 
    [255,   0,   0], [128, 128, 255], [112, 112, 214], [160, 160, 255], [255, 255, 255], [160, 160, 160], [255,   0, 255], [160,   0, 255], 
    [250, 222,  85], [210,  95,  18], [ 34,  85,  28], [ 22,  57,  51], [114, 120, 154], [ 83, 123,   9], [ 44,  66,   5], [ 98, 139,  23], 
    [  0,   0,  48], [162, 162, 132], [250, 240, 192], [ 48, 116,  68], [ 31,  95,  50], [ 64,  81,  26], [ 49,  85,  74], [ 36,  63,  54], 
    [ 89, 102,  81], [ 69,  79,  62], [ 80, 112,  80], [189, 178,  95], [167, 157, 100], [217,  69,  21], [176, 151, 101], [202, 140, 101], 
    [128, 128, 255], [128, 128, 255], [128, 128, 255], [128, 128, 255], [  0,   0, 172], [  0,   0, 144], [ 32,  32, 112], [  0,   0,  80], 
    [  0,   0,  64], [ 32,  32,  56], [ 64,  64, 144], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], 
    [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], 
    [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], 
    [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], 
    [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], 
    [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], 
    [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], 
    [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], 
    [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], 
    [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [  0,   0,   0], 
    [255,   0, 255], [141, 179,  96], [250, 148,  24], [ 96,  96,  96], [  5, 102,  33], [ 11, 102,  89], [  7, 249, 178], [255,   0, 255], 
    [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255, 255, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], 
    [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [ 83, 123,   9], [255,   0, 255], [ 98, 139,  23], 
    [255,   0, 255], [255,   0, 255], [255,   0, 255], [ 48, 116,  68], [ 31,  95,  50], [ 64,  81,  26], [ 49,  85,  74], [255,   0, 255], 
    [ 89, 102,  81], [ 69,  79,  62], [ 80, 112,  80], [189, 178,  95], [167, 157, 100], [217,  69,  21], [176, 151, 101], [202, 140, 101], 
    [118, 142,  20], [ 59,  71,  10], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], 
    [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], 
    [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], 
    [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], 
    [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], 
    [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], 
    [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], 
    [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], 
    [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], 
    [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], 
    [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255], [255,   0, 255]
]

biomes = {}
temp_cat = {}
data = {}
hs = {}
foliage_img = Image.open("foliage.png")
foliage_pix = foliage_img.load()
grass_img = Image.open("grass.png")
grass_pix = grass_img.load()

old_match = re.findall(REG_MIGRATE, OLD)
for match in old_match:
    temp_cat[match[0]] = match[1]

f = open("data/Biomes.java")
contents = f.read()
f.close()
biome_m = re.findall(REG_BIOMES, contents)
for match in biome_m:
    biomes[match[2]] = match

names = []
for biome in biomes.items():
    src = "data/"+biome[1][3]+".java"
    f = open(src,"r")
    content = f.read()
    f.close()
    node = {}
    node["name"] = src.split("/")[1].split(".")[0]
    features = re.findall(REG_FEATURE, content)
    node["features"] = features
    settings = re.findall(REG_SETTING, content, re.DOTALL)
    settings = settings[0].splitlines()
    out = []
    for setting in settings:
        match = re.findall(REG_SETTING_CHILD, setting)
        if match:
            out.append(setting[17:])
    settings = out
    for setting in settings:
        if setting.startswith(".configureSurfaceBuilder"):
            temp = setting[len(".configureSurfaceBuilder"):][1:][:-1]
            temp = temp.split(", ")
            node["surface_types"] = [x[len("SurfaceBuilder."):] for x in temp]
        elif setting.startswith(".precipitation"):
            temp = setting[len(".precipitation"):][1:][:-1]
            temp = temp[len("Biome.Precipitation."):]
            node["precipitation_type"] = temp
        elif setting.startswith(".category"):
            temp = setting[len(".category"):][1:][:-1]
            temp = temp[len("Biome.Category."):]
            node["category"] = temp
        elif setting.startswith(".depth"):
            temp = setting[len(".depth"):][1:][:-1]
            temp = temp[:-1]
            node["depth"] = float(temp)
        elif setting.startswith(".scale"):
            temp = setting[len(".scale"):][1:][:-1]
            temp = temp[:-1]
            node["scale"] = float(temp)  
        elif setting.startswith(".temperature"):
            temp = setting[len(".temperature"):][1:][:-1]
            temp = temp[:-1]
            node["temperature"] = float(temp) 
        elif setting.startswith(".downfall"):
            temp = setting[len(".downfall"):][1:][:-1]
            temp = temp[:-1]
            node["downfall"] = float(temp) 
        elif setting.startswith(".waterColor"):
            temp = setting[len(".waterColor"):][1:][:-1]
            temp = int(temp)
            node["water_color"] = (temp >> 16, (temp >> 8) & 255, temp & 255)
        elif setting.startswith(".waterFogColor"):
            temp = setting[len(".waterFogColor"):][1:][:-1]
            temp = int(temp)
            node["water_fog_color"] = (temp >> 16, (temp >> 8) & 255, temp & 255)
        elif setting.startswith(".parent"):
            temp = setting[len(".parent"):][1:][:-2]
            if temp == "null":
                node["parent"] = None
            else:
                node["parent"] = biomes[temp[1:][:-1]][0] 
    defaults = re.findall(REG_DEFAULT_FEATURE, content)
    node["default_features"] = [x.upper() for x in defaults]
    node["spawns"] = []
    spawns = re.findall(REG_SPAWNS, content)
    for spawn in spawns:
        node["spawns"].append({
            "type" : spawn[1],
            "class" : spawn[0],
            "weight" : int(spawn[2]),
            "min_amt" : int(spawn[3]),
            "max_amt" : int(spawn[4])
        })
    grass = re.findall(REG_GRASS, content)
    if len(grass):
        grass = int(grass[0])
        node["grass_color"] = (grass >> 16, (grass >> 8) & 255, grass & 255)
    else:
        node["grass_color"] = (255, 0, 255)
    foliage = re.findall(REG_FOLIAGE, content)
    if len(foliage):
        foliage = int(foliage[0])
        node["foliage_color"] = (foliage >> 16, (foliage >> 8) & 255, foliage & 255)
    else:
        node["foliage_color"] = (255, 0, 255)
    node["id"] = int(biome[1][1])
    node["minecraft_id"] = biome[1][2]
    if biome[1][0] not in temp_cat:
        node["temp_cat"] = temp_cat[node["parent"]]
    else:
        node["temp_cat"] = temp_cat[biome[1][0]]
    data[biome[1][0]] = node

code = []
code.append("Biome* current;")
# sorry for using semicolons; I brought that over from c++ by accident
for id, node in data.items():
    #code = []
    #print(json.dumps(node, indent=True))
    code.append("current = &(biomes[ID::%s]);" % str(id).upper());
    code.append("current->id = ID::%s;" % str(id).upper());
    code.append("current->type_cat = TypeCat::%s;" % ("OCEAN" if node["category"] == "NONE" else node["category"]));
    code.append("current->temp_cat = TempCat::%s;" % node["temp_cat"]);
    code.append("current->parent = ID::%s;" % str(node["parent"]).upper());
    code.append("current->elevation = %f;" % node["depth"]);
    code.append("current->temperature = %f;" % node["temperature"]);
    code.append("current->scale = %f;" % node["scale"]);
    code.append("current->downfall = %f;" % node["downfall"]);
    code.append("current->precipitation = PrecipType::%s;" % node["precipitation_type"]);
    for feat in node["features"]:
        code.append("current->feature_types.push_back(Feature::Type::%s);" % feat);
        code.append("current->feature_bits |= (1 << int32_t(Feature::Type::%s));" % feat);
    code.append("current->water_color[0] = %i;" % node["water_color"][0]);
    code.append("current->water_color[1] = %i;" % node["water_color"][1]);
    code.append("current->water_color[2] = %i;" % node["water_color"][2]);
    code.append("current->water_fog_color[0] = %i;" % node["water_fog_color"][0]);
    code.append("current->water_fog_color[1] = %i;" % node["water_fog_color"][1]);
    code.append("current->water_fog_color[2] = %i;" % node["water_fog_color"][2]);
    temperature = max(min(node["temperature"], 1.0), 0.0)
    rainfall = max(min(node["downfall"], 1.0), 0.0) * temperature
    foliage_x = int((1 - temperature) * 255);
    foliage_y = int((1 - rainfall) * 255);
    node["foliage_color"] = foliage_pix[foliage_x, foliage_y];
    code.append("current->foliage_color[0] = %i;" % node["foliage_color"][0]);
    code.append("current->foliage_color[1] = %i;" % node["foliage_color"][1]);
    code.append("current->foliage_color[2] = %i;" % node["foliage_color"][2]);
    node["grass_color"] = grass_pix[foliage_x, foliage_y];
    code.append("current->grass_color[0] = %i;" % node["grass_color"][0]);
    code.append("current->grass_color[1] = %i;" % node["grass_color"][1]);
    code.append("current->grass_color[2] = %i;" % node["grass_color"][2]);
    node["amidst_color"] = AMIDST_COLORS[node["id"]];
    code.append("current->amidst_color[0] = %i;" % node["amidst_color"][0]);
    code.append("current->amidst_color[1] = %i;" % node["amidst_color"][1]);
    code.append("current->amidst_color[2] = %i;" % node["amidst_color"][2]);
    for feat in node["default_features"]:
        code.append("current->all_feature_types.push_back(\"%s\");" % feat)
    for spawn in node["spawns"]:
        code.append("current->spawns.push_back({\"%s\", \"%s\", %i, %i, %i});" % tuple(list(spawn.values())))
    code.append("current->minecraft_id = \"%s\";" % node["minecraft_id"])
#sys.exit()
for line in code:
    print("\t"+line)