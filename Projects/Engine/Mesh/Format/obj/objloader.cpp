
// Header
#include "objloader.h"

// Library includes
#include <stdio.h>
#include <string>

// Project includes
#include <Math/Vector2.h>
#include <Math/Vector3.h>

// Namespace declarations


bool loadOBJ(Mesh::obj_t *obj, const std::string& filename)
{
	if ( !obj ) {
		return false;
	}

	FILE *file = 0;

	int err = fopen_s(&file, filename.c_str(), "r");
	if ( err ) {
		return false;
	}

	while ( true ) {
		char lineHeader[128];

		// read the first word of the line
		int res = fscanf_s(file, "%s", lineHeader);
		if ( res == EOF ) {
			break; // EOF = End Of File. Quit the loop.
		}
	 
		// else : parse lineHeader
		if ( strcmp( lineHeader, "v" ) == 0 ) {
			vector3f vertex;
			res = fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			obj->vertex.push_back(vertex);
		}
		else if ( strcmp( lineHeader, "vt" ) == 0 ) {
			vector2f uv;
			res = fscanf_s(file, "%f %f\n", &uv.x, &uv.y );
			obj->texture.push_back(uv);
		}
		else if ( strcmp( lineHeader, "vn" ) == 0 ) {
			vector3f normal;
			res = fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			obj->normal.push_back(normal);
		}
		else if ( strcmp( lineHeader, "f" ) == 0 ) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			if ( matches != 9 ) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}

			obj->vertex_index.push_back(vertexIndex[0]);
			obj->vertex_index.push_back(vertexIndex[1]);
			obj->vertex_index.push_back(vertexIndex[2]);
			obj->texture_index.push_back(uvIndex[0]);
			obj->texture_index.push_back(uvIndex[1]);
			obj->texture_index.push_back(uvIndex[2]);
			obj->normal_index.push_back(normalIndex[0]);
			obj->normal_index.push_back(normalIndex[1]);
			obj->normal_index.push_back(normalIndex[2]);
		}
	}

	return true;
}
