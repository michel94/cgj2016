#include "modelmanager.hpp"

map<string, Model*> ModelManager<>::models;

template<typename M>
Model* ModelManager<M>::getTriangle(Vec4 color) {
	if (ModelManager<>::models.find("triangle") == ModelManager::models.end())
		ModelManager<>::models["triangle"] = new M({ lb,rb,lt }, { 0, 1, 2 }, color);
	return ModelManager<>::models["triangle"];

}
template<typename M>
Model* ModelManager<M>::getSquare(Vec4 color) {
	if (ModelManager<>::models.find("square") == ModelManager<>::models.end())
		ModelManager<>::models["square"] = new M({ lb, rb, lt, rt }, { 1, 3, 2, 0, 1, 2 }, color);
	return ModelManager<>::models["square"];
}
template<typename M>
Model* ModelManager<M>::getParallelogram(Vec4 color) {
	if (ModelManager<>::models.find("parallelogram") == ModelManager::models.end())
		ModelManager<>::models["parallelogram"] = new M({ lb, rb, Vec4(0.5f, 0.5f, 0.0f, 1.0f), Vec4(1.5f, 0.5f, 0.0f, 1.0f) },
		{ 1, 3, 2, 0, 1, 2 }, color);
	return ModelManager<>::models["parallelogram"];
}
