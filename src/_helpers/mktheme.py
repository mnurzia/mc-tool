f = open("theme.txt", "r")
l = [l for l in f.readlines() if len(l) > 5]
for line in l:
	desc, col = line.split(": ")
	col = col.rstrip("\n")
	r, g, b = col[1:-1].split(", ")
	print("colors[ImGuiCol_%s] = ImVec4(%1.2f, %1.2f, %1.2f, 1);" % (desc, float(r)/255, float(g)/255, float(b)/255))
