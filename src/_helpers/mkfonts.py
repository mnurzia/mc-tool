import json, sys

contents = json.load(open("icons.json"))

if sys.argv[1] == "header":
    for glyph, data in contents.items():
        print("extern const char* fa_%s;" % glyph.replace("-","_"))    
elif sys.argv[1] == "implementation":
    maxw = max([len(glyph) for glyph in contents])
    for glyph, data in contents.items():
        print("const char* fa_%s = u8\"\\u%04X\";" % (glyph.replace("-","_").ljust(maxw), int(data["unicode"], 16)))
else:
    print("Bad argument")
    sys.exit(1)