import math
from scipy.optimize import fsolve
from PIL import Image, ImageFont, ImageDraw

width_scale = 460
h_padding = 15
length_line = 8
thickness_line = 3
v_spacing = 4
v_padding = 4
font_size = 14
color_background = (0x48, 0x14, 0x88)
color_scale = (0x80, 0x9c, 0xc8)
color_warning = 'yellow' 
def calc_logscale(x1,y1,x2,y2):
    k = math.log2(y2/y1) / (x2 - x1)
    y0 = y1 / (2**(x1 * k))
    ymid = y1 * (2**((0.5-x1)*k))
    ymax = y2 * 2**((1-x2) * k)
    return lambda x: y1 * 2**((x-x1)*k)

## Power
f = calc_logscale(0.5, 350, 0.8, 1000)
ticks_main = [70, 100, 150, 200, 300, 400,600, 1000, 1500, 2000]
ticks_sub = [80, 90, 110,120,130,140,160,170,180,190, 300, 500 , 700, 800, 900,1100, 1200, 1300, 1400]
threshold_tick = 1000

## SWR
f = calc_logscale(0, 1.0, 0.5, 2.0)
ticks_main = [1.0, 1.2, 1.5, 2.0, 3.0, 4.0]
ticks_sub = [1.1,1.2,1.3,1.4, 1.6,1.7,1.8,1.9, 2.1,2.2,2.3,2.4,2.5,2.6,2.7,2.8,2.9,4.0]
threshold_tick = 3.0

font = ImageFont.truetype('font/SourceHanSansCN-Bold.ttf', font_size)
font_big = ImageFont.truetype('font/SourceHanSansCN-Bold.ttf', font_size + 2)
img = Image.new(size = [width_scale+h_padding*2, length_line + v_spacing + font_size + v_padding],
 mode='RGB', color = color_background)
draw = ImageDraw.Draw(img)
# draw hline
x_threshold = fsolve(lambda t: f(t) - threshold_tick, 1)[0] * width_scale
draw.line((h_padding, 0, h_padding + x_threshold, 0), fill=color_scale, width=2)
draw.line((h_padding + x_threshold, 0, h_padding + width_scale, 0), fill=color_warning, width=2)
# draw main ticks
for  tick in ticks_main:
    x = round(fsolve(lambda t: f(t) - tick, 1)[0] * width_scale) + h_padding
    if tick <= threshold_tick:
        draw.line((x, 0, x, length_line), fill=color_scale, width=thickness_line)
        (w, h), (ox, oy) = font.font.getsize(str(tick))
        draw.text((x - w/2 - ox, length_line + v_spacing - oy), str(tick), fill=color_scale, font = font)
    else:
        draw.line((x, 0, x, length_line), fill=color_warning, width=thickness_line)
        (w, h), (ox, oy) = font_big.font.getsize(str(tick))
        draw.text((x - w/2 - ox, length_line + v_spacing - oy), str(tick), fill=color_warning, font = font)


for  tick in ticks_sub:
    x = round(fsolve(lambda t: f(t) - tick, 1)[0] * width_scale) + h_padding
    if(tick < threshold_tick):
        draw.line((x, 0, x, length_line * 0.7), fill=color_scale, width=round(thickness_line * 0.7))
    else:
        draw.line((x, 0, x, length_line * 0.7), fill=color_warning, width=round(thickness_line * 0.7))

    # w, h, ox, oy = font.font.getsize(str(tick))
    # draw.text((x - w/2 - ox, length_line + v_spacing - oy), str(tick), fill=color_scale, font = font)

img.show()
img.save('scale_swr.png')