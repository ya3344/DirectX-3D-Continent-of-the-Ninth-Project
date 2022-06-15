#ifndef QUAD_TREE_SRUCT_H__
#define QUAD_TREE_SRUCT_H__
namespace Engine
{
	enum CHILD { CHILD_LT, CHILD_RT, CHILD_RB, CHILD_LB, CHILD_END };

	typedef struct tagQuadTreeIdx
	{
		tagQuadTreeIdx() 
		{
			for (size_t i = 0; i < CHILD_END; i++)
			{
				pQuadTreeIdx[i] = nullptr;
			}
		};
		explicit tagQuadTreeIdx(size_t leftBottom, size_t leftTop, size_t rightBottom, size_t rightTop)
		{
			this->leftBottom = leftBottom;
			this->leftTop = leftTop;
			this->rightBottom = rightBottom;
			this->rightTop = rightTop;
		}
		~tagQuadTreeIdx()
		{
			/*for (size_t i = 0; i < CHILD_END; i++)
			{
				Safe_DoubleDelete(&pQuadTreeIdx[i]);
			}*/
		}

		size_t leftBottom = 0;
		size_t leftTop = 0;
		size_t rightBottom = 0;
		size_t rightTop = 0;

		size_t centerIdx = 0;

		float radius = 0.f;
		float distance = 10000.f;

		tagQuadTreeIdx* pQuadTreeIdx[CHILD_END];

	}QUAD_TREE_IDX;
}
#endif