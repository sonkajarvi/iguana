/**
 * Copyright (c) 2026, sonkajarvi
 *
 * Licensed under the BSD 2-Clause License.
 * The full license can be found in the LICENSE.txt file.
 */

#include <stddef.h>

#include <gui/color.h>

const char *color_to_string(uint32_t rgba)
{
    switch (rgba) {
    default: return NULL;
    case 0xf0f8ffff: return "aliceblue";
    case 0xfaebd7ff: return "antiquewhite";
    case 0x00ffffff: return "aqua cyan";
    case 0x7fffd4ff: return "aquamarine";
    case 0xf0ffffff: return "azure";
    case 0xf5f5dcff: return "beige";
    case 0xffe4c4ff: return "bisque";
    case 0x000000ff: return "black";
    case 0xffebcdff: return "blanchedalmond";
    case 0x0000ffff: return "blue";
    case 0x8a2be2ff: return "blueviolet";
    case 0xa52a2aff: return "brown";
    case 0xdeb887ff: return "burlywood";
    case 0x5f9ea0ff: return "cadetblue";
    case 0x7fff00ff: return "chartreuse";
    case 0xd2691eff: return "chocolate";
    case 0xff7f50ff: return "coral";
    case 0x6495edff: return "cornflowerblue";
    case 0xfff8dcff: return "cornsilk";
    case 0xdc143cff: return "crimson";
    case 0x00008bff: return "darkblue";
    case 0x008b8bff: return "darkcyan";
    case 0xb8860bff: return "darkgoldenrod";
    case 0xa9a9a9ff: return "darkgray darkgrey";
    case 0x006400ff: return "darkgreen";
    case 0xbdb76bff: return "darkkhaki";
    case 0x8b008bff: return "darkmagenta";
    case 0x556b2fff: return "darkolivegreen";
    case 0xff8c00ff: return "darkorange";
    case 0x9932ccff: return "darkorchid";
    case 0x8b0000ff: return "darkred";
    case 0xe9967aff: return "darksalmon";
    case 0x8fbc8fff: return "darkseagreen";
    case 0x483d8bff: return "darkslateblue";
    case 0x2f4f4fff: return "darkslategray darkslategrey";
    case 0x00ced1ff: return "darkturquoise";
    case 0x9400d3ff: return "darkviolet";
    case 0xff1493ff: return "deeppink";
    case 0x00bfffff: return "deepskyblue";
    case 0x696969ff: return "dimgray dimgrey";
    case 0x1e90ffff: return "dodgerblue";
    case 0xb22222ff: return "firebrick";
    case 0xfffaf0ff: return "floralwhite";
    case 0x228b22ff: return "forestgreen";
    case 0xff00ffff: return "fuchsia magenta";
    case 0xdcdcdcff: return "gainsboro";
    case 0xf8f8ffff: return "ghostwhite";
    case 0xffd700ff: return "gold";
    case 0xdaa520ff: return "goldenrod";
    case 0x808080ff: return "gray grey";
    case 0x008000ff: return "green";
    case 0xadff2fff: return "greenyellow";
    case 0xf0fff0ff: return "honeydew";
    case 0xff69b4ff: return "hotpink";
    case 0xcd5c5cff: return "indianred";
    case 0x4b0082ff: return "indigo";
    case 0xfffff0ff: return "ivory";
    case 0xf0e68cff: return "khaki";
    case 0xe6e6faff: return "lavender";
    case 0xfff0f5ff: return "lavenderblush";
    case 0x7cfc00ff: return "lawngreen";
    case 0xfffacdff: return "lemonchiffon";
    case 0xadd8e6ff: return "lightblue";
    case 0xf08080ff: return "lightcoral";
    case 0xe0ffffff: return "lightcyan";
    case 0xfafad2ff: return "lightgoldenrodyellow";
    case 0xd3d3d3ff: return "lightgray lightgrey";
    case 0x90ee90ff: return "lightgreen";
    case 0xffb6c1ff: return "lightpink";
    case 0xffa07aff: return "lightsalmon";
    case 0x20b2aaff: return "lightseagreen";
    case 0x87cefaff: return "lightskyblue";
    case 0x778899ff: return "lightslategray lightslategrey";
    case 0xb0c4deff: return "lightsteelblue";
    case 0xffffe0ff: return "lightyellow";
    case 0x00ff00ff: return "lime";
    case 0x32cd32ff: return "limegreen";
    case 0xfaf0e6ff: return "linen";
    case 0x800000ff: return "maroon";
    case 0x66cdaaff: return "mediumaquamarine";
    case 0x0000cdff: return "mediumblue";
    case 0xba55d3ff: return "mediumorchid";
    case 0x9370dbff: return "mediumpurple";
    case 0x3cb371ff: return "mediumseagreen";
    case 0x7b68eeff: return "mediumslateblue";
    case 0x00fa9aff: return "mediumspringgreen";
    case 0x48d1ccff: return "mediumturquoise";
    case 0xc71585ff: return "mediumvioletred";
    case 0x191970ff: return "midnightblue";
    case 0xf5fffaff: return "mintcream";
    case 0xffe4e1ff: return "mistyrose";
    case 0xffe4b5ff: return "moccasin";
    case 0xffdeadff: return "navajowhite";
    case 0x000080ff: return "navy";
    case 0xfdf5e6ff: return "oldlace";
    case 0x808000ff: return "olive";
    case 0x6b8e23ff: return "olivedrab";
    case 0xffa500ff: return "orange";
    case 0xff4500ff: return "orangered";
    case 0xda70d6ff: return "orchid";
    case 0xeee8aaff: return "palegoldenrod";
    case 0x98fb98ff: return "palegreen";
    case 0xafeeeeff: return "paleturquoise";
    case 0xdb7093ff: return "palevioletred";
    case 0xffefd5ff: return "papayawhip";
    case 0xffdab9ff: return "peachpuff";
    case 0xcd853fff: return "peru";
    case 0xffc0cbff: return "pink";
    case 0xdda0ddff: return "plum";
    case 0xb0e0e6ff: return "powderblue";
    case 0x800080ff: return "purple";
    case 0x663399ff: return "rebeccapurple";
    case 0xff0000ff: return "red";
    case 0xbc8f8fff: return "rosybrown";
    case 0x4169e1ff: return "royalblue";
    case 0x8b4513ff: return "saddlebrown";
    case 0xfa8072ff: return "salmon";
    case 0xf4a460ff: return "sandybrown";
    case 0x2e8b57ff: return "seagreen";
    case 0xfff5eeff: return "seashell";
    case 0xa0522dff: return "sienna";
    case 0xc0c0c0ff: return "silver";
    case 0x87ceebff: return "skyblue";
    case 0x6a5acdff: return "slateblue";
    case 0x708090ff: return "slategray slategrey";
    case 0xfffafaff: return "snow";
    case 0x00ff7fff: return "springgreen";
    case 0x4682b4ff: return "steelblue";
    case 0xd2b48cff: return "tan";
    case 0x008080ff: return "teal";
    case 0xd8bfd8ff: return "thistle";
    case 0xff6347ff: return "tomato";
    case 0x00000000: return "transparent";
    case 0x40e0d0ff: return "turquoise";
    case 0xee82eeff: return "violet";
    case 0xf5deb3ff: return "wheat";
    case 0xffffffff: return "white";
    case 0xf5f5f5ff: return "whitesmoke";
    case 0xffff00ff: return "yellow";
    case 0x9acd32ff: return "yellowgreen";
    }
}