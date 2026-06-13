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
 * 2025, beaver, liquid wood noise distortion

id=start displace amount-x=40  displace-mode=cartesian aux=[ ref=start
noise-solid detail=3 seed=24 tileable=false  x-size=2.5 y-size=2.5 width=812 height=1582 
alien-map cpn-1-frequency=18 cpn-2-frequency=2.2 cpn-3-frequency=3  
gimp:desaturate mode=average
multiply value=1.2
add value=0.01
]


]

Yeah, this plugin was inspired by Grokꞌs AI generated Liquid Noise plugin, mine is definitely better
and it was made manually.
]

end of syntax
 */

#include "config.h"
#include <glib/gi18n-lib.h>


#ifdef GEGL_PROPERTIES

enum_start (gegl_displace_mode5312)
  enum_value (GEGL_DISPLACE_MODE_CARTESIAN, "cartesian", N_("Cartesian"))
  enum_value (GEGL_DISPLACE_MODE_POLAR, "polar", N_("Polar"))
enum_end (GeglDisplaceMode5312)

property_enum (displace_mode, _("Displacement mode"),
                GeglDisplaceMode5312, gegl_displace_mode5312,
                GEGL_DISPLACE_MODE_CARTESIAN)
  description (_("Mode of displacement"))

/* 
  
I donꞌt think its needed

property_enum (displace_sampler_type, _("Sampler"),
               GeglSamplerType, gegl_sampler_type,
               GEGL_SAMPLER_CUBIC)
  description (_("Type of GeglSampler used to fetch input pixels"))

property_enum (displace_abyss_policy, _("Abyss policy"),
               GeglAbyssPolicy, gegl_abyss_policy,
               GEGL_ABYSS_CLAMP)
  description (_("How image edges are handled"))
*/



property_double (displace_amount_x, _("Displace Horizontally"), -67.0)
    description (_("Displace multiplier for X or radial direction."))
    value_range (-100.0, 100.0)
    ui_range    (-100.0, 100.0)
    ui_meta     ("unit", "pixel-distance")
    ui_meta     ("label", "[displace-mode {cartesian} : cartesian-label,"
                          " displace-mode {polar}     : polar-label]")
    ui_meta     ("cartesian-label", _("Horizontal displacement"))
    ui_meta     ("polar-label", _("Pinch"))
    ui_meta     ("description", "[displace-mode {cartesian} : cartesian-description,"
                                " displace-mode {polar}     : polar-description]")
    ui_meta     ("cartesian-description", _("Displacement multiplier for the horizontal direction"))
    ui_meta     ("polar-description", _("Displacement multiplier for the radial direction"))

property_double (displace_amount_y, _("Displace Vertically"), 12.0)
    description (_("Displace multiplier for Y or tangent (degrees) direction"))
    value_range (-100.0, 100.0)
    ui_range    (-100.0, 100.0)
    ui_meta     ("unit", "pixel-distance")
    ui_meta     ("label", "[displace-mode {cartesian} : cartesian-label,"
                          " displace-mode {polar}     : polar-label]")
    ui_meta     ("cartesian-label", _("Vertical displacement"))
    ui_meta     ("polar-label", _("Whirl"))
    ui_meta     ("description", "[displace-mode {cartesian} : cartesian-description,"
                                " displace-mode {polar}     : polar-description]")
    ui_meta     ("cartesian-description", _("Displacement multiplier for the vertical direction"))
    ui_meta     ("polar-description", _("Displacement multiplier for the angular offset"))





property_double (scale, _("Scale"), 1.1)
    description (_("Reverse scaling size of the noise"))
    value_range (0.1, 8.0)
    ui_range    (0.1, 3.0)


property_int    (x_stretch, _("Stretch X"), 370)
    description (_("Stretch the noise horizontally"))
    value_range (300, 1800)
    ui_range    (300, 1800)
    ui_meta     ("unit", "pixel-distance")

property_int (y_stretch, _("Stretch Y"), 426)
    description(_("Stretch the noise vertically"))
    value_range (300, 1800)
    ui_range    (300, 1800)
    ui_meta     ("unit", "pixel-distance")

property_int    (liquid, _("Liquid"), 3)
    description (_("Intensity of the liquid distortion on the rings"))
    ui_range    (0, 4)
    value_range (0, 4)

property_seed (seed, _("Random seed"), rand)




#else

#define GEGL_OP_META
#define GEGL_OP_NAME     liquidwooddistortion
#define GEGL_OP_C_SOURCE liquidwooddistortion.c

#include "gegl-op.h"



typedef struct
{
 GeglNode *input;
 GeglNode *noisesolid;
 GeglNode *graph;
 GeglNode *normal;
 GeglNode *crop;
 GeglNode *displace;
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
" alien-map cpn-1-frequency=18 cpn-2-frequency=2.2 cpn-3-frequency=3   multiply value=1.5 gimp:desaturate mode=average add value=0.01 "\

  state->graph    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", EMBEDDEDGRAPH, 
                                  NULL);


  state->normal  = gegl_node_new_child (gegl, "operation", "gegl:over",  NULL);



  state->crop   = gegl_node_new_child (gegl, "operation", "gegl:crop",  NULL);



  state->displace   = gegl_node_new_child (gegl, "operation", "gegl:displace", "center", 0, "amount-x", 40.0, "amount-y", 0,  "displace-mode", 0,    NULL);



/*meta redirect property to new child orders go here

 gegl_operation_meta_redirect (operation, "propertyname", state->newchildname,  "originalpropertyname");
*/
 gegl_operation_meta_redirect (operation, "scale", state->noisesolid,  "x-size");
 gegl_operation_meta_redirect (operation, "scale", state->noisesolid,  "y-size");
 gegl_operation_meta_redirect (operation, "x_stretch", state->noisesolid,  "width");
 gegl_operation_meta_redirect (operation, "y_stretch", state->noisesolid,  "height");
 gegl_operation_meta_redirect (operation, "liquid", state->noisesolid,  "detail");
 gegl_operation_meta_redirect (operation, "seed", state->noisesolid,  "seed");



 gegl_operation_meta_redirect (operation, "displace-mode", state->displace,  "displace-mode");
 gegl_operation_meta_redirect (operation, "displace-amount-x", state->displace,  "amount-x");
 gegl_operation_meta_redirect (operation, "displace-amount-y", state->displace,  "amount-y");

}

static void
update_graph (GeglOperation *operation)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  State *state = o->user_data;
  if (!state) return;


  gegl_node_link_many (state->input, state->displace, state->output,  NULL);
  gegl_node_link_many (  state->noisesolid, state->graph, state->normal, state->crop,  NULL);
  gegl_node_connect (state->displace, "aux", state->crop, "output");
  gegl_node_connect (state->displace, "aux2", state->crop, "output");
  gegl_node_connect (state->crop, "aux", state->input, "output");

/*This is the first time I used an aux2 in a GEGL plugin!*/

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
    "name",        "lb:liquid-noise-distortion",
    "title",       _("Liquid Ring Distortion"),
    "reference-hash", "scarygirllaurenfromtotaldramaislandrebootwassocute",
    "description", _("Liquidated tree ring noise distortion effect"),
    "gimp:menu-path", "<Image>/Filters/Distorts",
    "gimp:menu-label", _("Liquid Ring Noise Distortion..."),
    NULL);
}

#endif
