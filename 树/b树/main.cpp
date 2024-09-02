#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cstring>

#define DEGREE 3 // B树的阶数
typedef int KEY_VALUE;

class BTreeNode
{
public:
    KEY_VALUE *keys;                // 节点本身的keys
    BTreeNode **children;           // 子结点的数组
    int num;                        // 节点本身keys的数量
    bool leaf;                      // 是否是叶子节点 true：yes  false：no

    BTreeNode(int t, bool leaf);
    ~BTreeNode();
};

class BTree
{
public:
    BTreeNode *root;
    int t; // B树的阶数

    BTree(int t);
    ~BTree();

    void insert(KEY_VALUE key);
    void remove(KEY_VALUE key);
    void traverse();

private:
    void splitChild(BTreeNode *x, int i);
    void insertNonFull(BTreeNode *x, KEY_VALUE k);
    void traverse(BTreeNode *x);
    void print(BTreeNode *node, int layer);
    int binSearch(BTreeNode *node, int low, int high, KEY_VALUE key);
    void merge(BTreeNode *node, int idx);
    void deleteKey(BTreeNode *node, KEY_VALUE key);
    void removeFromLeaf(BTreeNode *node, int idx);
    void removeFromNonLeaf(BTreeNode *node, int idx);
    KEY_VALUE getPredecessor(BTreeNode *node, int idx);
    KEY_VALUE getSuccessor(BTreeNode *node, int idx);
    void fill(BTreeNode *node, int idx);
    void borrowFromPrev(BTreeNode *node, int idx);
    void borrowFromNext(BTreeNode *node, int idx);
};

// BTreeNode 构造函数
BTreeNode::BTreeNode(int t, bool leaf)
{
    this->leaf = leaf;
    this->keys = new KEY_VALUE[2 * t - 1];
    this->children = new BTreeNode *[2 * t];
    this->num = 0;
}

// BTreeNode 析构函数
BTreeNode::~BTreeNode()
{
    delete[] keys;
    delete[] children;
}

// BTree 构造函数
BTree::BTree(int t)
{
    this->t = t;
    root = new BTreeNode(t, true);
}

// BTree 析构函数
BTree::~BTree()
{
    delete root;
}

// 分裂子节点
void BTree::splitChild(BTreeNode *x, int i)
{
    BTreeNode *y = x->children[i];
    BTreeNode *z = new BTreeNode(t, y->leaf);

    z->num = t - 1;
    for (int j = 0; j < t - 1; j++)
    {
        z->keys[j] = y->keys[j + t];
    }
    if (!y->leaf)
    {
        for (int j = 0; j < t; j++)
        {
            z->children[j] = y->children[j + t];
        }
    }
    y->num = t - 1;

    for (int j = x->num; j >= i + 1; j--)
    {
        x->children[j + 1] = x->children[j];
    }
    x->children[i + 1] = z;

    for (int j = x->num - 1; j >= i; j--)
    {
        x->keys[j + 1] = x->keys[j];
    }
    x->keys[i] = y->keys[t - 1];
    x->num++;
}

// 插入非满节点
void BTree::insertNonFull(BTreeNode *x, KEY_VALUE k)
{
    int i = x->num - 1;
    if (x->leaf)
    {
        while (i >= 0 && x->keys[i] > k)
        {
            x->keys[i + 1] = x->keys[i];
            i--;
        }
        x->keys[i + 1] = k;
        x->num++;
    }
    else
    {
        while (i >= 0 && x->keys[i] > k)
            i--;
        if (x->children[i + 1]->num == 2 * t - 1)
        {
            splitChild(x, i + 1);
            if (k > x->keys[i + 1])
                i++;
        }
        insertNonFull(x->children[i + 1], k);
    }
}

// 插入新元素
void BTree::insert(KEY_VALUE key)
{
    BTreeNode *r = root;
    if (r->num == 2 * t - 1)
    {
        BTreeNode *node = new BTreeNode(t, false);
        root = node;
        node->children[0] = r;
        splitChild(node, 0);

        int i = 0;
        if (node->keys[0] < key)
            i++;
        insertNonFull(node->children[i], key);
    }
    else
    {
        insertNonFull(r, key);
    }
}

// 遍历树
void BTree::traverse(BTreeNode *x)
{
    int i;
    for (i = 0; i < x->num; i++)
    {
        if (!x->leaf)
            traverse(x->children[i]);
        std::cout << x->keys[i] << " ";
    }
    if (!x->leaf)
        traverse(x->children[i]);
}

// 公开的遍历接口
void BTree::traverse()
{
    if (root)
        traverse(root);
    std::cout << std::endl;
}

// 从叶子节点删除
void BTree::removeFromLeaf(BTreeNode *node, int idx)
{
    for (int i = idx + 1; i < node->num; i++)
        node->keys[i - 1] = node->keys[i];
    node->num--;
}

// 从非叶子节点删除
void BTree::removeFromNonLeaf(BTreeNode *node, int idx)
{
    KEY_VALUE key = node->keys[idx];

    if (node->children[idx]->num >= t)
    {
        KEY_VALUE pred = getPredecessor(node, idx);
        node->keys[idx] = pred;
        deleteKey(node->children[idx], pred);
    }
    else if (node->children[idx + 1]->num >= t)
    {
        KEY_VALUE succ = getSuccessor(node, idx);
        node->keys[idx] = succ;
        deleteKey(node->children[idx + 1], succ);
    }
    else
    {
        merge(node, idx);
        deleteKey(node->children[idx], key);
    }
}

// 获取前驱
KEY_VALUE BTree::getPredecessor(BTreeNode *node, int idx)
{
    BTreeNode *cur = node->children[idx];
    while (!cur->leaf)
        cur = cur->children[cur->num];
    return cur->keys[cur->num - 1];
}

// 获取后继
KEY_VALUE BTree::getSuccessor(BTreeNode *node, int idx)
{
    BTreeNode *cur = node->children[idx + 1];
    while (!cur->leaf)
        cur = cur->children[0];
    return cur->keys[0];
}

// 填充子节点
void BTree::fill(BTreeNode *node, int idx)
{
    if (idx != 0 && node->children[idx - 1]->num >= t)
        borrowFromPrev(node, idx);
    else if (idx != node->num && node->children[idx + 1]->num >= t)
        borrowFromNext(node, idx);
    else
    {
        if (idx != node->num)
            merge(node, idx);
        else
            merge(node, idx - 1);
    }
}

// 从前一个兄弟借一个键
void BTree::borrowFromPrev(BTreeNode *node, int idx)
{
    BTreeNode *child = node->children[idx];
    BTreeNode *sibling = node->children[idx - 1];

    for (int i = child->num - 1; i >= 0; i--)
        child->keys[i + 1] = child->keys[i];

    if (!child->leaf)
    {
        for (int i = child->num; i >= 0; i--)
            child->children[i + 1] = child->children[i];
    }

    child->keys[0] = node->keys[idx - 1];

    if (!node->leaf)
        child->children[0] = sibling->children[sibling->num];

    node->keys[idx - 1] = sibling->keys[sibling->num - 1];
    child->num += 1;
    sibling->num -= 1;
}

// 从下一个兄弟借一个键
void BTree::borrowFromNext(BTreeNode *node, int idx)
{
    BTreeNode *child = node->children[idx];
    BTreeNode *sibling = node->children[idx + 1];

    child->keys[child->num] = node->keys[idx];

    if (!child->leaf)
        child->children[child->num + 1] = sibling->children[0];

    node->keys[idx] = sibling->keys[0];

    for (int i = 1; i < sibling->num; i++)
        sibling->keys[i - 1] = sibling->keys[i];

    if (!sibling->leaf)
    {
        for (int i = 1; i <= sibling->num; i++)
            sibling->children[i - 1] = sibling->children[i];
    }

    child->num += 1;
    sibling->num -= 1;
}

// 合并子节点
void BTree::merge(BTreeNode *node, int idx)
{
    BTreeNode *child = node->children[idx];
    BTreeNode *sibling = node->children[idx + 1];

    child->keys[t - 1] = node->keys[idx];

    for (int i = 0; i < sibling->num; i++)
        child->keys[i + t] = sibling->keys[i];

    if (!child->leaf)
    {
        for (int i = 0; i <= sibling->num; i++)
            child->children[i + t] = sibling->children[i];
    }

    for (int i = idx + 1; i < node->num; i++)
        node->keys[i - 1] = node->keys[i];

    for (int i = idx + 2; i <= node->num; i++)
        node->children[i - 1] = node->children[i];

    child->num += sibling->num + 1;
    node->num--;

    delete sibling;
}

// 删除关键字
void BTree::deleteKey(BTreeNode *node, KEY_VALUE key)
{
    int idx = 0;
    while (idx < node->num && node->keys[idx] < key)
        idx++;

    if (idx < node->num && node->keys[idx] == key)
    {
        if (node->leaf)
            removeFromLeaf(node, idx);
        else
            removeFromNonLeaf(node, idx);
    }
    else
    {
        if (node->leaf)
        {
            std::cout << "The key " << key << " is not in the tree\n";
            return;
        }

        bool flag = (idx == node->num);

        if (node->children[idx]->num < t)
            fill(node, idx);

        if (flag && idx > node->num)
            deleteKey(node->children[idx - 1], key);
        else
            deleteKey(node->children[idx], key);
    }
}

// 删除元素
void BTree::remove(KEY_VALUE key)
{
    if (!root)
    {
        std::cout << "The tree is empty\n";
        return;
    }

    deleteKey(root, key);

    if (root->num == 0)
    {
        BTreeNode *tmp = root;
        if (root->leaf)
            root = nullptr;
        else
            root = root->children[0];

        delete tmp;
    }
}

// 主函数
int main()
{
    BTree tree(3);
    srand(48);

    char key[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (int i = 0; i < 26; i++)
    {
        std::cout << key[i] << " ";
        tree.insert(key[i]);
    }
    std::cout << std::endl;
    tree.traverse();
    for (int i = 0; i < 26; i++)
    {
        std::cout << "\n---------------------------------\n";
        tree.remove(key[25 - i]);
        tree.traverse();
    }
    return 0;
}
