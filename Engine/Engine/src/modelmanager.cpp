#include "modelmanager.hpp"

template<typename M>
Model* ModelManager<M>::getTriangle(Vec4 color) {
	if (models.find("triangle") == ModelManager::models.end())
		models["triangle"] = new M({ lb,rb,lt }, { 0, 1, 2 }, color);
	return models["triangle"];

}
template<typename M>
Model* ModelManager<M>::getSquare(Vec4 color) {
	if (models.find("square") == models.end())
		models["square"] = new M({ lb, rb, lt, rt }, { 1, 3, 2, 0, 1, 2 }, color);
	return models["square"];
}
template<typename M>
Model* ModelManager<M>::getParallelogram(Vec4 color) {
	if (models.find("parallelogram") == ModelManager::models.end())
		models["parallelogram"] = new M({ lb, rb, Vec4(0.5f, 0.5f, 0.0f, 1.0f), Vec4(1.5f, 0.5f, 0.0f, 1.0f) },
		{ 1, 3, 2, 0, 1, 2 }, color);
	return models["parallelogram"];
}
