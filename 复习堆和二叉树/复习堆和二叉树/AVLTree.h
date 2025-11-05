#pragma once
#include <iostream>
#include <cassert>
using namespace std;

namespace AVL {
	template<class K,class V>
	struct AVLTreeNode {
		pair<K, V> _kv;
		AVLTreeNode<K, V>* _left;
		AVLTreeNode<K, V>* _right;
		AVLTreeNode<K, V>* _parent;
		int _bf;

		// 构造
		AVLTreeNode(const pair<K,V>& kv)
			:_kv(kv)
			,_left(nullptr)
			,_right(nullptr)
			,_parent(nullptr)
			,_bf(0){ }
	};

	template<class K, class V>
	class AVLTree {
	public:
		AVLTree() = default;

		typedef AVLTreeNode<K, V> Node;

		// 查找 BST相同逻辑
		V* find(const K& key) {
			Node* cur = _root;
			while (cur) {
				if (key < cur->_kv.first) cur = cur->_left;
				else if (cur->_kv.first < key) cur = cur->_right;
				else return &cur->_kv.second; // 命中，返回 value 指针
			}
			return nullptr;
		}
		const V* find(const K& key) const {
			Node* cur = _root;
			while (cur) {
				if (key < cur->_kv.first) cur = cur->_left;
				else if (cur->_kv.first < key) cur = cur->_right;
				else return &cur->_kv.second;
			}
			return nullptr;
		}

		// 插入
		bool insert(const pair<K, V>& kv) {
			// 空树 直接插入
			if (_root == nullptr) {
				_root = new Node(kv);
				return true;
			}

			// 非空树
			Node* cur = _root;
			Node* parent = nullptr; // 记录cur的父节点
			// 1. 查找
			while (cur) {
				if (kv.first < cur->_kv.first) {
					parent = cur;
					cur = cur->_left;
				}
				else if (kv.first > cur->_kv.first) {
					parent = cur;
					cur = cur->_right;
				}
				else return false; // 相同值 不插入
			}

			// 2. 插入
			// 此时 cur就是待插入位置 要判断待插入位置在parent的哪一边
			cur = new Node(kv);
			if (cur->_kv.first < parent->_kv.first)
				parent->_left = cur;
			else parent->_right = cur;

			// 3. 调整平衡因子
			// 插入前 parent的平衡因子 只有三种情况 1 -1 0
			// 插入之后可能会变成 1 -1 2 -2 0
			// 1> 1  ：插入之前是0 插入到右边 向上调整
			// 2> -1 ：插入之前是0 插入到左边 向上调整
			// 3> 2  ：插入之前是1 插入到右边 不平衡 旋转
			// 4> -2 : 插入之前是-1 插入到左边 不平衡 旋转
			// 5> 0  : 插入前是-1 插入到右边 插入前是1 插入到左边
			while (parent) {
				if (cur == parent->_right) parent->_bf++;		// 插入到右边 bf++
				else parent->_bf--;								// 插入到左边 bf--

				if (0 == parent->_bf) break;					// bf为0 不用调整
					
				// 向上更新
				if (-1 == parent->_bf || 1 == parent->_bf) {	// parent没有失衡 但会影响parent的父节点
					cur = cur->_parent;
					parent = parent->_parent;
				}
				// 旋转操作
				else if (-2 == parent->_bf || 2 == parent->_bf) {
					if (-2 == parent->_bf && -1 == cur->_bf) {		// 左边高 cur又是-1 插入左边高的左子树 LL 右旋
						rotateRight(parent);
					}
					else if (-2 == parent->_bf && 1 == cur->_bf) {	// 左边高 cur又是1 插入左边高的右子树 LR 先左选后右旋
						rotateLR(parent);
					}
					else if (2 == parent->_bf && -1 == cur->_bf) {	// 右边高 cur又是-1 插入右边高的左子树 RL 先右旋后左旋
						rotateRL(parent);
					}
					else if (2 == parent->_bf && 1 == cur->_bf) {	// 右边高 cur又是1 插入右边高的右子树 RR 左旋
						rotateLeft(parent);
					}
					else assert(false);
				}
				else {	// 异常检查
					assert(false);
				}

			}
			return true;
		}

		// 中序遍历
		void inOrder() { _inOrder(_root); }

		// 判断是否为AVL树
		bool isAVLtree() { _isAVLTree(_root); }

		// 树的最大高度
		int height() { _height(_root); }

	private:
		Node* _root = nullptr;

		// RR 新插入节点在较高右子树的右边 左旋
		void rotateLeft(Node* node) {
		/*
			1. 以child为支撑 左旋node
			2. node成为child的左孩子 child的左孩子childKid成为node的右孩子 

		node -->		 10									 40
						/  \								/  \
					   0   40    <--- child	 左旋	      10   50
						  /  \			    =====>		  /  \   \
		childKid --->	 30  50				             0   30  60
							   \
							   60
		*/
			Node* child = node->_right;				// 失衡节点的右孩子
			Node* childKid = child->_left;			// 失衡节点右孩子的左孩子
			Node* nodeParent = node->_parent;		// 失衡节点的父节点

			// 1. 下沉node 上提child
			child->_left = node;
			node->_parent = child;

			// 2. 如果child有左孩子childKid成为node的右孩子
			node->_right = childKid;
			if (childKid) childKid->_parent = node;

			// 3. 梳理清楚新的关系
			child->_parent = nodeParent;
			if (nodeParent != nullptr) {
				if (node == nodeParent->_right) nodeParent->_right = child;
				else nodeParent->_left = child;
			}
			else { // 说明把根节点转下来了
				_root = child;
			}

			// 4. 更新平衡因子
			child->_bf = node->_bf = 0;
		}

		// LL 新插入节点在较高左子树的左侧 右旋
		void rotateRight(Node* node) {
		/*
			1. 以child为轴 右旋node
			2. node成为child的右孩子 child的右孩子childKid成为node的左孩子
		
		node -->	50                           30
				   /  \           右旋          /  \
		child --> 30  60        =====>        10   50
				 /  \                        /    /  \
				10  40 <-- childKid         0   40   60
			   /
			  0
		*/
			Node* child = node->_left;
			Node* childKid = child->_right;
			Node* nodeParent = node->_parent;

			// 1. 下沉node 上提child
			node->_parent = child;
			child->_right = node;

			// 2. 如果child有右孩子childKid childKid成为node的左孩子
			node->_left = childKid;
			if (childKid) childKid->_parent = node;

			// 3. 梳理清楚新关系
			child->_parent = nodeParent;
			if (nodeParent != nullptr) {
				if (nodeParent->_left == node) nodeParent->_left = child;
				else nodeParent->_right = child;
			}
			else { // 说明把根节点转下来了
				_root = child;
			}

			// 4. 更新平衡因子
			child->_bf = node->_bf = 0;
		}

		// 先左旋后右旋
		void rotateLR(Node* node) {
			Node* child = node->_left;
			Node* childKid = child->_right;
			int bf = childKid->_bf;

			rotateLeft(child);
			rotateRight(node);

			// 调节平衡因子
			if (0 == bf) {
				child->_bf = 0;
				node->_bf = 0;
				childKid->_bf = 0;
			}
			else if (1 == bf) { // 右边插入
				// 这是经过左右双旋之后的平衡因子
				child->_bf = -1;
				node->_bf = 0;
				childKid->_bf = 0;
			}
			else if (-1 == bf) { // 左边插入
				// 这是经过左右双旋之后的平衡因子
				child->_bf = 0;
				node->_bf = 1;
				childKid->_bf = 0;
			}
			else assert(false);
		}


		// 先右旋后左旋
		void rotateRL(Node* node) {
			Node* child = node->_right;
			Node* childKid = child->_left;
			int bf = childKid->_bf;

			rotateRight(child);
			rotateLeft(node);

			// 调整平衡因子
			if (0 == bf) {
				child->_bf = 0;
				node->_bf = 0;
				childKid->_bf = 0;
			}
			else if (1 == bf) { // 右边插入
				child->_bf = 0;
				node->_bf = -1;
				childKid->_bf = 0;
			}
			else if (-1 == bf) { // 左边插入
				child->_bf = 1;
				node->_bf = 0;
				childKid->_bf = 0;
			}
			else assert(false);
		}

		// 辅助中序遍历
		void _inOrder(Node* root) {
			if (nullptr == root) return;

			_inOrder(root->_left);
			cout << root->_kv.first << " : " << root->_kv.second << endl;
			_inOrder(root->_right);
		}

		// 辅助检测是否AVL树
		bool _isAVLTree(Node* root) {
			if (nullptr == root) return true;

			int leftHeight = 0, rightHeight = 0;
			leftHeight = _height(root->_left);
			rightHeight = _height(root->_right);

			int bf = abs(leftHeight - rightHeight);

			return bf <= 1 && _isAVLTree(root->_left) && _isAVLTree(root->_right);
		}

		// 辅助树高
		int _height(Node* root) {
			if (nullptr == root) return 0;

			int leftHeight = 0, rightHeight = 0;
			leftHeight = _height(root->_left);
			rightHeight = _height(root->_right);

			return max(leftHeight, rightHeight) + 1;
		}
	};

	void testAVL() {
		AVLTree<int, int> t;
		// 常规的测试用例
		//int a[] = { 16, 3, 7, 11, 9, 26, 18, 14, 15 };
		// 特殊的带有双旋场景的测试用例
		int a[] = { 4, 2, 6, 1, 3, 5, 15, 7, 16, 14 };
		for (auto e : a)
		{
			t.insert({ e, e });
		}
		t.inOrder();
	}
}