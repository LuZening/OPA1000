from PIL import Image
import os
import sys

if __name__ == '__main__':
    fname = sys.argv[1]
    f = open(fname, 'rb')
    w = f.read(1)[0] + (f.read(1)[0] << 8) # width
    h = f.read(1)[0] + (f.read(1)[0] << 8) # height
    print(w, h)
    img = Image.new(size=(w,h), mode = 'RGB')

    for y in range(img.size[1]):
        for x in range(img.size[0]):
            rgb565 = f.read(1)[0] + (f.read(1)[0] << 8)
            b = (rgb565 & 0b11111) << 3
            rgb565 >>= 5
            g = (rgb565 & 0b111111) << 2
            rgb565 >>= 6
            r = rgb565 << 3  
            rgb = img.putpixel((x, y), (r, g, b))
    img.show()
    img.close()
    fout.close()



