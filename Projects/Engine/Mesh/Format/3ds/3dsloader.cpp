
// Header
#include "3dsloader.h"

// Library includes
#include <conio.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>

// Project includes

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

char load3DS(Mesh::mesh3ds_vbo *p_object, const std::string& filename)
{
	int i; //Index variable

	FILE *pFile = NULL; //File pointer

	unsigned short l_chunk_id; //Chunk identifier
	unsigned int l_chunk_lenght; //Chunk lenght
	unsigned char l_char; //Char variable
	unsigned short l_qty; //Number of elements in each chunk

	unsigned short l_face_flags; //Flag that stores some face information

	fopen_s(&pFile, filename.c_str(), "rb");

	if ( !pFile ) {
		return 0;
	}

	while ( ftell(pFile) < _filelength( _fileno(pFile)) ) {						// Loop to scan the whole file 
		fread (&l_chunk_id, 2, 1, pFile);										// Read the chunk header
		fread (&l_chunk_lenght, 4, 1, pFile);									// Read the lenght of the chunk

		switch ( l_chunk_id ) {
			//----------------- MAIN3DS -----------------
			// Description: Main chunk, contains all the other chunks
			// Chunk ID: 4d4d 
			// Chunk Lenght: 0 + sub chunks
			//-------------------------------------------
			case 0x4d4d: 
				break;    

			//----------------- EDIT3DS -----------------
			// Description: 3D Editor chunk, objects layout info 
			// Chunk ID: 3d3d (hex)
			// Chunk Lenght: 0 + sub chunks
			//-------------------------------------------
			case 0x3d3d:
				break;

			//--------------- EDIT_OBJECT ---------------
			// Description: Object block, info for each object
			// Chunk ID: 4000 (hex)
			// Chunk Lenght: len(object name) + sub chunks
			//-------------------------------------------
			case 0x4000: 
				i = 0;
				do {
					fread(&l_char, 1, 1, pFile);
                    p_object->name[i] = l_char;
					i++;
                } while ( l_char != '\0' && i < 20 );
				break;

			//--------------- OBJ_TRIMESH ---------------
			// Description: Triangular mesh, contains chunks for 3d mesh info
			// Chunk ID: 4100 (hex)
			// Chunk Lenght: 0 + sub chunks
			//-------------------------------------------
			case 0x4100:
				break;
			
			//--------------- TRI_VERTEXL ---------------
			// Description: Vertices list
			// Chunk ID: 4110 (hex)
			// Chunk Lenght: 1 x unsigned short (number of vertices) 
			//             + 3 x float (vertex coordinates) x (number of vertices)
			//             + sub chunks
			//-------------------------------------------
			case 0x4110: 
				fread(&l_qty, sizeof (unsigned short), 1, pFile);
				p_object->vertex.resize(l_qty);
                
				for ( i = 0; i < l_qty; i += 1 ) {
					vector3f v;

					fread(&v.x, sizeof(float), 1, pFile);
                    fread(&v.z, sizeof(float), 1, pFile);
					fread(&v.y, sizeof(float), 1, pFile);

					p_object->vertex[i] = v;
				}
				break;

			//--------------- TRI_FACEL1 ----------------
			// Description: Polygons (faces) list
			// Chunk ID: 4120 (hex)
			// Chunk Lenght: 1 x unsigned short (number of polygons) 
			//             + 3 x unsigned short (polygon points) x (number of polygons)
			//             + sub chunks
			//-------------------------------------------
			case 0x4120:
				fread(&l_qty, sizeof(unsigned short), 1, pFile);
				p_object->polygon.resize(l_qty);

				for ( i = 0; i < l_qty; i += 1 ) {
					vector4<unsigned short> p;

					fread (&p.x, sizeof(unsigned short), 1, pFile);
					fread (&p.y, sizeof(unsigned short), 1, pFile);
					fread (&p.z, sizeof(unsigned short), 1, pFile);
					fread (&l_face_flags, sizeof(unsigned short), 1, pFile);

					p_object->polygon[i] = p;
				}
                break;

			//case 0x4130:
			//	break;

			//------------- TRI_MAPPINGCOORS ------------
			// Description: Vertices list
			// Chunk ID: 4140 (hex)
			// Chunk Lenght: 1 x unsigned short (number of mapping points) 
			//             + 2 x float (mapping coordinates) x (number of mapping points)
			//             + sub chunks
			//-------------------------------------------
			case 0x4140:
				fread(&l_qty, sizeof(unsigned short), 1, pFile);
				p_object->texture.resize(l_qty);

				for ( i = 0; i < l_qty; i += 1 ) {
					vector2<float> m;

					fread (&m.x, sizeof(float), 1, pFile);
                    fread (&m.y, sizeof(float), 1, pFile);

					p_object->texture[i] = m;
				}
                break;

			//case 0x4150:
			//	break;

			//----------- Skip unknow chunks ------------
			// We need to skip all the chunks that currently we don't use.
			// We use the chunk length information to set the file pointer
			// to the same level next chunk.
			//-------------------------------------------
			default:
				 fseek(pFile, l_chunk_lenght - 6, SEEK_CUR);
				 break;
        } 
	}

	fclose(pFile);

	return 1;
}
