/**
 * Copyright (c) 2026, sonkajarvi
 *
 * Licensed under the BSD 2-Clause License.
 * The full license can be found in the LICENSE.txt file.
 */

#ifndef _IGUANA_COLOR_H
#define _IGUANA_COLOR_H

#include <stdint.h>

/*
 * Colors are unsigned 32-bit integers, and are in RGBA format where the most
 * significant byte is the red channel.
 */
#define IGU_COLOR_R(rgba) (((rgba) >> 24) & 255)
#define IGU_COLOR_G(rgba) (((rgba) >> 16) & 255)
#define IGU_COLOR_B(rgba) (((rgba) >>  8) & 255)
#define IGU_COLOR_A(rgba) (((rgba)      ) & 255)

#define IGU_COLOR_INT_TO_FLOAT(channel) ((float)(channel) / 255.0f)

/**
 * color_to_3f - Transform RGB color to three floats.
 * @rgb: The color to transform.
 * @out: Buffer to write to.
 */
static inline void igu_color_to_3f(uint32_t rgb, float out[static 3])
{
    out[0] = IGU_COLOR_INT_TO_FLOAT(IGU_COLOR_R(rgb));
    out[1] = IGU_COLOR_INT_TO_FLOAT(IGU_COLOR_G(rgb));
    out[2] = IGU_COLOR_INT_TO_FLOAT(IGU_COLOR_B(rgb));
}

/**
 * color_to_4f - Transform RGBA color to four floats.
 * @rgba: The color to transform.
 * @out: Buffer to write to.
 */
static inline void igu_color_to_4f(uint32_t rgba, float out[static 4])
{
    igu_color_to_3f(rgba, out);
    out[3] = IGU_COLOR_INT_TO_FLOAT(IGU_COLOR_A(rgba));
}

/* TODO: make this debug only */
const char *igu_color_to_string(uint32_t rgba);

/*
 * These colors are the same as the CSS named colors.
 * See: https://developer.mozilla.org/en-US/docs/Web/CSS/Reference/Values/named-color
 */
#define IGU_ALICEBLUE                   0xf0f8ffff
#define IGU_ANTIQUEWHITE                0xfaebd7ff
#define IGU_AQUA                        0x00ffffff
#define IGU_AQUAMARINE                  0x7fffd4ff
#define IGU_AZURE                       0xf0ffffff
#define IGU_BEIGE                       0xf5f5dcff
#define IGU_BISQUE                      0xffe4c4ff
#define IGU_BLACK                       0x000000ff
#define IGU_BLANCHEDALMOND              0xffebcdff
#define IGU_BLUE                        0x0000ffff
#define IGU_BLUEVIOLET                  0x8a2be2ff
#define IGU_BROWN                       0xa52a2aff
#define IGU_BURLYWOOD                   0xdeb887ff
#define IGU_CADETBLUE                   0x5f9ea0ff
#define IGU_CHARTREUSE                  0x7fff00ff
#define IGU_CHOCOLATE                   0xd2691eff
#define IGU_CORAL                       0xff7f50ff
#define IGU_CORNFLOWERBLUE              0x6495edff
#define IGU_CORNSILK                    0xfff8dcff
#define IGU_CRIMSON                     0xdc143cff
#define IGU_CYAN                        0x00ffffff /* same as COLOR_AQUA */
#define IGU_DARKBLUE                    0x00008bff
#define IGU_DARKCYAN                    0x008b8bff
#define IGU_DARKGOLDENROD               0xb8860bff
#define IGU_DARKGRAY                    0xa9a9a9ff
#define IGU_DARKGREEN                   0x006400ff
#define IGU_DARKGREY                    0xa9a9a9ff
#define IGU_DARKKHAKI                   0xbdb76bff
#define IGU_DARKMAGENTA                 0x8b008bff
#define IGU_DARKOLIVEGREEN              0x556b2fff
#define IGU_DARKORANGE                  0xff8c00ff
#define IGU_DARKORCHID                  0x9932ccff
#define IGU_DARKRED                     0x8b0000ff
#define IGU_DARKSALMON                  0xe9967aff
#define IGU_DARKSEAGREEN                0x8fbc8fff
#define IGU_DARKSLATEBLUE               0x483d8bff
#define IGU_DARKSLATEGRAY               0x2f4f4fff
#define IGU_DARKSLATEGREY               0x2f4f4fff
#define IGU_DARKTURQUOISE               0x00ced1ff
#define IGU_DARKVIOLET                  0x9400d3ff
#define IGU_DEEPPINK                    0xff1493ff
#define IGU_DEEPSKYBLUE                 0x00bfffff
#define IGU_DIMGRAY                     0x696969ff
#define IGU_DIMGREY                     0x696969ff
#define IGU_DODGERBLUE                  0x1e90ffff
#define IGU_FIREBRICK                   0xb22222ff
#define IGU_FLORALWHITE                 0xfffaf0ff
#define IGU_FORESTGREEN                 0x228b22ff
#define IGU_FUCHSIA                     0xff00ffff
#define IGU_GAINSBORO                   0xdcdcdcff
#define IGU_GHOSTWHITE                  0xf8f8ffff
#define IGU_GOLD                        0xffd700ff
#define IGU_GOLDENROD                   0xdaa520ff
#define IGU_GRAY                        0x808080ff
#define IGU_GREEN                       0x008000ff
#define IGU_GREENYELLOW                 0xadff2fff
#define IGU_GREY                        0x808080ff /* same as COLOR_GRAY */
#define IGU_HONEYDEW                    0xf0fff0ff
#define IGU_HOTPINK                     0xff69b4ff
#define IGU_INDIANRED                   0xcd5c5cff
#define IGU_INDIGO                      0x4b0082ff
#define IGU_IVORY                       0xfffff0ff
#define IGU_KHAKI                       0xf0e68cff
#define IGU_LAVENDER                    0xe6e6faff
#define IGU_LAVENDERBLUSH               0xfff0f5ff
#define IGU_LAWNGREEN                   0x7cfc00ff
#define IGU_LEMONCHIFFON                0xfffacdff
#define IGU_LIGHTBLUE                   0xadd8e6ff
#define IGU_LIGHTCORAL                  0xf08080ff
#define IGU_LIGHTCYAN                   0xe0ffffff
#define IGU_LIGHTGOLDENRODYELLOW        0xfafad2ff
#define IGU_LIGHTGRAY                   0xd3d3d3ff
#define IGU_LIGHTGREEN                  0x90ee90ff
#define IGU_LIGHTGREY                   0xd3d3d3ff
#define IGU_LIGHTPINK                   0xffb6c1ff
#define IGU_LIGHTSALMON                 0xffa07aff
#define IGU_LIGHTSEAGREEN               0x20b2aaff
#define IGU_LIGHTSKYBLUE                0x87cefaff
#define IGU_LIGHTSLATEGRAY              0x778899ff
#define IGU_LIGHTSLATEGREY              0x778899ff
#define IGU_LIGHTSTEELBLUE              0xb0c4deff
#define IGU_LIGHTYELLOW                 0xffffe0ff
#define IGU_LIME                        0x00ff00ff
#define IGU_LIMEGREEN                   0x32cd32ff
#define IGU_LINEN                       0xfaf0e6ff
#define IGU_MAGENTA                     0xff00ffff /* same as COLOR_FUCHSIA */
#define IGU_MAROON                      0x800000ff
#define IGU_MEDIUMAQUAMARINE            0x66cdaaff
#define IGU_MEDIUMBLUE                  0x0000cdff
#define IGU_MEDIUMORCHID                0xba55d3ff
#define IGU_MEDIUMPURPLE                0x9370dbff
#define IGU_MEDIUMSEAGREEN              0x3cb371ff
#define IGU_MEDIUMSLATEBLUE             0x7b68eeff
#define IGU_MEDIUMSPRINGGREEN           0x00fa9aff
#define IGU_MEDIUMTURQUOISE             0x48d1ccff
#define IGU_MEDIUMVIOLETRED             0xc71585ff
#define IGU_MIDNIGHTBLUE                0x191970ff
#define IGU_MINTCREAM                   0xf5fffaff
#define IGU_MISTYROSE                   0xffe4e1ff
#define IGU_MOCCASIN                    0xffe4b5ff
#define IGU_NAVAJOWHITE                 0xffdeadff
#define IGU_NAVY                        0x000080ff
#define IGU_OLDLACE                     0xfdf5e6ff
#define IGU_OLIVE                       0x808000ff
#define IGU_OLIVEDRAB                   0x6b8e23ff
#define IGU_ORANGE                      0xffa500ff
#define IGU_ORANGERED                   0xff4500ff
#define IGU_ORCHID                      0xda70d6ff
#define IGU_PALEGOLDENROD               0xeee8aaff
#define IGU_PALEGREEN                   0x98fb98ff
#define IGU_PALETURQUOISE               0xafeeeeff
#define IGU_PALEVIOLETRED               0xdb7093ff
#define IGU_PAPAYAWHIP                  0xffefd5ff
#define IGU_PEACHPUFF                   0xffdab9ff
#define IGU_PERU                        0xcd853fff
#define IGU_PINK                        0xffc0cbff
#define IGU_PLUM                        0xdda0ddff
#define IGU_POWDERBLUE                  0xb0e0e6ff
#define IGU_PURPLE                      0x800080ff
#define IGU_REBECCAPURPLE               0x663399ff /* <3 */
#define IGU_RED                         0xff0000ff
#define IGU_ROSYBROWN                   0xbc8f8fff
#define IGU_ROYALBLUE                   0x4169e1ff
#define IGU_SADDLEBROWN                 0x8b4513ff
#define IGU_SALMON                      0xfa8072ff
#define IGU_SANDYBROWN                  0xf4a460ff
#define IGU_SEAGREEN                    0x2e8b57ff
#define IGU_SEASHELL                    0xfff5eeff
#define IGU_SIENNA                      0xa0522dff
#define IGU_SILVER                      0xc0c0c0ff
#define IGU_SKYBLUE                     0x87ceebff
#define IGU_SLATEBLUE                   0x6a5acdff
#define IGU_SLATEGRAY                   0x708090ff
#define IGU_SLATEGREY                   0x708090ff
#define IGU_SNOW                        0xfffafaff
#define IGU_SPRINGGREEN                 0x00ff7fff
#define IGU_STEELBLUE                   0x4682b4ff
#define IGU_TAN                         0xd2b48cff
#define IGU_TEAL                        0x008080ff
#define IGU_THISTLE                     0xd8bfd8ff
#define IGU_TOMATO                      0xff6347ff
#define IGU_TRANSPARENT                 0x00000000
#define IGU_TURQUOISE                   0x40e0d0ff
#define IGU_VIOLET                      0xee82eeff
#define IGU_WHEAT                       0xf5deb3ff
#define IGU_WHITE                       0xffffffff
#define IGU_WHITESMOKE                  0xf5f5f5ff
#define IGU_YELLOW                      0xffff00ff
#define IGU_YELLOWGREEN                 0x9acd32ff

#endif /* !_IGUANA_COLOR_H */
