import glob
import math
import sys
import os
from PIL import Image

ICON_SIZE = int(sys.argv[1])
ICON_SET = sys.argv[2]

all_img = []
iset = ICON_SET
sz_folds = glob.glob("%s/*" % iset)
if iset+"/all" in sz_folds:
    all_img.extend(glob.glob(iset+"/all/*.png"))
else:
    all_img.extend(glob.glob(iset+"/%i/*.png" % ICON_SIZE))

atl_width = (32 if len(all_img) > 32 else len(all_img))
atl_height = int(math.ceil(len(all_img) / atl_width))

out_img = Image.new("RGBA", (atl_width * ICON_SIZE, atl_height * ICON_SIZE))
img_x = 0
img_y = 0
for ico_img_name in all_img:
    ico_img = Image.open(ico_img_name)
    ico_img = ico_img.convert("RGBA")
    #print(ico_img_name, ico_img.size)
    ico_img = ico_img.resize((ICON_SIZE, ICON_SIZE))
    int_img = ico_img.copy()
    out_img.paste(int_img, (img_x * ICON_SIZE, img_y * ICON_SIZE, (img_x + 1) * ICON_SIZE, (img_y + 1) * ICON_SIZE))
    ico_img.close()
    img_x += 1
    if img_x == atl_width:
        img_x = 0
        img_y += 1
    print(ico_img_name, ico_img.size)
out_img.save("atlas_%i.png" % ICON_SIZE)

out_head = open("Icons.hpp","w")
out_head.write("#ifndef APP_ICONS_H\n")
out_head.write("#define APP_ICONS_H\n")
out_head.write("\n")
out_head.write("#define APP_ICONS_ATLAS_WIDTH %i\n" % atl_width)
out_head.write("#define APP_ICONS_ATLAS_HEIGHT %i\n" % atl_height)
out_head.write("\n")
out_head.write("#define APP_ICONS_MIN %i\n" % 0xF000)
out_head.write("#define APP_ICONS_MAX %i\n" % (0xF000 + len(all_img)))
out_head.write("\n")
for i, ico_img_name in enumerate(all_img):
    segs = os.path.split(ico_img_name)
    head = segs[0].split("/")[0]
    tail = segs[1]
    out_head.write("#define ICON_%s_%s u8\"\\u%04x\"\n" % (head.upper(), tail.upper().replace("-","_").split(".")[0], i + 0xF000))
    out_head.write("#define ICON_VAL_%s_%s 0x%04X\n" % (head.upper(), tail.upper().replace("-","_").split(".")[0], i + 0xF000))
out_head.write("\n")
out_head.write("#endif")
out_head.close()