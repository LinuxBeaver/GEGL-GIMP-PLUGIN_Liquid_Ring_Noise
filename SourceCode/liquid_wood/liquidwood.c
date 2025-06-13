/* This file is an image processing operation for GEGL
 *
 * GEGL is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * GEGL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with GEGL; if not, see <https://www.gnu.org/licenses/>.
 *
 * Credit to Øyvind Kolås (pippin) for major GEGL contributions
 * 2025, beaver, liquid wood noise

noise-solid detail=3 seed=0 tileable=false  x-size=2.5 y-size=2.5 width=812 height=1582 
alien-map cpn-1-frequency=18 cpn-2-frequency=2.2 cpn-3-frequency=3  
gimp:desaturate mode=average
multiply value=1.2
add value=0.01

id=1 over aux=[ ref=1 edge opacity value=0.2 ]

Yeah, this plugin was inspired by Grok's AI generated Liquid Noise plugin, mine is definitely better
and it was made manually.
]

end of syntax
 */

#include "config.h"
#include <glib/gi18n-lib.h>


#ifdef GEGL_PROPERTIES

enum_start (liquidwood_clownworld)
  enum_value (GEGL_BLEND_MODE_TYPE_NONE, "none",
              N_("None"))
  enum_value (GEGL_BLEND_MODE_TYPE_MULTIPLY, "multipy",
              N_("Multiply"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINMERGE,      "grainmerge",
              N_("Grain Merge"))
  enum_value (GEGL_BLEND_MODE_TYPE_HSLCOLOR,      "hslcolor",
              N_("HSL Color"))
  enum_value (GEGL_BLEND_MODE_TYPE_SOFTLIGHT,      "softlight",
              N_("Soft Light"))
  enum_value (GEGL_BLEND_MODE_TYPE_LCHCOLOR,      "lchcolor",
              N_("LCh Color"))
  enum_value (GEGL_BLEND_MODE_TYPE_OVERLAY,      "overlay",
              N_("Overlay"))
enum_end (liquidwoodclownworld)

property_enum (blendmode, _("Blend Mode of Color"),
    liquidwoodclownworld, liquidwood_clownworld,
    GEGL_BLEND_MODE_TYPE_GRAINMERGE)
    description (_("Blend Mode Color Overlay of the liquid wood noise"))

property_double (scale, _("Scale"), 1.1)
    description (_("Reverse scaling size of the noise"))
    value_range (0.1, 8.0)
    ui_range    (0.1, 3.0)


property_int    (x_stretch, _("Stretch X"), 400)
    description (_("Stretch the noise horizontally"))
    value_range (300, 1800)
    ui_range    (300, 1800)
    ui_meta     ("unit", "pixel-distance")

property_int (y_stretch, _("Stretch Y"), 1087)
    description(_("Stretch the noise vertically"))
    value_range (300, 1800)
    ui_range    (300, 1800)
    ui_meta     ("unit", "pixel-distance")

property_int    (liquid, _("Liquid"), 3)
    description (_("Intensity of the liquid distortion on the rings"))
    ui_range    (0, 4)
    value_range (0, 4)

property_double (glimmer, _("Glimmer effect"), 1.0)
    description (_("Glimmer effect on the noise"))
    value_range (0.0, 1.0)
    ui_range    (0.0, 1.0)
    ui_meta     ("unit", "pixel-distance")


property_seed (seed, _("Random seed"), rand)

property_color (color, _("Color"), "#a976ff")
    description (_("Color of the liquid ring noise"))
ui_meta ("visible", "!blendmode {none}")


#else

#define GEGL_OP_META
#define GEGL_OP_NAME     liquidwood
#define GEGL_OP_C_SOURCE liquidwood.c

#include "gegl-op.h"



typedef struct
{
 GeglNode *input;
 GeglNode *noisesolid;
 GeglNode *idref;
 GeglNode *opacity1;
 GeglNode *opacity2;
 GeglNode *graph;
 GeglNode *normal;
 GeglNode *edge;
 GeglNode *grainmerge;
 GeglNode *multiply;
 GeglNode *hslcolor;
 GeglNode *lchcolor;
 GeglNode *overlay;
 GeglNode *softlight;
 GeglNode *crop;
 GeglNode *color;
 GeglNode *none;
 GeglNode *output;
}State;

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglProperties *o = GEGL_PROPERTIES (operation);


  State *state = o->user_data = g_malloc0 (sizeof (State));

/*new child node list is here, this is where starred nodes get defined

 state->newchildname = gegl_node_new_child (gegl, "operation", "lb:name", NULL);*/
  state->input    = gegl_node_get_input_proxy (gegl, "input");
  state->output   = gegl_node_get_output_proxy (gegl, "output");

  state->noisesolid   = gegl_node_new_child (gegl, "operation", "gegl:noise-solid", "detail", 3, "tileable", 0, "x-size", 2.5, "y-size", 2.5, "width", 812, "height", 1582, NULL);

#define EMBEDDEDGRAPH \
" alien-map cpn-1-frequency=18 cpn-2-frequency=2.2 cpn-3-frequency=3   multiply value=1.2 gimp:desaturate mode=average add value=0.01 "\

  state->graph    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", EMBEDDEDGRAPH, 
                                  NULL);

  state->idref   = gegl_node_new_child (gegl, "operation", "gegl:nop",  NULL);


  state->normal  = gegl_node_new_child (gegl, "operation", "gegl:over",  NULL);

  state->edge   = gegl_node_new_child (gegl, "operation", "gegl:edge",  NULL);


  state->crop   = gegl_node_new_child (gegl, "operation", "gegl:crop",  NULL);


  state->color   = gegl_node_new_child (gegl, "operation", "gegl:color",  NULL);


  state->opacity1   = gegl_node_new_child (gegl, "operation", "gegl:opacity",  NULL);

  state->opacity2   = gegl_node_new_child (gegl, "operation", "gegl:opacity", "value", 0.55,  NULL);

  state->none   = gegl_node_new_child (gegl, "operation", "gegl:dst",  NULL);

state->grainmerge = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 47, "composite-mode", 0, "blend-space", 4, NULL);

state->multiply = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 30, "composite-mode", 0, NULL);

state->lchcolor = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 26, "composite-mode", 0,  "blend-space", 3, NULL);

state->hslcolor = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 39, "composite-mode", 0, NULL);

state->overlay = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 23, "composite-mode", 0, NULL);


state->softlight = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 45, "composite-mode", 0, NULL);


/*meta redirect property to new child orders go here

 gegl_operation_meta_redirect (operation, "propertyname", state->newchildname,  "originalpropertyname");
*/
 gegl_operation_meta_redirect (operation, "scale", state->noisesolid,  "x-size");
 gegl_operation_meta_redirect (operation, "scale", state->noisesolid,  "y-size");
 gegl_operation_meta_redirect (operation, "x_stretch", state->noisesolid,  "width");
 gegl_operation_meta_redirect (operation, "y_stretch", state->noisesolid,  "height");
 gegl_operation_meta_redirect (operation, "liquid", state->noisesolid,  "detail");
 gegl_operation_meta_redirect (operation, "seed", state->noisesolid,  "seed");
 gegl_operation_meta_redirect (operation, "glimmer", state->opacity1,  "value");
 gegl_operation_meta_redirect (operation, "color", state->color,  "value");


}

static void
update_graph (GeglOperation *operation)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  State *state = o->user_data;
  if (!state) return;

  GeglNode *blend = state->grainmerge;
  switch (o->blendmode) {
    case GEGL_BLEND_MODE_TYPE_NONE: blend = state->none; break;
    case GEGL_BLEND_MODE_TYPE_MULTIPLY: blend = state->multiply; break;
    case GEGL_BLEND_MODE_TYPE_GRAINMERGE: blend = state->grainmerge; break;
    case GEGL_BLEND_MODE_TYPE_HSLCOLOR: blend = state->hslcolor; break;
    case GEGL_BLEND_MODE_TYPE_SOFTLIGHT: blend = state->softlight; break;
    case GEGL_BLEND_MODE_TYPE_LCHCOLOR: blend = state->lchcolor; break;
    case GEGL_BLEND_MODE_TYPE_OVERLAY: blend = state->overlay; break;


default: blend = state->grainmerge;
}

  gegl_node_link_many (state->input,  state->noisesolid, state->graph, state->idref, state->normal, blend, state->crop, state->output,  NULL);
  gegl_node_link_many (state->idref, state->edge, state->opacity1, state->opacity2,  NULL);
  gegl_node_connect (state->normal, "aux", state->opacity2, "output");
  gegl_node_connect (blend, "aux", state->color, "output");
  gegl_node_connect (state->crop, "aux", state->input, "output");

}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;
GeglOperationMetaClass *operation_meta_class = GEGL_OPERATION_META_CLASS (klass);
  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_class->attach = attach;
  operation_meta_class->update = update_graph;

  gegl_operation_class_set_keys (operation_class,
    "name",        "lb:liquid-noise",
    "title",       _("Liquid Ring Noise"),
    "reference-hash", "fairygeglvisitsneotoyko",
    "description", _("Liquidated tree ring noise"),
    "gimp:menu-path", "<Image>/Filters/Noise",
    "gimp:menu-label", _("Liquid Ring Noise..."),
    NULL);
}

#endif
