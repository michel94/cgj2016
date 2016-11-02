#pragma once
#include "models.hpp"
#include <vector>

class PrettyModel : public Model {
public:
	PrettyModel(vector<Vec4> vertices, vector<GLubyte> indices, Vec4 color);

protected:
	void createData(vector<Vec4>& vertices, Vec4 color) override;
private:
	float randF() {
		return (rand() % 10000 / 10000.0f);
	}

};