#include "MeshData.h"

#include <algorithm>
#include <string>

#include <core/config/ConEntry.h>
#include <core/math/Matrix.h>

using namespace chira;

MeshData::~MeshData() {
	if (this->initialized) {
		Renderer::destroyMesh(this->handle);
	}
}

void MeshData::render(glm::mat4 model, MeshCullType cullType) {
	if (!this->initialized)
		this->setupForRendering();
	if (this->material) {
		this->material->use();
		if (this->material->getShader()->usesModelMatrix())
			this->material->getShader()->setUniform("m", model);
	}
	Renderer::drawMesh(this->handle, this->depthFunction, cullType);
}

SharedPointer<IMaterial> MeshData::getMaterial() const {
    return this->material;
}

void MeshData::setMaterial(SharedPointer<IMaterial> newMaterial) {
    this->material = std::move(newMaterial);
}

MeshDepthFunction MeshData::getDepthFunction() const {
    return this->depthFunction;
}

void MeshData::setDepthFunction(MeshDepthFunction function) {
    this->depthFunction = function;
}

std::vector<std::byte> MeshData::getMeshData(const std::string& meshLoader) const {
    return IMeshLoader::getMeshLoader(meshLoader)->createMesh(this->vertices, this->indices);
}

void MeshData::appendMeshData(const std::string& loader, const std::string& identifier) {
    IMeshLoader::getMeshLoader(loader)->loadMesh(identifier, this->vertices, this->indices);
}

void MeshData::setupForRendering() {
	this->handle = Renderer::createMesh(this->vertices, this->indices, MeshDrawMode::STATIC);
	this->initialized = true;
}

void MeshData::updateMeshData() {
	if (!this->initialized)
		return;
	Renderer::updateMesh(&this->handle, this->vertices, this->indices, this->drawMode);
}

void MeshData::clearMeshData() {
    this->vertices.clear();
    this->indices.clear();
}
