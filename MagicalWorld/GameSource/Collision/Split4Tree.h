/**
* File: Split4.h
*
* 空間分割
*
* Date: 2016.11.10
* Author: koshi ito
*/
#include <windows.h>
#include <list>
#include "Collision.h"

// 先行宣言
class CollisionNode;
class Cell;

// 空間に登録されるオブジェクト
class Tree
{
private:
	CollisionNode* owner_;	// 登録したい判定
	Cell* pCell_;			// 登録されている空間
	Tree* pPrev_;			// 同じ空間にある自身より前のオブジェクト
	Tree* pNext_;			// 同じ空間にある自身より後ろのオブジェクト
public:
	// コンストラクタ
	Tree() = default;
	// デストラクタ
	~Tree() = default;

	// 空間に登録
	void RegistCell(Cell* cell)
	{
		pCell_ = cell;
	}
	// アクセッサ
	Cell* getCell() { return pCell_; }
	Tree* getPrev() { return pPrev_; }
	Tree* getNext() { return pNext_; }
	void setPrev(Tree* p) { pPrev_ = p; }
	void setNext(Tree* p) { pNext_ = p; }

	// 所有者の登録
	CollisionNode* getOwner() { return owner_; }
	void setOwner(CollisionNode* p) { owner_ = p; }
};

// 分割した空間を構成するオブジェクト
class Cell
{
private:
	Tree* begin_;	// 自身に登録されているオブジェクトの先頭の要素
public:
	// コンストラクタ
	Cell() = default;
	// デストラクタ
	~Cell()
	{
		begin_ = nullptr;
	}

	// 要素の追加
	bool Push(Tree* tree)
	{
		if (tree == nullptr) return false;		// 無効なオブジェクト
		if (tree->getCell() == this) return false;	// 二重登録しない
		if (begin_ == nullptr)
		{
			begin_ = tree; // 先頭が入っていなかったら登録
		}
		else
		{
			// 先頭を入れ替える
			tree->setNext(begin_);
			begin_->setPrev(tree);
			begin_ = tree;
		}
		tree->RegistCell(this);	// 空間を登録
		return true;
	}
	// 先頭要素の取得
	Tree* getBegin() { return begin_; }
};

// 最大分割数
#define MAX_LEVEL 9
// 分割した空間を所持するクラス
class Split4Tree
{
public:
	float x_;			// 最小値X
	float y_;			// 最小値Y
	float w_;			// X軸幅
	float h_;			// Y軸幅
	float unit_w_;		// 最小単位サイズX
	float unit_h_;		// 最小単位サイズY
	unsigned int level_;// 分割レベル
	Cell** cellAry_;	// 空間
	unsigned int hierarchy_[MAX_LEVEL + 1]; // 分割階層毎の空間数
	unsigned int cellNum_;				   // 空間数
public:
	// コンストラクタ
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
	// デストラクタ
	~Split4Tree()
	{
		// 全要素削除
		unsigned int i;
		for (i = 0; i < cellNum_; i++)
		{
			// 各要素の消去
			if (cellAry_[i] != nullptr)
				delete cellAry_[i];
		}
		// 本体の消去
		delete[] cellAry_;
	}

	// 初期化処理
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
		// 階層毎の空間数を求める
		hierarchy_[0] = 1;
		for (i = 1; i < MAX_LEVEL; i++)
		{
			hierarchy_[i] = hierarchy_[i - 1] * 4;
		}
		// 使用する階層すべての空間を合わせた数を求める
		cellNum_ = (hierarchy_[level + 1] - 1) / 3;

		cellAry_ = new Cell*[cellNum_];
		ZeroMemory(cellAry_, sizeof(Cell*) * cellNum_);
	}

	// 座標から空間番号を算出
	DWORD GetMortonNumber(float left, float top, float right, float bottom)
	{
		// 最小レベルにおける各軸位置を算出
		DWORD LT = GetPointElem(left, top);
		DWORD RB = GetPointElem(right, bottom);

		// 空間番号を引き算して
		// 最上位区切りから所属レベルを算出
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

	// ビット分割関数
	DWORD BitSeparate32(DWORD n)
	{
		n = (n | (n << 8)) & 0x00ff00ff;
		n = (n | (n << 4)) & 0x0f0f0f0f;
		n = (n | (n << 2)) & 0x33333333;
		return (n | (n << 1)) & 0x55555555;
	}

	// 2Dモートン空間番号算出関数
	WORD Get2DMortonNumber(WORD x, WORD y)
	{
		return (WORD)(BitSeparate32(x) | (BitSeparate32(y) << 1));
	}

	// 座標→線形4分木要素番号変換関数
	DWORD GetPointElem(float pos_x, float pos_y)
	{
		return Get2DMortonNumber((WORD)((pos_x - x_) / unit_w_), (WORD)((pos_y - y_) / unit_h_));
	}

	// 空間を生成
	bool CreateNewCell(DWORD Elem)
	{
		// 引数の要素番号
		while (!cellAry_[Elem])
		{
			// 指定の要素番号に空間を新規作成
			cellAry_[Elem] = new Cell;

			// 親空間にジャンプ
			Elem = (Elem - 1) >> 2;
			if (Elem >= cellNum_) break;
		}
		return true;
	}

	// 空間への登録
	void Entry(Tree* tree, float minX, float minY, float maxX, float maxY)
	{
		DWORD elem = GetMortonNumber(minX, minY, maxX, maxY);
		if (elem > cellNum_) return;
		if (!cellAry_[elem])
			CreateNewCell(elem);
		cellAry_[elem]->Push(tree);
	}
	
	// 空間毎に必要な衝突を判定する
	void Collision()
	{
		std::list<Tree*> list;

		if (!cellAry_[0]) return;

		Seach(0, list);
	}

	void Seach(DWORD elem, std::list<Tree*> list)
	{
		// 上から受け取ったNodeと判定する
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
		// 同空間内での判定
		Tree* t = cellAry_[elem]->getBegin();
		while (t)
		{
			Tree* t2 = t->getNext();
			while (t2)
			{
				t->getOwner()->Collision(t2->getOwner());
				t2 = t2->getNext();
			}
			// 子空間に送るオブジェクト
			list.push_back(t);
			// 更新
			t = t->getNext();
		}

		// 子空間の探索
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