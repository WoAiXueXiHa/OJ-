#pragma once
#include <iostream>
#include<vector>
#include<cassert>
#include <random>
#include <algorithm>
#include <chrono>
using namespace std;

// 优先级队列的底层是堆
// 这里用vector实现堆
// 默认大顶堆
namespace HEAP {
	template<class T>
	class heap {
	public:
		// 默认构造
		heap() = default;

		// 建堆
		// 1. 用初始化列表建堆


		// 2. 用区间建堆

		// 建堆接口
		// 1. 利用push建堆 向上调整时间复杂度是叶子的高度O(logn)  push n个元素 O(n）--> O(nlogn)
		// 2. 从最后一个非叶子节点( (size - 1 - 1) / 2)，往上调整 O(logn) 叶子节点不用堆化
		// 所以选择方式2
		void buildHeap() {
			if (_arr.empty()) return;
			for (int i = ((int)_arr.size() - 1 - 1) / 2; i >= 0; i--) {
				adjustDown(i);
			}
		}

		// 入堆 push
		void push(const T& val) {
			_arr.push_back(val);
			adjustUp((int)_arr.size() - 1);
		}

		// 出堆 pop
		void pop() {
			assert(!empty() && "heap is empty!");
			// 1. 将堆顶元素和堆底元素交换
			swap(_arr[0], _arr[_arr.size() - 1]);
			// 2. 删除堆底元素
			_arr.pop_back();
			// 2. 向下调整恢复堆结构
			if (!empty()) adjustDown(0);
		}

		// 清空
		void clear() { if (!empty()) _arr.clear(); }

		// 取堆顶元素
		const T& top() { assert(!empty() && "heap is empty!"); return _arr[0]; }

		// 堆的其他性质
		bool empty() const { return _arr.empty(); }
		size_t size() const { return _arr.size(); }
		size_t capacity() const { return _arr.capacity(); }


	private:
		vector<T> _arr;

		// 向上调整操作
		void adjustUp(int child) {
			int parent = (child - 1) / 2;
			// child走到0  相当于走到了堆顶
			while (child > 0 && _arr[child] > _arr[parent]) {
				swap(_arr[child], _arr[parent]);
				child = parent;
				parent = (parent - 1) / 2;
			}
		}

		//  向下调整操作
		void adjustDown(int parent) {
			int child = 2 * parent + 1; // 假设左孩子比右孩子大
			int size = (int)_arr.size();
			while (child < size) {
				if (child + 1 < size && _arr[child] < _arr[child + 1]) // 确保右孩子存在 并且右孩子更大
					child = child + 1;
				if (_arr[parent] < _arr[child]) {
					swap(_arr[parent], _arr[child]);
					parent = child;
					child = 2 * child + 1;
				}
				else break;
			}
		}

	};

	// TopK 这里是大顶堆 找前k小
	vector<int> TopKSmall(int K, vector<int>& v) {
		if (K == 0 || K >= v.size()) { return v; }
		// 1. 建立大顶堆
		heap<int> maxHeap;
		// 2. 遍历数组 找前K小
		for (int i = 0; i < v.size(); i++) {
			if (maxHeap.size() < K) { // 放入前K个元素
				maxHeap.push(v[i]);
			}
			else if (v[i] < maxHeap.top()) { // 当前元素比堆顶小 说明有资格进入前K小名单
				maxHeap.pop(); // 淘汰堆顶
				maxHeap.push(v[i]); // 新元素入堆
			}
			// 否则 v[i] >= 堆顶 直接丢弃 连第K小都排不上
		}
		// 此时堆中保存了无序的前K小元素
		vector<int> ret;
		while (!maxHeap.empty()) {
			ret.push_back(maxHeap.top());
			maxHeap.pop();
		}

		return ret;
	}

	// 生成随机数据（海量）
	static vector<int> gen_data(size_t N, int lo = -1000000000, int hi = 1000000000, uint64_t seed = 20241103) {
		vector<int> a; a.reserve(N);
		mt19937_64 rng(seed);
		uniform_int_distribution<long long> dist(lo, hi);
		for (size_t i = 0; i < N; ++i) a.push_back((int)dist(rng));
		return a;
	}

	// 随机生成 N 个整数，调用 TopKSmall，并把结果升序打印（无任何额外输出）
	void TestTopKSmall(size_t N, int K, unsigned seed = 1) {
		vector<int> a; a.reserve(N);
		mt19937 rng(seed);
		uniform_int_distribution<int> dist(-100, 1000000000);
		for (size_t i = 0; i < N; ++i) a.push_back(dist(rng));

		auto ans = HEAP::TopKSmall(K, a);
		sort(ans.begin(), ans.end());
		for (size_t i = 0; i < ans.size(); ++i) {
			if (i) cout << ' ';
			cout << ans[i];
		}
		if (!ans.empty()) cout << '\n';
	}

}

namespace aaa {
	// 向下调整
	void adjustDown(vector<int>& v, int parent, int heapSize) {
		int child = 2 * parent + 1; // 假设左孩子比右孩子更大

		while (child < heapSize) {
			if (child + 1 < heapSize && v[child] < v[child + 1]) { // 假设错误
				child++;
			}

			if (v[child] > v[parent]) { // 违反大顶堆的规则
				swap(v[child], v[parent]);
				parent = child;
				child = 2 * child + 1; // 依旧假设左孩子更大
			}
			else break;	// 无需调整
		}
	}

	// 建立大顶堆
	void bulidMaxHeap(vector<int>& v) {
		// 叶子节点无需调整
		// 从第一个非叶子节点开始
		int size = (int)v.size();
		for (int i = (size - 1 - 1) / 2; i >= 0; i--) {
			adjustDown(v, i, size);
		}
	}

	void heapSort(vector<int>& v) {
		int size = (int)v.size();
		if (size <= 1) return;

		// 1. 建大顶堆
		bulidMaxHeap(v);

		// 2. 排序 把堆顶(最大)交换到当前区间末尾 缩小堆 再调整
		for (int end = size - 1; end > 0; end--) {
			swap(v[0], v[end]);
			adjustDown(v, 0, end); // 区间调整
		}
	}
}



// 建立小顶堆
// 堆排序
// 向下调整
void adjustDown(vector<int>& v, int parent, int heapSize) {
	int child = 2 * parent + 1; // 假设左孩子比右孩子更小

	while (child < heapSize) {
		if (child + 1 < heapSize && v[child] > v[child + 1]) { // 假设错误
			child++;
		}

		if (v[child] < v[parent]) { // 违反小顶堆的规则
			swap(v[child], v[parent]);
			parent = child;
			child = 2 * child + 1; // 依旧假设左孩子更小
		}
		else break;	// 无需调整
	}
}
void bulidMinHeap(vector<int>& v) {
	// 叶子节点无需调整
	// 从第一个非叶子节点开始
	int size = (int)v.size();
	for (int i = (size - 1 - 1) / 2; i >= 0; i--) {
		adjustDown(v, i, size);
	}
}

void heapSort(vector<int>& v) {
	int size = (int)v.size();
	if (size <= 1) return;

	// 1. 建小顶堆
	bulidMinHeap(v);

	// 2. 排序 把堆顶(最小)交换到当前区间末尾 缩小堆 再调整
	for (int end = size - 1; end > 0; end--) {
		swap(v[0], v[end]);
		adjustDown(v, 0, end); // 区间调整
	}
}