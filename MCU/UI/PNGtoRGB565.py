from PIL import Image
import os
import sys

if __name__ == '__main__':
    fname = sys.argv[1]
    img = Image.open(fname)
    fout = open(fname.split('.')[0] + '.565', 'wb') 
    fout.write(bytes([img.size[0] & 0xff, img.size[0] >> 8])) # width
    fout.write(bytes([img.size[1] & 0xff, img.size[1] >> 8])) # height
    for y in range(img.size[1]):
        for x in range(img.size[0]):
            rgb = img.getpixel((x, y))
            r5 = rgb[0] >> 3
            g6 = rgb[1] >> 2
            b5 = rgb[2] >> 3
            rgb565 = b5 + (g6 << 5) + (r5 << 11)
            fout.write(bytes([rgb565 & 0xff, rgb565 >> 8]))
    img.close()
    fout.close()



