
#ifndef _Mesh_Format_3ds_3dsloader_h_
#define _Mesh_Format_3ds_3dsloader_h_


// Library includes
#include <string>

// Project includes
#include "Types.h"

// Forward declarations

// Namespace declarations


/**********************************************************
 *
 * FUNCTION Load3DS (obj_type_ptr, char *)
 *
 * This function loads a mesh from a 3ds file.
 * Please note that we are loading only the vertices, polygons and mapping lists.
 * If you need to load meshes with advanced features as for example: 
 * multi objects, materials, lights and so on, you must insert other chunk parsers.
 *
 *********************************************************/

extern char load3DS(Mesh::mesh3ds_vbo *p_object, const std::string& filename);


#endif
