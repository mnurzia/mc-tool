# mc-tool
![screenshot of seed finder](https://i.imgur.com/WF8EF7H.png)

Experimental seed viewer / NBT editor / Data Pack IDE for Minecraft JE 1.14

This is about 3-5x faster than AMIDST (the leading competitive seed-finder) and shows twice the resolution.

I used the reverse engineered and optimized algorithms from [cubiomes](https://github.com/Cubitect/cubiomes/) to generate most of the terrain. I manually reverse-engineered certain terrain passes.

I had to reverse some of the structure finding methods myself as well, particularly the newer ones.

In addition, there is a from-scratch NBT viewer written.
