from PIL import Image, ImageFont, ImageDraw
import numpy as np

def gendesc(fn):
    desc = f"""
/*
    FILE:           {fn}
    ---------------------------------------------
    PARENT PROJECT: RpiOS
    CREDITS:        Vaclav Hajsman
                    (c) 2016-2020 VileR
                        < http://int10h.org/oldschool-pc-fonts/ >
                        < http://creativecommons.org/licenses/by-sa/4.0/ >
    VERSION:        1.0
    REVISION:       0
    
    PURPOSE:        Font
    
    ---------------------------------------------
    NOTES:          This file is script generated.
*/

/*
    The font was downloaded from https://int10h.org/oldschool-pc-fonts/download/
    and was just converted using TTF font to C bitmap array converter software.

        The Ultimate Oldschool PC Font Pack v2.2:
        oldschool_pc_­font_pack_v2.2­_FULL.zip
*/


    """

def genbitmap(ttf_path, w, h, out):
    if w > 8:
        """
            Because "uint8_t" is used, w can not
            be bigger than 8.
        """

        print("Character width too high.")
        return 1

    bitmaps={}

    font = ImageFont.truetype(ttf_path, size=max(w, h))

    # Define characters to generate bitmaps for (ASCII printable characters)
    chars = [chr(i) for i in range(32, 127)]

    for ci in chars:
        """
            This algorythm renders the characters onto the image and
            converts the image to the NumPy array. The bitmap is then
            stored in dictionary.
        """

        img = Image.new('L', (w, h), color=255)
        draw = ImageDraw.Draw(img)

        # Size and position calculations
        tw, th = draw.textsize(ci, font=font)
        tx = (w - tw) / 2
        ty = (h - th) / 2

        draw.text((tx, th), ci, font=font, fill=0)

        bitmaps[ci] = np.array(img)

    with open(out, 'w') as f:
        fontname="vga8x16"

        f.write(gendesc(out))

        f.write("#ifndef __FONT_H\n#define __FONT_H\n\n")   # Header include guard
        f.write("#include <stdint.h>\n\n")

        f.write("// --- CONSTANTS ---\n")
        f.write(f"    #define FONT_CH_WIDTH     {w}\n")
        f.write(f"    #define FONT_CH_HEIGHT    {h}\n\n")

        f.write("// --- TYPE DEFINITIONS ---\n")
        f.write(f"    typedef const uint8_t font_bitmap_t[{h}];\n")
        f.write(f"    typedef font_bitmap_t font_t[{len(chars)}];\n\n")

        f.write("const uint8_t font_bitmap[][{}][{} / 8] = {{\n".format(h, w))

        for char in characters:
            f.write("    // ASCII: {}\n".format(ord(char)))
            f.write("    {\n")

            for row in bitmaps[char]:
                f.write("        {")

                for col in range(0, character_width, 8):
                    byte = 0

                    for i in range(8):
                        if col + i < character_width and row[col + i] < 128:
                            byte |= 1 << (7 - i)

                    f.write("0x{:02x},".format(byte))

                f.write("},\n")

            f.write("    },\n")

        f.write("};\n")

        f.write(f"#define FONT {fontname}")

if __name__ == "__main__":
    genbitmap("./drivers/video/font/font.ttf", 8, 16, "./drivers/video/font/font.h")