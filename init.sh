echo "--> Running init"

echo "  --> Framebuffer font download + extraction..."
    wget https://int10h.org/oldschool-pc-fonts/download/oldschool_pc_font_pack_v2.2_FULL.zip
    unzip oldschool_pc_font_pack_v2.2_FULL.zip -d ./drivers/video/font/wget

echo "  --> Framebuffer font conversion..."
    link "./drivers/video/font/wget/ttf - Px (pixel outline)/Px437_IBM_VGA_8x16.ttf" ./drivers/video/font/font.ttf
    python ./mkfont.py