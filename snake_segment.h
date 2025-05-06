#ifndef SNAKE_SEGMENT_H
#define SNAKE_SEGMENT_H

#include "model.h"

namespace Models {
	namespace SnakeSegmentInternal {
		extern float vertices[];
		extern float normals[];
		extern float vertexNormals[];
		extern float texCoords[];
		extern float colors[];
		extern unsigned int vertexCount;
	}

	class SnakeSegment : public Model {
	public:
		SnakeSegment();
		virtual ~SnakeSegment();
		virtual void drawSolid(bool smooth = false);
	};

	extern SnakeSegment snakeSegment;
}

#endif
