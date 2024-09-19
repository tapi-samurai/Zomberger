#pragma once
#include <list>
using std::list;

class Node
{
private:
	Node* m_parent;					// 親のポインタ
	list<Node*> m_children;			// 子のポインタリスト
	list<Node*> m_removeChildren;	// 削除する子のポインタリスト
	bool m_isLoaded;				// Load関数を実行したかどうか
	bool m_isDestroyed;				// Destroy関数を実行したかどうか
	
protected:
	// リソースの読み込み
	virtual void Load(){}

	// リソースの開放
	virtual void Release(){}

	// 更新
	virtual void Update(){}

	// 描画
	virtual void Draw(){}

	// 自身の削除
	void Destroy();
	
	// 親の取得
	Node* GetParent(){ return m_parent; }

public:
	// コンストラクタ
	Node() :
		m_parent(nullptr),
		m_isLoaded(false),
		m_isDestroyed(false) {}

	// デストラクタ
	virtual ~Node();	// 基底クラスのデストラクタはvirtual付ける

	// 子ノードを含むリソースの読み込み
	virtual void TreeLoad();

	// 子ノードを含むリソースの解放
	virtual void TreeRelease();

	// 子ノードを含む更新
	virtual void TreeUpdate();

	// 子ノードを含む描画
	virtual void TreeDraw();

	// 子ノードの追加
	virtual void AddChild(Node* node);

	// 子ノードの削除
	void RemoveChild(Node* node);

	// 子ノードの数を取得
	const size_t GetChildCount() const { return m_children.size(); }
};
