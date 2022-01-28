from PIL import Image, ImageDraw, ImageFont

font_path = 'font/SourceHanSansCN-Normal.ttf'
font_size = 16
charset_path = 'ascii16.cst'
img_all = Image.new(size=(2400, 28), mode= 'L')
d_all = ImageDraw.Draw(img_all)
x_all = 0

if __name__ == '__main__':
    font = ImageFont.truetype(font_path, font_size) 
    f = open(charset_path, 'wb')
    c_list = [chr(i) for i in range(32, 127)]
    f.write(bytes([len(c_list)]))
    for c in c_list:
        (w, h), (off_x, off_y) = font.font.getsize(c)
        x_begin = -off_x
        y_begin = -off_y
        d_all.text((x_all, y_begin), c, font = font, fill = 'white')
        x_all += w + off_x + 1
        img = Image.new(size=(w+1, h+1), mode = 'L')
        d = ImageDraw.Draw(img)
        d.text((x_begin, y_begin), c, font=font, fill='white')
        # convert to .cst file
        f.write(bytes([img.width, img.height,    # Byte1: width Byte2: height
                    (256+off_x)&0xff, (256+off_y)&0xff])) # Byte3: signed off_x Byte2: signed off_y
        bitmap = img.tobytes()
        assert len(bitmap) == img.width * img.height, 'Bitmap size mismatch: ' + str(ord(c))
        f.write(bitmap)
        # img.show()
    f.close()
    img_all.show()