
// Header
#include "Md2.h"

// Library includes

// Project includes

// Namespace declarations


CLoadMD2::CLoadMD2(const Common::Logger *p)
: Common::Logger(p, "CLoadMD2")
{
	// Here we initialize our structures to 0
	memset(&m_Header, 0, sizeof(tMd2Header));

	// Set the pointers to null
	m_pSkins = 0;
	m_pTexCoords = 0;
	m_pTriangles = 0;
	m_pFrames = 0;
}


///////////////////////////////// IMPORT MD2 \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This is called by the client to open the .Md2 file, read it, then clean up
/////
///////////////////////////////// IMPORT MD2 \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

t3DModel* CLoadMD2::ImportMD2(char *strFileName, char *strTexture)
{
	char strMessage[255] = {0};

	// Open the MD2 file in binary
	fopen_s(&m_FilePointer, strFileName, "rb");

	// Make sure we have a valid file pointer (we found the file)
	if ( !m_FilePointer ) {
		// Display an error message and don't load anything if no file was found
		sprintf_s(strMessage, "ImportMD2(): '%s' not found!\n", strFileName);
		warn(strMessage);
		return false;
	}
	
	// Read the header data and store it in our m_Header member variable
	fread(&m_Header, 1, sizeof(tMd2Header), m_FilePointer);

	// Make sure the version is this crazy number '8' or else it's a bad egg
	if ( m_Header.version != 8 ) {
		// Display an error message for bad file format, then stop loading
		sprintf_s(strMessage, "ImportMD2(): Invalid file format (Version not 8): %s!\n", strFileName);
		warn(strMessage);
		return false;
	}

	// Read in the model and animation data
	ReadMD2Data();
	
	t3DModel* pModel = new t3DModel();

	// Here we pass in our model structure to it can store the read Quake data
	// in our own model and object structure data
	ConvertDataStructures(pModel);

	// If there is a valid texture name passed in, we want to set the texture data
	if ( strTexture ) {
		// Create a local material info structure
		tMaterialInfo texture;

		// Copy the name of the file into our texture file name variable
		strcpy_s(texture.strFile, strTexture);

		// Since there is only one texture for a .Md2 file, the ID is always 0
		texture.texureId = 0;

		// The tile or scale for the UV's is 1 to 1 (but Quake saves off a 0-256 ratio)
		texture.uTile = texture.uTile = 1;

		// We only have 1 material for a model
		pModel->numOfMaterials = 1;

		// Add the local material info to our model's material list
		pModel->pMaterials.push_back(texture);
	}

	// Clean up after everything
	CleanUp();

	// Return a success
	return pModel;
}


///////////////////////////////// READ MD2 DATA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function reads in all of the model's data, except the animation frames
/////
///////////////////////////////// READ MD2 DATA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CLoadMD2::ReadMD2Data()
{
	// Create a larger buffer for the frames of animation (not fully used yet)
	unsigned char buffer[MD2_MAX_FRAMESIZE];

	// Here we allocate all of our memory from the header's information
	m_pSkins     = new tMd2Skin [m_Header.numSkins];
	m_pTexCoords = new tMd2TexCoord [m_Header.numTexCoords];
	m_pTriangles = new tMd2Face [m_Header.numTriangles];
	m_pFrames    = new tMd2Frame [m_Header.numFrames];

	// Next, we start reading in the data by seeking to our skin names offset
	fseek(m_FilePointer, m_Header.offsetSkins, SEEK_SET);
	
	// Depending on the skin count, we read in each skin for this model
	fread(m_pSkins, sizeof(tMd2Skin), m_Header.numSkins, m_FilePointer);
	
	// Move the file pointer to the position in the file for texture coordinates
	fseek(m_FilePointer, m_Header.offsetTexCoords, SEEK_SET);
	
	// Read in all the texture coordinates in one fell swoop
	fread(m_pTexCoords, sizeof(tMd2TexCoord), m_Header.numTexCoords, m_FilePointer);

	// Move the file pointer to the triangles/face data offset
	fseek(m_FilePointer, m_Header.offsetTriangles, SEEK_SET);
	
	// Read in the face data for each triangle (vertex and texCoord indices)
	fread(m_pTriangles, sizeof(tMd2Face), m_Header.numTriangles, m_FilePointer);
			
	// Move the file pointer to the vertices (frames)
	fseek(m_FilePointer, m_Header.offsetFrames, SEEK_SET);

	// In the last tutorial we just read in one frame of animation here.  Now we are going
	// to extract every key frame from the .MD2 model.  These key frames will be used to
	// interpolate between each other to form the somewhat smooth animation.
	// The only thing different is that we are putting a for loop in front, then changing
	// the '0' for the m_pFrames[0] in the last tutorial, to a 'i'.  The loop will
	// continue until 'i' has reached the number of key frames for this model.

	for ( int i = 0; i < m_Header.numFrames; i += 1 ) {
		// Assign our alias frame to our buffer memory
		tMd2AliasFrame *pFrame = (tMd2AliasFrame *) buffer;

		// Allocate the memory for the first frame of animation's vertices
		m_pFrames[i].pVertices = new tMd2Triangle [m_Header.numVertices];

		// Read in the first frame of animation
		fread(pFrame, 1, m_Header.frameSize, m_FilePointer);

		// Copy the name of the animation to our frames array
		strcpy_s(m_pFrames[i].strName, pFrame->name);
			
		// Store off a vertex array pointer to cut down large lines of code
		tMd2Triangle *pVertices = m_pFrames[i].pVertices;

		// Go through all of the number of vertices and assign the scale and translations.
		// Store the vertices in our current frame's vertex list array, while swapping Y and Z.
		// Notice we also negate the Z axis as well to make the swap correctly.
		for ( int j = 0; j < m_Header.numVertices; j += 1 ) {
			pVertices[j].vertex[0] = pFrame->aliasVertices[j].vertex[0] * pFrame->scale[0] + pFrame->translate[0];
			pVertices[j].vertex[2] = -1 * (pFrame->aliasVertices[j].vertex[1] * pFrame->scale[1] + pFrame->translate[1]);
			pVertices[j].vertex[1] = pFrame->aliasVertices[j].vertex[2] * pFrame->scale[2] + pFrame->translate[2];
		}
	}
}


///////////////////////////////// PARSE ANIMATIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function fills in the animation list for each animation by name and frame
/////
///////////////////////////////// PARSE ANIMATIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CLoadMD2::ParseAnimations(t3DModel *pModel)
{
	tAnimationInfo animation;
	std::string strLastName = "";

	// This function felt like a hack when I wrote it.  You aren't really given
	// any good information about the animations, other than the fact that each
	// key frame has a name assigned to it with a frame number for that animation.
	// For instance, the first animation is the "stand" animation.  The first frame
	// would have the name of: "stand01" or perhaps "stand1".  The 40th frame is
	// usually the last frame for the standing animation, so it would look like:
	// "stand40".  After this frame, the next animation is the running animation.
	// The next frame is labeled something like "run01".  You now know that the
	// standing animation is from frame 1 to frame 40 of the total frames.  The
	// start of the run animation is 41.  We will know how long the run animation
	// goes when we run into another animation name besides "run..".  That is how
	// I went about finding out the animation information.  I just grab each frame
	// name and check if it's the same animation name as the last name we found.
	// If it is, I just ignore it and continue to the next frame.  Once I find that
	// it's not, I then have the last frame saved off from the index 'i', and then
	// I create a new animation to add to the list, then start from the beginning.
	// It wasn't until later that I found on www.planetquake.com that there is a
	// standard frame count for each animation and they are all the same.  I figure
	// this way makes it modular so you don't have to stick to those standards anyway.

	// Go through all of the frames of animation and parse each animation
	for ( int i = 0; i < pModel->numOfObjects; i += 1 ) {
		// Assign the name of this frame of animation to a string object
		std::string strName  = m_pFrames[i].strName;
		int frameNum = 0;
		
		// Go through and extract the frame numbers and erase them from the name
		for ( unsigned int j = 0; j < strName.length(); j += 1 ) {
			// If the current index is a number and it's one of the last 2 characters of the name
			if ( isdigit((unsigned char)strName[j]) && j >= strName.length() - 2 ) {
				// Use a C function to convert the character to a integer.
				// Notice we use the address to pass in the current character and on
				frameNum = atoi(&strName[j]);

				// Erase the frame number from the name so we extract the animation name
				strName.erase(j, strName.length() - j);
				break;
			}
		}

		// Check if this animation name is not the same as the last frame,
		// or if we are on the last frame of animation for this model
		if ( strName != strLastName || i == pModel->numOfObjects - 1 ) {
			// If this animation frame is NOT the first frame
			if ( strLastName != "" ) {
				// Copy the last animation name into our new animation's name
				strcpy_s(animation.strName, strLastName.c_str());

				// Set the last frame of this animation to i
				animation.endFrame = i;

				// Add the animation to our list and reset the animation object for next time
				pModel->pAnimations.push_back(animation);
				memset(&animation, 0, sizeof(tAnimationInfo));

				// Increase the number of animations for this model
				pModel->numOfAnimations++;
			}

			// Set the starting frame number to the current frame number we just found,
			// minus 1 (since 0 is the first frame) and add 'i'.
			animation.startFrame = frameNum - 1 + i;
		}

		// Store the current animation name in the strLastName string to check it latter
		strLastName = strName;
	}
}


///////////////////////////////// CONVERT DATA STRUCTURES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function converts the .md2 structures to our own model and object structures
/////
///////////////////////////////// CONVERT DATA STRUCTURES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CLoadMD2::ConvertDataStructures(t3DModel *pModel)
{
	int j = 0, i = 0;

	// Like the previous function, not a lot was changed her from the last tutorial.
	// Since we are dealing with multiple key frames, we need to pretty much do the 
	// same things for each frame.  This entails adding a for loop to go through
	// every frame.  We use index 'i' into the m_pFrames array to set each key frame.
	// This is one tricky thing in the middle of this for loop though, since we only
	// need to save the face and texture information once, this is only calculated
	// once and stored in the first frame of animation.  We can then reference the 
	// polygon information for every other frame from the first frame.  That way
	// we don't copy the same information for every key frame, which would make a huge
	// memory footprint.

	// Initialize the model structure first before assigning data to it
	//memset(pModel, 0, sizeof(t3DModel));

	// Set the number of objects to our number of frames since pObjects will
	// hold the key frames
	pModel->numOfObjects = m_Header.numFrames;

	// Create our animation list and store it in our model
	ParseAnimations(pModel);

	// Go through every key frame and store it's vertices info in our pObject list.
	for ( i = 0; i < pModel->numOfObjects; i+= 1 ) {
		// Create a local object to store the first frame of animation's data
		t3DObject currentFrame = {0};

		// Assign the vertex, texture coord and face count to our new structure
		currentFrame.numOfVerts   = m_Header.numVertices;
		currentFrame.numTexVertex = m_Header.numTexCoords;
		currentFrame.numOfFaces   = m_Header.numTriangles;

		// Allocate memory for the vertices, texture coordinates and face data.
		if ( currentFrame.numOfVerts <= 0 ) {
			return;
		}

		currentFrame.pVerts = new CModelVector3[currentFrame.numOfVerts];

		// Go through all of the vertices and assign them over to our structure
		for ( j = 0; j < currentFrame.numOfVerts; j += 1 ) {
			currentFrame.pVerts[j].x = m_pFrames[i].pVertices[j].vertex[0];
			currentFrame.pVerts[j].y = m_pFrames[i].pVertices[j].vertex[1];
			currentFrame.pVerts[j].z = m_pFrames[i].pVertices[j].vertex[2];
		}

		// We can now free the old vertices stored in this frame of animation
		// since we have them stored in the pObject list of our model
		delete [] m_pFrames[i].pVertices;

		// Now comes the tricky part, since we only need to store the UV coordinates
		// and face indices once, we only do this on the first frame.  If the
		// current frame is past the first frame, we just add the current frame
		// as it is to our list, then continue to the next frame.  If we are
		// on the first frame, we still need to store the UV and face information.
		// This will only go in the first frame, and will be referenced from the
		// remaining frames when we animate the model.

		// Check if we are past the first key frame
		if ( i > 0 ) {
			// Here we add the current object (or frame) to our list object list
			pModel->pObject.push_back(currentFrame);

			continue;	// Go on to the next key frame
		}

		// We will only get here ONE because we just need this information
		// calculated for the first key frame.

		// Allocate memory for our UV coordinates and face information
		currentFrame.pTexVerts = new CVector2[currentFrame.numTexVertex];
		currentFrame.pFaces    = new tFace[currentFrame.numOfFaces];

		// Go through all of the uv coordinates and assign them over to our structure.
		// The UV coordinates are not normal uv coordinates, they have a pixel ratio of
		// 0 to 256.  We want it to be a 0 to 1 ratio, so we divide the u value by the
		// skin width and the v value by the skin height.  This gives us our 0 to 1 ratio.
		// For some reason also, the v coodinate is flipped upside down.  We just subtract
		// the v coordinate from 1 to remedy this problem.
		for ( j = 0; j < currentFrame.numTexVertex; j += 1 ) {
			currentFrame.pTexVerts[j].x = m_pTexCoords[j].u / float(m_Header.skinWidth);
			currentFrame.pTexVerts[j].y = 1 - m_pTexCoords[j].v / float(m_Header.skinHeight);
		}

		// Go through all of the face data and assign it over to OUR structure
		for ( j = 0; j < currentFrame.numOfFaces; j += 1 ) {
			// Assign the vertex indices to our face data
			currentFrame.pFaces[j].vertIndex[0] = m_pTriangles[j].vertexIndices[0];
			currentFrame.pFaces[j].vertIndex[1] = m_pTriangles[j].vertexIndices[1];
			currentFrame.pFaces[j].vertIndex[2] = m_pTriangles[j].vertexIndices[2];

			// Assign the texture coord indices to our face data
			currentFrame.pFaces[j].coordIndex[0] = m_pTriangles[j].textureIndices[0];
			currentFrame.pFaces[j].coordIndex[1] = m_pTriangles[j].textureIndices[1];
			currentFrame.pFaces[j].coordIndex[2] = m_pTriangles[j].textureIndices[2];
		}

		// Here we add the current object (or frame) to our list object list
		pModel->pObject.push_back(currentFrame);

		//delete [] currentFrame.pTexVerts;
		//delete [] currentFrame.pFaces;
	}
}


///////////////////////////////// CLEAN UP \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function cleans up our allocated memory and closes the file
/////
///////////////////////////////// CLEAN UP \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CLoadMD2::CleanUp()
{
	// This just just the regular cleanup or our md2 model class.  We can free
	// all of this data because we already have it stored in our own structures.

	fclose(m_FilePointer);						// Close the current file pointer

	delete [] ( m_pSkins );						// Free the skins data
	delete [] ( m_pTexCoords );					// Free the texture coord data
	delete [] ( m_pTriangles );					// Free the triangle face data
	delete [] ( m_pFrames );					// Free the frames of animation
}
