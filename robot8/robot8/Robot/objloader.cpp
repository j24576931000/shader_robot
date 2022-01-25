//#define STB_IMAGE_IMPLEMENTATION
//#include<stb/stb_image.h>
#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>
using namespace std;
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>

using namespace glm;
#include "objloader.hpp"
//GLuint texture2;
GLuint id;
// Very, VERY simple OBJ loader.
// Here is a short list of features a real function would provide : 
// - Binary files. Reading a model should be just a few memcpy's away, not parsing a file at runtime. In short : OBJ is not very great.
// - Animations & bones (includes bones weights)
// - Multiple UVs
// - All attributes should be optional, not "forced"
// - More stable. Change a line in the OBJ file and it crashes.
// - More secure. Change another line and you can inject code.
// - Loading from memory, stream, etc
void LoadTexture(const char * path)
{
	int image_width ;
    int image_height ;
    int imgcor;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* bytes =stbi_load(path,&image_width,&image_height,&imgcor,0);
    glGenTextures(1, &id);
    glActiveTexture(GL_TEXTURE0);
    //glGenTextures(1, &textureid);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glGenerateMipmap(GL_TEXTURE_2D);
    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width,image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE,bytes);
    stbi_image_free(bytes);
    glBindTexture(GL_TEXTURE_2D, 0);
	
	
}
GLuint loadTexture(GLchar* path, GLboolean alpha)
{
    //Generate texture ID and load texture data 
    GLuint textureID;
    glGenTextures(1, &textureID);
    int width,height;
    int imgcor;
     stbi_set_flip_vertically_on_load(true);
    unsigned char* image =stbi_load(path,&width,&height,&imgcor,0);
    //unsigned char* image = SOIL_load_image(path, &width, &height, 0, alpha ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
    // Assign texture to ID
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width,height, 0, GL_RGBA, GL_UNSIGNED_BYTE,image);
    //glTexImage2D(GL_TEXTURE_2D, 0, alpha ? GL_RGBA : GL_RGB, width, height, 0, alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);	

    // Parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT );	// Use GL_MIRRORED_REPEAT to prevent white borders. Due to interpolation it takes value from next repeat 
    //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(image);
    //SOIL_free_image_data(image);
    return textureID;
}

// Generates a texture that is suited for attachments to a framebuffer
GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil)
{
    // What enum to use?
    GLenum attachment_type;
    if(!depth && !stencil)
        attachment_type = GL_RGB;
    else if(depth && !stencil)
        attachment_type = GL_DEPTH_COMPONENT;
    else if(!depth && stencil)
        attachment_type = GL_STENCIL_INDEX;

    //Generate texture ID and load texture data 
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    if(!depth && !stencil)
        glTexImage2D(GL_TEXTURE_2D, 0, attachment_type, 800, 600, 0, attachment_type, GL_UNSIGNED_BYTE, NULL);
    else // Using both a stencil and depth test, needs special format arguments
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, 800, 600, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    return textureID;
}
static GLuint loadCubeMapTexture(std::vector<const char*> picFilePathVec, 
		GLint internalFormat = GL_RGB, GLenum picFormat = GL_RGB,
		GLenum picDataType = GL_UNSIGNED_BYTE)
	{
		GLuint textId;
		int image_width ;
        int image_height ;
  
        stbi_set_flip_vertically_on_load(true);
        
		glGenTextures(1, &textId);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textId);
		GLubyte *imageData = NULL;
		//int picWidth, picHeight;
		for (std::vector<const char*>::size_type  i =0; i < picFilePathVec.size(); ++i)
		{
			int channels = 0;
			imageData =stbi_load(picFilePathVec[i],&image_width,&image_height,&channels,0);
			//imageData = SOIL_load_image(picFilePathVec[i], &picWidth, 
				//&picHeight, &channels, loadChannels);
			if (imageData == NULL)
			{
				//cout << "Error::loadCubeMapTexture could not load texture file:"
					//<< picFilePathVec[i] << endl;
				return 0;
			}
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, internalFormat, image_width, image_height, 0, picFormat, picDataType, imageData);
			stbi_image_free(imageData);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		return textId;
	}


bool loadOBJ(
	const char * path, 
	std::vector<vec3> & out_vertices, 
	std::vector<vec2> & out_uvs,
	std::vector<vec3> & out_normals,
	std::vector<unsigned int> &out_materialIndices,
	std::vector<string> &out_mtls
)
{
	printf("Loading OBJ file %s...\n", path);

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices,materialIndices;
	std::vector<vec3> temp_vertices; 
	std::vector<vec2> temp_uvs;
	std::vector<vec3> temp_normals;
	std::vector<string> temp_mtls;

	FILE * file = fopen(path, "r");
	if( file == NULL ){
		printf("Impossible to open the .obj file ! Are you in the right path ? See Tutorial 1 for details\n");
		return false;
	}
	int fcount = 0;//face count for material
	while( 1 ){

		char lineHeader[128];
		
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader
	
		if ( strcmp( lineHeader, "v" ) == 0 ){//'v':�O�I
			vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			temp_vertices.push_back(vertex);
		
		}else if ( strcmp( lineHeader, "vt" ) == 0 ){
			vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y );
			// Invert V coordinate since we will only use DDS texture,
			//which are inverted.Remove if you want to use TGA or BMP loaders.
			//uv.y = -uv.y; 
			temp_uvs.push_back(uv);
		}else if ( strcmp( lineHeader, "vn" ) == 0 ){
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			temp_normals.push_back(normal);
		}else if ( strcmp( lineHeader, "f" ) == 0 ){
		//	std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0],
																	   &vertexIndex[1], &uvIndex[1], &normalIndex[1],
																	   &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			if (matches != 9){
				printf("File can't be read by our simple parser :-( Try exporting with other options\n");
				return false;
			}
			fcount++;
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices    .push_back(uvIndex[0]);
			uvIndices    .push_back(uvIndex[1]);
			uvIndices    .push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}else if(strcmp( lineHeader , "usemtl") == 0){
			out_materialIndices.push_back(fcount);//last material used face counts	
			char material[50];
			fscanf(file, "%s", material);
			out_mtls.push_back(material);
			fcount = 0;//face count for next material
		}else{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}
	}
	out_materialIndices.push_back(fcount);//at the end , push the last faces
	// For each vertex of each triangle
	for( unsigned int i=0; i<vertexIndices.size(); i++ ){
		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];
		
		// Get the attributes thanks to the index
		vec3 vertex = temp_vertices[ vertexIndex-1 ];
		vec2 uv = temp_uvs[ uvIndex-1 ];
		vec3 normal = temp_normals[ normalIndex-1 ];
		
		// Put the attributes in buffers
		out_vertices.push_back(vertex);
		out_uvs     .push_back(uv);
		out_normals .push_back(normal);
	}

	return true;
}

bool loadMTL(const char * path,
			 std::vector<vec3> &Kd,
			 std::vector<vec3> &Ka,
			 std::vector<vec3> &Ks,
			 std::vector<std::string> &out_name,
			 std::string &texture
			 )
{
			 FILE * file = fopen(path, "r");
			 if( file == NULL ){
				printf("Impossible to open the .mtl file ! Are you in the right path ? See Tutorial 1 for details\n");
				return false;
			 }
			 while( 1 ){
				 char lineHeader[128];
				 // read the first word of the line
				 int res = fscanf(file, "%s", lineHeader);
				 if (res == EOF)
					 break; // EOF = End Of File. Quit the loop.

				 // else : parse lineHeader
				 if(strcmp(lineHeader , "newmtl")==0){
					 char material[50];
					 fscanf(file, "%s", material);
					 out_name.push_back(material);
				 }else if(strcmp(lineHeader ,"Kd")==0){
					 vec3 diffuse;
					 fscanf(file, "%f %f %f\n",&diffuse.x,&diffuse.y,&diffuse.z);
					 Kd.push_back(diffuse);
				 }else if(strcmp(lineHeader ,"Ka")==0){
					 vec3 ambient;
					 fscanf(file, "%f %f %f\n",&ambient.x,&ambient.y,&ambient.z);
					 Ka.push_back(ambient);
				 }else if(strcmp(lineHeader ,"Ks")==0){
					 vec3 specular;
					 fscanf(file, "%f %f %f\n",&specular.x,&specular.y,&specular.z);
					 Ks.push_back(specular);
				 }else if(strcmp(lineHeader ,"map_Kd")==0){
					 fscanf(file, "%s\n",(char*)texture.c_str());
					 //printf("%s\n",texture.c_str());
					 //LoadTexture(texture.c_str());
					 //texture.push_back(texture);
				 }else{
					 // Probably a comment, eat up the rest of the line
					 char stupidBuffer[1000];
					 fgets(stupidBuffer, 1000, file);
				 }
			 }

			 return true;
}





#ifdef USE_ASSIMP // don't use this #define, it's only for me (it AssImp fails to compile on your machine, at least all the other tutorials still work)

// Include AssImp
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

bool loadAssImp(
	const char * path, 
	std::vector<unsigned short> & indices,
	std::vector<glm::vec3> & vertices,
	std::vector<glm::vec2> & uvs,
	std::vector<glm::vec3> & normals
){

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path, 0/*aiProcess_JoinIdenticalVertices | aiProcess_SortByPType*/);
	if( !scene) {
		fprintf( stderr, importer.GetErrorString());
		return false;
	}
	const aiMesh* mesh = scene->mMeshes[0]; // In this simple example code we always use the 1rst mesh (in OBJ files there is often only one anyway)

	// Fill vertices positions
	vertices.reserve(mesh->mNumVertices);
	for(unsigned int i=0; i<mesh->mNumVertices; i++){
		aiVector3D pos = mesh->mVertices[i];
		vertices.push_back(glm::vec3(pos.x, pos.y, pos.z));
	}

	// Fill vertices texture coordinates
	uvs.reserve(mesh->mNumVertices);
	for(unsigned int i=0; i<mesh->mNumVertices; i++){
		aiVector3D UVW = mesh->mTextureCoords[0][i]; // Assume only 1 set of UV coords; AssImp supports 8 UV sets.
		uvs.push_back(glm::vec2(UVW.x, UVW.y));
	}

	// Fill vertices normals
	normals.reserve(mesh->mNumVertices);
	for(unsigned int i=0; i<mesh->mNumVertices; i++){
		aiVector3D n = mesh->mNormals[i];
		normals.push_back(glm::vec3(n.x, n.y, n.z));
	}


	// Fill face indices
	indices.reserve(3*mesh->mNumFaces);
	for (unsigned int i=0; i<mesh->mNumFaces; i++){
		// Assume the model has only triangles.
		indices.push_back(mesh->mFaces[i].mIndices[0]);
		indices.push_back(mesh->mFaces[i].mIndices[1]);
		indices.push_back(mesh->mFaces[i].mIndices[2]);
	}
	
	// The "scene" pointer will be deleted automatically by "importer"

}

#endif