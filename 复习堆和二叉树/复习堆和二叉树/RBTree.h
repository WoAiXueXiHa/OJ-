#pragma once
#include <iostream>
#include <cassert>
#include <vector>
using namespace std;

namespace RBT {
	enum Color { RED, BLACK };
	template <class K, class V>
	struct RBTreeNode {
		pair<K, V> _kv;
		RBTreeNode<K, V>* _left;
		RBTreeNode<K, V>* _right;
		RBTreeNode<K, V>* _parent;
		Color _col;

		// 构造 默认红色
		RBTreeNode(const pair<K, V>& kv, Color col = RED)
			:_kv(kv)
			, _left(nullptr)
			, _right(nullptr)
			, _parent(nullptr)
			, _col(col)
		{}
	};

	template <class K, class V>
	class RBTree {
	public:
		typedef RBTreeNode<K, V> Node;
		RBTree() : _root(nullptr) {}
		// 拷贝构造 深拷贝
		RBTree(const RBTree& other){}
		
		// 查找
		bool find(const pair<K, V>& kv) {
			if (nullptr == _root) return  false;
			// 二分逻辑
			Node* cur = _root;

			while (cur) {
				if (kv.first < cur->_kv.first) cur = cur->_left;
				else if (kv.first > cur->_kv.first) cur = cur->_right;
				else return true;		// 命中
			}

			return false;
		}

		// 插入 红色节点
		bool insert(const pair<K, V>& kv) {
			if (nullptr == _root) {
				_root = new Node(kv);
				_root->_col = BLACK;
				return true;
			}

			// 先查找合适插入位置
			Node* cur = _root;
			Node* parent = nullptr;
			while (cur) {
				if (kv.first < cur->_kv.first) {
					parent = cur;
					cur = cur->_left;
				}
				else if (kv.first > cur->_kv.first) {
					parent = cur;
					cur = cur->_right;
				}
				else return false;		// 找到相同的值 不执行插入操作
			}

			// cur的位置就是待插入位置
			cur = new Node(kv);
			cur->_col = RED;

			if (parent->_kv.first > kv.first) parent->_left = cur;
			else parent->_right = cur;
			cur->_parent = parent;

			// 检测红黑树结构是否被破坏
			// 如果parent是黑色节点就没有被破坏 无需调整
			// 父亲存在是前提 并且为红色 那么不存在两个连续红色节点的性质被破坏
			// 存在父节点 那么说明一定存在爷节点 因为父节点此时为红 不可能是根节点！
			while (parent && parent->_col == RED) {
				Node* grandfather = parent->_parent;	// 保存爷节点
				Node* uncle = nullptr;
				if (grandfather) {
					uncle = (parent == grandfather->_left) ? grandfather->_right : grandfather->_left;
				}
				// uncle在右
				if (parent == grandfather->_left) {
					// uncle存在且为红色
					// 父 叔变黑 爷变红 cur指向爷 继续更新
					if (uncle && uncle->_col == RED) {
						parent->_col = uncle->_col = BLACK;
						grandfather->_col = RED;

						// 继续更新
						cur = grandfather;
						parent = grandfather->_parent;
					}
					else {
						// uncle不存在 或为黑色
						if (cur == parent->_left) {
							/*
									g黑						  f黑
								  f红  u黑   左边高 右旋     c黑  g红
								c红								   u黑	
							*/

							rotateRight(grandfather);
							parent->_col = BLACK;
							grandfather->_col = RED;
							cur->_col = BLACK;
						}
						else {
							/*
									g黑					  g黑					  c黑
								  f红  u黑   先左旋f	   c红  u黑  后右旋g        f黑   g红 
								    c红				 f红								   u黑
								始终保持每条路径的黑色数量不变

							*/
							rotateLeft(parent);
							rotateRight(grandfather);

							parent->_col = BLACK;
							grandfather->_col = RED;
							cur->_col = BLACK;
						}
						break;		// 旋转后就调整好了 直接退出即可
					}
				}
				else {
				 // uncle在左
				 // 
					// uncle存在且为红色
					// 父 叔变黑 爷变红 cur指向爷 继续更新
					if (uncle && uncle->_col == RED) {
						parent->_col = uncle->_col = BLACK;
						grandfather->_col = RED;

						// 继续更新
						cur = grandfather;
						parent = grandfather->_parent;
					}
					else {
						// uncle不存在或为黑色
						if (cur == parent->_right) {
							/*
									g黑									f黑
								 u黑   f红      右边高 左旋			  g红 c黑
								         c红						    u黑
							*/
							rotateLeft(grandfather);

							grandfather->_col = RED;
							parent->_col = BLACK;
							cur->_col = BLACK;
						}
						else {
							/*
									g黑					g黑						c黑
								 u黑   f红   先右旋f	  u黑  c红     后左旋g     g红 f黑
									c红						 f红				u黑
							*/
							rotateRight(parent);
							rotateLeft(grandfather);

							grandfather->_col = RED;
							parent->_col = BLACK;
							cur->_col = BLACK;
						}
						break;	//只要旋转了后续就调整完成
					}
				}
			}
			_root->_col = BLACK;   //最后根节点强制涂黑即可
			return true;
		}


		void inOrder() {
			_inOrder(_root);
			cout << endl;
		}

		int height() { return _height(_root); }

	private:
		Node* _root;

		int _height(Node* root) {
			if (root == nullptr) return 0;

			int _leftHeight = 0, _rightHeight = 0;
			_leftHeight = _height(root->_left);
			_rightHeight = _height(root->_right);

			return _leftHeight > _rightHeight ? _leftHeight + 1 : _rightHeight + 1;
		}

		void _inOrder(Node* root) {
			if (root == nullptr) return;

			_inOrder(root->_left);
			cout << root->_kv.first << ":" << root->_kv.second << "   ";
			_inOrder(root->_right);
		}

		// 左旋
		void rotateLeft(Node* node) {

			// RR 左旋 右边高
			/*
					&            parent
					  &			 node
					    &	     child
		      childKid^   *      newNode
			*/
			Node* parent = node->_parent;
			Node* child = node->_right;
			Node* childKid = child->_left;

			// 下沉node 上浮child
			child->_left = node;
			node->_parent = child;
			node->_right = childKid;
			if (childKid) childKid->_parent = node;

			// 梳理清爷 父 孙关系
			child->_parent = parent;
			if (parent != nullptr) { // parent不是空节点
				if (parent->_left == node) parent->_left = child;
				else parent->_right = child;
			}
			else { // parent是空 那么child成为根节点
				_root = child;
			}
			

		}

		// 右旋 
		void rotateRight(Node* node) {
			Node* child = node->_left;
			Node* parent = node->_parent;
			Node* childKid = child->_right;
			// LL 右旋 左边高
			/*
					&            parent
				  &			 node
				&	     child
	newNode	  *  ^childKid
			*/

			// 下沉node 上浮child
			node->_parent = child;
			child->_right = node;
			node->_left = childKid;
			if (childKid) childKid->_parent = node;

			// 梳理清新的关系
			child->_parent = parent;
			if (parent != nullptr) {
				if (parent->_left == node) parent->_left = child;
				else parent->_right = child;
			}
			else {
				_root = child;
			}
		}
	};

	void testRBT()
	{
		RBTree<int, int> t;
		// 常规的测试用例
		int b[] = { 16, 3, 7, 11, 9, 26, 18, 14, 15 };
		// 特殊的带有双旋场景的测试用例
		int a[] = { 4, 2, 6, 1, 3, 5, 15, 7, 16, 14 };

		for (auto e : b)
		{
			t.insert({ e, e });
		}

		t.inOrder();
	}

	
}