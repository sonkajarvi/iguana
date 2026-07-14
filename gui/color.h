/**
 * Copyright (c) 2026, sonkajarvi
 *
 * Licensed under the BSD 2-Clause License.
 * The full license can be found in the LICENSE.txt file.
 */

#ifndef __GUI_COLOR
#define __GUI_COLOR

#include <stdint.h>

/*
 * Colors are unsigned 32-bit integers, and are in RGBA format where the most
 * significant byte is the red channel.
 */
#define COLOR_R(rgba) (((rgba) >> 24) & 255)
#define COLOR_G(rgba) (((rgba) >> 16) & 255)
#define COLOR_B(rgba) (((rgba) >>  8) & 255)
#define COLOR_A(rgba) (((rgba)      ) & 255)

#define COLOR_INT_TO_FLOAT(channel) ((float)(channel) / 255.0f)

/**
 * color_to_3f - Transform RGB color to three floats.
 * @rgb: The color to transform.
 * @out: Buffer to write to.
 */
static inline void color_to_3f(uint32_t rgb, float out[static 3])
{
    out[0] = (float)COLOR_R(rgb) / 255.0f;
    out[1] = (float)COLOR_G(rgb) / 255.0f;
    out[2] = (float)COLOR_B(rgb) / 255.0f;
}

/**
 * color_to_4f - Transform RGBA color to four floats.
 * @rgba: The color to transform.
 * @out: Buffer to write to.
 */
static inline void color_to_4f(uint32_t rgba, float out[static 4])
{
    color_to_3f(rgba, out);
    out[3] = (float)COLOR_A(rgba) / 255.0f;
}

/* TODO: make this debug only */
const char *color_to_string(uint32_t rgba);

/*
 * These colors are the same as the CSS named colors.
 * See: https://developer.mozilla.org/en-US/docs/Web/CSS/Reference/Values/named-color
 */
#define COLOR_ALICEBLUE                 0xf0f8ffff
#define COLOR_ANTIQUEWHITE              0xfaebd7ff
#define COLOR_AQUA                      0x00ffffff
#define COLOR_AQUAMARINE                0x7fffd4ff
#define COLOR_AZURE                     0xf0ffffff
#define COLOR_BEIGE                     0xf5f5dcff
#define COLOR_BISQUE                    0xffe4c4ff
#define COLOR_BLACK                     0x000000ff
#define COLOR_BLANCHEDALMOND            0xffebcdff
#define COLOR_BLUE                      0x0000ffff
#define COLOR_BLUEVIOLET                0x8a2be2ff
#define COLOR_BROWN                     0xa52a2aff
#define COLOR_BURLYWOOD                 0xdeb887ff
#define COLOR_CADETBLUE                 0x5f9ea0ff
#define COLOR_CHARTREUSE                0x7fff00ff
#define COLOR_CHOCOLATE                 0xd2691eff
#define COLOR_CORAL                     0xff7f50ff
#define COLOR_CORNFLOWERBLUE            0x6495edff
#define COLOR_CORNSILK                  0xfff8dcff
#define COLOR_CRIMSON                   0xdc143cff
#define COLOR_CYAN                      0x00ffffff /* same as COLOR_AQUA */
#define COLOR_DARKBLUE                  0x00008bff
#define COLOR_DARKCYAN                  0x008b8bff
#define COLOR_DARKGOLDENROD             0xb8860bff
#define COLOR_DARKGRAY                  0xa9a9a9ff
#define COLOR_DARKGREEN                 0x006400ff
#define COLOR_DARKGREY                  0xa9a9a9ff
#define COLOR_DARKKHAKI                 0xbdb76bff
#define COLOR_DARKMAGENTA               0x8b008bff
#define COLOR_DARKOLIVEGREEN            0x556b2fff
#define COLOR_DARKORANGE                0xff8c00ff
#define COLOR_DARKORCHID                0x9932ccff
#define COLOR_DARKRED                   0x8b0000ff
#define COLOR_DARKSALMON                0xe9967aff
#define COLOR_DARKSEAGREEN              0x8fbc8fff
#define COLOR_DARKSLATEBLUE             0x483d8bff
#define COLOR_DARKSLATEGRAY             0x2f4f4fff
#define COLOR_DARKSLATEGREY             0x2f4f4fff
#define COLOR_DARKTURQUOISE             0x00ced1ff
#define COLOR_DARKVIOLET                0x9400d3ff
#define COLOR_DEEPPINK                  0xff1493ff
#define COLOR_DEEPSKYBLUE               0x00bfffff
#define COLOR_DIMGRAY                   0x696969ff
#define COLOR_DIMGREY                   0x696969ff
#define COLOR_DODGERBLUE                0x1e90ffff
#define COLOR_FIREBRICK                 0xb22222ff
#define COLOR_FLORALWHITE               0xfffaf0ff
#define COLOR_FORESTGREEN               0x228b22ff
#define COLOR_FUCHSIA                   0xff00ffff
#define COLOR_GAINSBORO                 0xdcdcdcff
#define COLOR_GHOSTWHITE                0xf8f8ffff
#define COLOR_GOLD                      0xffd700ff
#define COLOR_GOLDENROD                 0xdaa520ff
#define COLOR_GRAY                      0x808080ff
#define COLOR_GREEN                     0x008000ff
#define COLOR_GREENYELLOW               0xadff2fff
#define COLOR_GREY                      0x808080ff /* same as COLOR_GRAY */
#define COLOR_HONEYDEW                  0xf0fff0ff
#define COLOR_HOTPINK                   0xff69b4ff
#define COLOR_INDIANRED                 0xcd5c5cff
#define COLOR_INDIGO                    0x4b0082ff
#define COLOR_IVORY                     0xfffff0ff
#define COLOR_KHAKI                     0xf0e68cff
#define COLOR_LAVENDER                  0xe6e6faff
#define COLOR_LAVENDERBLUSH             0xfff0f5ff
#define COLOR_LAWNGREEN                 0x7cfc00ff
#define COLOR_LEMONCHIFFON              0xfffacdff
#define COLOR_LIGHTBLUE                 0xadd8e6ff
#define COLOR_LIGHTCORAL                0xf08080ff
#define COLOR_LIGHTCYAN                 0xe0ffffff
#define COLOR_LIGHTGOLDENRODYELLOW      0xfafad2ff
#define COLOR_LIGHTGRAY                 0xd3d3d3ff
#define COLOR_LIGHTGREEN                0x90ee90ff
#define COLOR_LIGHTGREY                 0xd3d3d3ff
#define COLOR_LIGHTPINK                 0xffb6c1ff
#define COLOR_LIGHTSALMON               0xffa07aff
#define COLOR_LIGHTSEAGREEN             0x20b2aaff
#define COLOR_LIGHTSKYBLUE              0x87cefaff
#define COLOR_LIGHTSLATEGRAY            0x778899ff
#define COLOR_LIGHTSLATEGREY            0x778899ff
#define COLOR_LIGHTSTEELBLUE            0xb0c4deff
#define COLOR_LIGHTYELLOW               0xffffe0ff
#define COLOR_LIME                      0x00ff00ff
#define COLOR_LIMEGREEN                 0x32cd32ff
#define COLOR_LINEN                     0xfaf0e6ff
#define COLOR_MAGENTA                   0xff00ffff /* same as COLOR_FUCHSIA */
#define COLOR_MAROON                    0x800000ff
#define COLOR_MEDIUMAQUAMARINE          0x66cdaaff
#define COLOR_MEDIUMBLUE                0x0000cdff
#define COLOR_MEDIUMORCHID              0xba55d3ff
#define COLOR_MEDIUMPURPLE              0x9370dbff
#define COLOR_MEDIUMSEAGREEN            0x3cb371ff
#define COLOR_MEDIUMSLATEBLUE           0x7b68eeff
#define COLOR_MEDIUMSPRINGGREEN         0x00fa9aff
#define COLOR_MEDIUMTURQUOISE           0x48d1ccff
#define COLOR_MEDIUMVIOLETRED           0xc71585ff
#define COLOR_MIDNIGHTBLUE              0x191970ff
#define COLOR_MINTCREAM                 0xf5fffaff
#define COLOR_MISTYROSE                 0xffe4e1ff
#define COLOR_MOCCASIN                  0xffe4b5ff
#define COLOR_NAVAJOWHITE               0xffdeadff
#define COLOR_NAVY                      0x000080ff
#define COLOR_OLDLACE                   0xfdf5e6ff
#define COLOR_OLIVE                     0x808000ff
#define COLOR_OLIVEDRAB                 0x6b8e23ff
#define COLOR_ORANGE                    0xffa500ff
#define COLOR_ORANGERED                 0xff4500ff
#define COLOR_ORCHID                    0xda70d6ff
#define COLOR_PALEGOLDENROD             0xeee8aaff
#define COLOR_PALEGREEN                 0x98fb98ff
#define COLOR_PALETURQUOISE             0xafeeeeff
#define COLOR_PALEVIOLETRED             0xdb7093ff
#define COLOR_PAPAYAWHIP                0xffefd5ff
#define COLOR_PEACHPUFF                 0xffdab9ff
#define COLOR_PERU                      0xcd853fff
#define COLOR_PINK                      0xffc0cbff
#define COLOR_PLUM                      0xdda0ddff
#define COLOR_POWDERBLUE                0xb0e0e6ff
#define COLOR_PURPLE                    0x800080ff
#define COLOR_REBECCAPURPLE             0x663399ff /* <3 */
#define COLOR_RED                       0xff0000ff
#define COLOR_ROSYBROWN                 0xbc8f8fff
#define COLOR_ROYALBLUE                 0x4169e1ff
#define COLOR_SADDLEBROWN               0x8b4513ff
#define COLOR_SALMON                    0xfa8072ff
#define COLOR_SANDYBROWN                0xf4a460ff
#define COLOR_SEAGREEN                  0x2e8b57ff
#define COLOR_SEASHELL                  0xfff5eeff
#define COLOR_SIENNA                    0xa0522dff
#define COLOR_SILVER                    0xc0c0c0ff
#define COLOR_SKYBLUE                   0x87ceebff
#define COLOR_SLATEBLUE                 0x6a5acdff
#define COLOR_SLATEGRAY                 0x708090ff
#define COLOR_SLATEGREY                 0x708090ff
#define COLOR_SNOW                      0xfffafaff
#define COLOR_SPRINGGREEN               0x00ff7fff
#define COLOR_STEELBLUE                 0x4682b4ff
#define COLOR_TAN                       0xd2b48cff
#define COLOR_TEAL                      0x008080ff
#define COLOR_THISTLE                   0xd8bfd8ff
#define COLOR_TOMATO                    0xff6347ff
#define COLOR_TRANSPARENT               0x00000000
#define COLOR_TURQUOISE                 0x40e0d0ff
#define COLOR_VIOLET                    0xee82eeff
#define COLOR_WHEAT                     0xf5deb3ff
#define COLOR_WHITE                     0xffffffff
#define COLOR_WHITESMOKE                0xf5f5f5ff
#define COLOR_YELLOW                    0xffff00ff
#define COLOR_YELLOWGREEN               0x9acd32ff

#endif /* !__GUI_COLOR */
