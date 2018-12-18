#include "model.h"

#include <glm/glm.hpp>

#include "log.h"

bgfx::VertexDecl Vertex::ms_decl;

Model::Model(const char* path)
{
	// read file via ASSIMP
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenNormals);
	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		IG_CORE_ERROR("ASSIMP: {}", importer.GetErrorString());
		return;
	}

	// process ASSIMP's root node recursively
	processNode(scene->mRootNode, scene);
}

// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
void Model::processNode(aiNode *node, const aiScene *scene)
{
	// process each mesh located at the current node
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		// the node object only contains indices to index the actual objects in the scene. 
		// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}

}

Mesh* Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
	// data to fill
	std::vector<Vertex> vertices;
	std::vector<uint16_t> indices;

	// Walk through each of the mesh's vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex = { 0.0f };
		vertex.pos_x = mesh->mVertices[i].x;
		vertex.pos_y = mesh->mVertices[i].y;
		vertex.pos_z = mesh->mVertices[i].z;
		// normals
		if (mesh->mNormals) // does the mesh contain normals?
		{
			vertex.norm_x = mesh->mNormals[i].x;
			vertex.norm_y = mesh->mNormals[i].y;
			vertex.norm_z = mesh->mNormals[i].z;
		}
		// texture coordinates
		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			vertex.tex_x = mesh->mTextureCoords[0][i].x;
			vertex.tex_y = mesh->mTextureCoords[0][i].y;
		}
		// tangent
		if (mesh->mTangents) // does the mesh contain tangents?
		{
			vertex.tan_x = mesh->mTangents[i].x;
			vertex.tan_y = mesh->mTangents[i].y;
			vertex.tan_z = mesh->mTangents[i].z;
		}
		// bitangent
		if (mesh->mBitangents) // does the mesh contain bitangents?
		{
			vertex.bi_x = mesh->mBitangents[i].x;
			vertex.bi_y = mesh->mBitangents[i].y;
			vertex.bi_z = mesh->mBitangents[i].z;
		}

		vertices.push_back(vertex);
	}
	// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(uint16_t(face.mIndices[j]));
	}

	// return a mesh object created from the extracted mesh data
	return new Mesh(vertices, indices);
}

Model::~Model()
{
	for (Mesh* mesh : meshes)
	{
		delete mesh;
	}
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint16_t> indices)
{
	this->vertices = vertices;
	this->indices = indices;

	vbh = bgfx::createVertexBuffer(bgfx::makeRef(&this->vertices[0], this->vertices.size() * sizeof(Vertex)), Vertex::ms_decl);
	ibh = bgfx::createIndexBuffer(bgfx::makeRef(&this->indices[0], this->indices.size() * sizeof(uint16_t)));
}

Mesh::~Mesh()
{
	bgfx::destroy(vbh);
	bgfx::destroy(ibh);
}