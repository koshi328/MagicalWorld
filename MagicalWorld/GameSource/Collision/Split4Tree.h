/**
* File: Split4.h
*
* ��ԕ���
*
* Date: 2016.11.10
* Author: koshi ito
*/
#include <windows.h>
#include <list>
#include "Collision.h"

// ��s�錾
class CollisionNode;
class Cell;

// ��Ԃɓo�^�����I�u�W�F�N�g
class Tree
{
private:
	CollisionNode* owner_;	// �o�^����������
	Cell* pCell_;			// �o�^����Ă�����
	Tree* pPrev_;			// ������Ԃɂ��鎩�g���O�̃I�u�W�F�N�g
	Tree* pNext_;			// ������Ԃɂ��鎩�g�����̃I�u�W�F�N�g
public:
	// �R���X�g���N�^
	Tree() = default;
	// �f�X�g���N�^
	~Tree() = default;

	// ��Ԃɓo�^
	void RegistCell(Cell* cell)
	{
		pCell_ = cell;
	}
	// �A�N�Z�b�T
	Cell* getCell() { return pCell_; }
	Tree* getPrev() { return pPrev_; }
	Tree* getNext() { return pNext_; }
	void setPrev(Tree* p) { pPrev_ = p; }
	void setNext(Tree* p) { pNext_ = p; }

	// ���L�҂̓o�^
	CollisionNode* getOwner() { return owner_; }
	void setOwner(CollisionNode* p) { owner_ = p; }
};

// ����������Ԃ��\������I�u�W�F�N�g
class Cell
{
private:
	Tree* begin_;	// ���g�ɓo�^����Ă���I�u�W�F�N�g�̐擪�̗v�f
public:
	// �R���X�g���N�^
	Cell() = default;
	// �f�X�g���N�^
	~Cell()
	{
		begin_ = nullptr;
	}

	// �v�f�̒ǉ�
	bool Push(Tree* tree)
	{
		if (tree == nullptr) return false;		// �����ȃI�u�W�F�N�g
		if (tree->getCell() == this) return false;	// ��d�o�^���Ȃ�
		if (begin_ == nullptr)
		{
			begin_ = tree; // �擪�������Ă��Ȃ�������o�^
		}
		else
		{
			// �擪�����ւ���
			tree->setNext(begin_);
			begin_->setPrev(tree);
			begin_ = tree;
		}
		tree->RegistCell(this);	// ��Ԃ�o�^
		return true;
	}
	// �擪�v�f�̎擾
	Tree* getBegin() { return begin_; }
};

// �ő啪����
#define MAX_LEVEL 9
// ����������Ԃ���������N���X
class Split4Tree
{
public:
	float x_;			// �ŏ��lX
	float y_;			// �ŏ��lY
	float w_;			// X����
	float h_;			// Y����
	float unit_w_;		// �ŏ��P�ʃT�C�YX
	float unit_h_;		// �ŏ��P�ʃT�C�YY
	unsigned int level_;// �������x��
	Cell** cellAry_;	// ���
	unsigned int hierarchy_[MAX_LEVEL + 1]; // �����K�w���̋�Ԑ�
	unsigned int cellNum_;				   // ��Ԑ�
public:
	// �R���X�g���N�^
	Split4Tree()
	{
		x_ = 0;
		y_ = 0;
		w_ = 0;
		h_ = 0;
		unit_w_ = 0;
		unit_h_ = 0;
		level_ = 0;
		cellAry_ = nullptr;
		cellNum_ = 0;
	}
	// �f�X�g���N�^
	~Split4Tree()
	{
		// �S�v�f�폜
		unsigned int i;
		for (i = 0; i < cellNum_; i++)
		{
			// �e�v�f�̏���
			if (cellAry_[i] != nullptr)
				delete cellAry_[i];
		}
		// �{�̂̏���
		delete[] cellAry_;
	}

	// ����������
	void Initialize(unsigned int level, float x, float y, float w, float h)
	{
		level_ = level;
		x_ = x;
		y_ = y;
		w_ = w - x;
		h_ = h - y;
		unit_w_ = w_ / (1 << level);
		unit_h_ = h_ / (1 << level);

		int i;
		// �K�w���̋�Ԑ������߂�
		hierarchy_[0] = 1;
		for (i = 1; i < MAX_LEVEL; i++)
		{
			hierarchy_[i] = hierarchy_[i - 1] * 4;
		}
		// �g�p����K�w���ׂĂ̋�Ԃ����킹���������߂�
		cellNum_ = (hierarchy_[level + 1] - 1) / 3;

		cellAry_ = new Cell*[cellNum_];
		ZeroMemory(cellAry_, sizeof(Cell*) * cellNum_);
	}

	// ���W�����Ԕԍ����Z�o
	DWORD GetMortonNumber(float left, float top, float right, float bottom)
	{
		// �ŏ����x���ɂ�����e���ʒu���Z�o
		DWORD LT = GetPointElem(left, top);
		DWORD RB = GetPointElem(right, bottom);

		// ��Ԕԍ��������Z����
		// �ŏ�ʋ�؂肩�珊�����x�����Z�o
		DWORD Def = RB - LT;
		unsigned int HiLevel = 0;
		unsigned int i;
		for (i = 0; i<level_; i++)
		{
			DWORD Check = (Def >> (i * 2)) & 0x3;
			if (Check != 0)
				HiLevel = i + 1;
		}
		DWORD SpaceNum = RB >> (HiLevel * 2);
		DWORD AddNum = (hierarchy_[level_ - HiLevel] - 1) / 3;
		SpaceNum += AddNum;

		if (SpaceNum > cellNum_)
			return 0xffffffff;

		return SpaceNum;
	}

	// �r�b�g�����֐�
	DWORD BitSeparate32(DWORD n)
	{
		n = (n | (n << 8)) & 0x00ff00ff;
		n = (n | (n << 4)) & 0x0f0f0f0f;
		n = (n | (n << 2)) & 0x33333333;
		return (n | (n << 1)) & 0x55555555;
	}

	// 2D���[�g����Ԕԍ��Z�o�֐�
	WORD Get2DMortonNumber(WORD x, WORD y)
	{
		return (WORD)(BitSeparate32(x) | (BitSeparate32(y) << 1));
	}

	// ���W�����`4���ؗv�f�ԍ��ϊ��֐�
	DWORD GetPointElem(float pos_x, float pos_y)
	{
		return Get2DMortonNumber((WORD)((pos_x - x_) / unit_w_), (WORD)((pos_y - y_) / unit_h_));
	}

	// ��Ԃ𐶐�
	bool CreateNewCell(DWORD Elem)
	{
		// �����̗v�f�ԍ�
		while (!cellAry_[Elem])
		{
			// �w��̗v�f�ԍ��ɋ�Ԃ�V�K�쐬
			cellAry_[Elem] = new Cell;

			// �e��ԂɃW�����v
			Elem = (Elem - 1) >> 2;
			if (Elem >= cellNum_) break;
		}
		return true;
	}

	// ��Ԃւ̓o�^
	void Entry(Tree* tree, float minX, float minY, float maxX, float maxY)
	{
		DWORD elem = GetMortonNumber(minX, minY, maxX, maxY);
		if (elem > cellNum_) return;
		if (!cellAry_[elem])
			CreateNewCell(elem);
		cellAry_[elem]->Push(tree);
	}
	
	// ��Ԗ��ɕK�v�ȏՓ˂𔻒肷��
	void Collision()
	{
		std::list<Tree*> list;

		if (!cellAry_[0]) return;

		Seach(0, list);
	}

	void Seach(DWORD elem, std::list<Tree*> list)
	{
		// �ォ��󂯎����Node�Ɣ��肷��
		std::list<Tree*>::iterator it = list.begin();
		for (; it != list.end(); it++)
		{
			Tree* t = cellAry_[elem]->getBegin();
			while (t)
			{
				(*it)->getOwner()->Collision(t->getOwner());
				t = t->getNext();
			}
		}
		// ����ԓ��ł̔���
		Tree* t = cellAry_[elem]->getBegin();
		while (t)
		{
			Tree* t2 = t->getNext();
			while (t2)
			{
				t->getOwner()->Collision(t2->getOwner());
				t2 = t2->getNext();
			}
			// �q��Ԃɑ���I�u�W�F�N�g
			list.push_back(t);
			// �X�V
			t = t->getNext();
		}

		// �q��Ԃ̒T��
		int i;
		for (i = 0; i < 4; i++)
		{
			DWORD tmp = elem * 4 + i + 1;
			if (tmp >= cellNum_) return;
			if (cellAry_[tmp])
				Seach(tmp, list);
		}
	}
};