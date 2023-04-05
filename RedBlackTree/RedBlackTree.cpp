/*
* 红黑树代码
*/
#include<iostream>
using namespace std;

/*
* 节点结构
* {
    key，
*   value，
    color,
*   lchild，
*   rchild，
*   parent
* }
*/
const int RED = 1;
const int BlACK = 2;
template<class ValueType> 
class TreeNode
{
public:
    TreeNode(int k, ValueType v);
    virtual ~TreeNode();
    void changeChild(TreeNode *l = nullptr, TreeNode *r = nullptr);
    void changeColor(int c);
    

public:
    int key;
    ValueType value;
    int color;
    TreeNode *lChild;
    TreeNode *rChild;
    TreeNode *parent;
};

template<class ValueType> TreeNode<ValueType >::TreeNode(int k, ValueType v):
key(k), value(v), lChild(nullptr), rChild(nullptr), parent(nullptr), color(RED)
{
    cout << "key: " << key << ", value: " << value << endl;
}

template<class ValueType> TreeNode<ValueType>::~TreeNode()
{
    //cout << "delete node" << endl;
}

template<class ValueType> void TreeNode<ValueType>::changeChild(TreeNode *l, TreeNode *r)
{
    if(l != nullptr)
    {
        lChild = l;
    }
    if(r != nullptr)
    {
        rChild = r;
    }
}

template<class ValueType> void TreeNode<ValueType>::changeColor(int c)
{
    if(c == RED || c == BlACK)
    {
        color = c;
    }
    else
    {
        cout << "Wrong clolor" << endl;
    }
}


/*
* 红黑树
* 成员变量：{
    根节点指针
}
*/
template<class ValueType>
class RedBlackTree
{
public:
    RedBlackTree(TreeNode<ValueType> *r);
    virtual ~RedBlackTree();
    void printTree(TreeNode<ValueType> *T, int nLayer);
    void insertNode(TreeNode<ValueType> *node);
    void deleteNode(int key);
    void creatRBTree();
    TreeNode<ValueType> *findNode(int key);
    TreeNode<ValueType> *findAfterNode(TreeNode<ValueType> *node);//找node的后继节点

private:
    void fixAfterInsert(TreeNode<ValueType> *node);
    void leftRotate(TreeNode<ValueType> *node);
    void rightRotate(TreeNode<ValueType> *node);

public:
    TreeNode<ValueType> *root;
};

template<class ValueType> RedBlackTree<ValueType>::RedBlackTree(TreeNode<ValueType> *r)
{
    if(r != nullptr)
    {
        r->changeColor(BlACK);
        root = r;
        cout << "root created" << endl;
    }
    else
    {
        cout << "empty tree" << endl;
    }
}

template<class ValueType> RedBlackTree<ValueType>::~RedBlackTree()
{
    
}

template<class ValueType> void RedBlackTree<ValueType>::printTree(TreeNode<ValueType> *T, int nLayer)
{
    if(T == nullptr)
    {
        return;
    }
    printTree(T->rChild, nLayer + 5);
    for (int i = 0; i < nLayer; i++)
    {
        cout << " ";
    }
    cout << T->key <<"(" << T->color <<")" << endl;
    printTree(T->lChild, nLayer + 5);
}

template<class ValueType> void RedBlackTree<ValueType>::insertNode(TreeNode<ValueType> *node)
{
    if(node == nullptr)
    {
        cout << "node can not be null, insert failed";
        return;
    }

    TreeNode<ValueType> *p = root;
    //先找到应该插入节点的位置，然后进行插入
    while(1)
    {
        if(node->key < p->key)
        {
            if(p->lChild != nullptr)
            {
                p = p->lChild;
            }
            else
            {
                p->lChild = node;
                node->parent = p;
                break;
            }
        }
        if(node->key > p->key)
        {
            if(p->rChild != nullptr)
            {
                p = p->rChild;
            }
            else
            {
                p->rChild = node;
                node->parent = p;
                break;
            }
        }
    }
    //插入以后再进行调整
    fixAfterInsert(node);
}

template<class ValueType> void RedBlackTree<ValueType>::fixAfterInsert(TreeNode<ValueType> *node)
{
    //只有当父节点是红色的时候才会进行调整
    TreeNode<ValueType> *father = node->parent;
    TreeNode<ValueType> *grandFather = father->parent;
    if(father != nullptr)
    {
        if(father->color == RED)
        {
        //情况一：父亲为红色且没有兄弟，即在2-3-4树中，父亲的父亲为3节点
        //只需要进行左旋或者右旋，将其旋转后的根变为黑色，左右孩子变为红色
            if(father == grandFather->lChild && grandFather->rChild == nullptr)
            {
                //右旋
                if(node == father->rChild)
                {
                    leftRotate(node);
                    node = father;
                    father = father->parent;
                }
                father->color = BlACK;
                grandFather->color = RED;
                rightRotate(grandFather);
            }
            else if(father == grandFather->rChild && grandFather->lChild == nullptr)
            {
                //左旋
                if(node == father->lChild)
                {
                    rightRotate(node);
                    node = father;
                    father = father->parent;
                }
                    father->color = BlACK;
                    grandFather->color = RED;
                    leftRotate(grandFather);
            }

                //情况二：父亲为红色且有兄弟，即在2-3-4树中，父亲的父亲为4节点
                else if(grandFather->lChild != nullptr && grandFather->rChild != nullptr)
                {
                    //只需把爷爷变红色(如果是根节点，再变黑），父亲及父亲的兄弟变黑
                    grandFather->color = RED;
                    if(root == grandFather)
                    {
                        grandFather->color = BlACK;
                    }
                    grandFather->lChild->color = BlACK;
                    grandFather->rChild->color = BlACK;
                }
        }
    }

}

template<class ValueType> void RedBlackTree<ValueType>::leftRotate(TreeNode<ValueType> *node)
{
    TreeNode<ValueType> *r = node->rChild;
    TreeNode<ValueType> *p = node->parent;
    if(r != nullptr)
    {
        r->parent = node->parent;
        if(p != nullptr)
        {
            if(p->lChild == node)
            {
                p->lChild = r;
            }
            else
            {
                p->rChild = r;
            }
        }
        node->parent = r;
        node->rChild = r->lChild;
        if(r->lChild != nullptr)
        {
            r->lChild->parent = node;
        }
        r->lChild = node;
        if(node == root)
        {
            root = r;
        }
    }
}

template<class ValueType> void RedBlackTree<ValueType>::rightRotate(TreeNode<ValueType> *node)
{
    TreeNode<ValueType> *l = node->lChild;
    TreeNode<ValueType> *p = node->parent;
    if(l != nullptr)
    {
        l->parent = node->parent;
        if(p != nullptr)
        {
            if(p->lChild == node)
            {
                p->lChild = l;
            }
            else
            {
                p->rChild = l;
            }
        }
        node->parent = l;
        node->lChild = l->rChild;
        if(l->rChild != nullptr)
        {
            l->rChild->parent = node;
        }
        l->rChild = node;
        if(node == root)
        {
            root = l;
        }
    }
}

template<class ValueType> void RedBlackTree<ValueType>::creatRBTree()
{
    while(1)
    {
        int key;
        ValueType value;
        cout << "input key: ";
        cin >> key;
        if(key < 0)
        {
            cout << "over" << endl;
            return;
        }
        cout << "input value: ";
        cin >> value;
        TreeNode<ValueType> *node = new TreeNode<ValueType>(key, value);
        insertNode(node);
    }
}

template<class ValueType>  TreeNode<ValueType>* RedBlackTree<ValueType>::findNode(int key)
{
    if(root == nullptr)
    {
        cout << "empty tree" << endl;
        return nullptr;
    }
    TreeNode<ValueType> *node = root;
    while(node->key != key && node != nullptr)
    {
        if(key < node->key)
        {
            node = node->lChild;
        }
        else if(key > node->key)
        {
            node = node->rChild;
        }
    }
    return node;
}

template<class ValueType> void RedBlackTree<ValueType>::deleteNode(int key)
{
    TreeNode<ValueType> *node = findNode(key);
    if(node != nullptr)
    {
        //第一步：先找到实际删除的节点，实际就是找到后继节点
        //第二步：再根据具体情况来进行删除
        //情况1：删除节点有两个子节点，找到其后继节点，通过删除后继节点来进行替代删除
        if(node->lChild != nullptr && node->rChild != nullptr)
        {
            TreeNode<ValueType> *afterNode = findAfterNode(node);
            node->key = afterNode->key;
            node->value = afterNode->value;
            node = afterNode;
        }

        //情况2：删除节点只有一个子节点，必为上黑下红,直接用子节点替换父节点
        if(node->lChild != nullptr && node->rChild == nullptr)
        {
            TreeNode<ValueType> *child = node->lChild;
            node->key = child->key;
            node->value = child->value;
            node->lChild = nullptr;
            delete child;
            child = nullptr;
        }
        else if(node->lChild == nullptr && node->rChild != nullptr)
        {
            TreeNode<ValueType> *child = node->rChild;
            node->key = child->key;
            node->value = child->value;
            node->rChild = nullptr;
            delete child;
            child = nullptr;
        }

        //情况3：删除节点没有子节点，该节点可能为红，可能为黑
        else if(node->lChild == nullptr && node->rChild == nullptr)
        //情况3.1：删除节点为红，直接删除
        {        
            if(node->color == RED)
            {
                TreeNode<ValueType> *father = node->parent;
                if(node == father->lChild)
                {
                    father->lChild = nullptr;
                }
                else
                {
                    father->rChild = nullptr;
                }
                delete node;
                node = nullptr;
            }
            //情况3.2：删除节点为黑色
            else
            {
                //又分三种情况
                TreeNode<ValueType> *father = node->parent;
                TreeNode<ValueType> *brother = nullptr;
                if(father == nullptr)
                {
                    cout << "delete this tree" << endl;
                    delete node;
                    root = nullptr;
                }
                else if(node == father->lChild)
                {
                    brother = father->rChild;
                    //情况3.2.1：兄弟节点是红色，此时该红色节点并不是真正的2-3-4树上的兄弟节点，首先找到真正的兄弟节点
                    if(brother->color == RED)
                    {
                        father->color = RED;
                        brother->color = BlACK;
                        leftRotate(father);
                        brother = father->rChild;
                    }
                    //情况3.2.2：兄弟节点是黑色，并且兄弟节点有孩子
                    if(brother->color == BlACK && (brother->lChild != nullptr || brother->rChild != nullptr))
                    {
                        if(brother->lChild != nullptr && brother->rChild == nullptr)
                        {
                            brother->lChild->color = BlACK;
                            brother->color = RED;
                            rightRotate(brother);
                            brother = father->rChild;//旋转后要重新寻找兄弟
                        }
                        brother->rChild->color = brother->color;
                        brother->color = father->color;
                        father->color = node->color;
                        leftRotate(father);
                        father->lChild = nullptr;
                        delete node;
                        node = nullptr;
                    }
                    //情况3.2.3：兄弟节点是黑色，并且没有孩子
                    else if(brother->color == BlACK && brother->lChild == nullptr && brother->rChild == nullptr)
                    {
                        father->lChild = nullptr;
                        delete node;
                        node = nullptr;
                        while(father->color != RED && father != root)
                        {
                            brother->color = RED;
                            TreeNode<ValueType> *grandFather = father->parent;
                            if(father == grandFather->lChild)
                            {
                                brother = grandFather->rChild;
                            }
                            else
                            {
                                brother = grandFather->lChild;
                            }
                            father = grandFather;
                        }
                        father->color = BlACK;
                        brother->color = RED;
                    }
                }
                else
                {
                    brother = father->lChild;
                    //情况3.2.1：兄弟节点是红色，此时该红色节点并不是真正的2-3-4树上的兄弟节点，首先找到真正的兄弟节点
                    if(brother->color == RED)
                    {
                        father->color = RED;
                        brother->color = BlACK;
                        rightRotate(father);
                        brother = father->lChild;
                    }
                    //情况3.2.2：兄弟节点是黑色，并且兄弟节点有孩子
                    if(brother->color == BlACK && (brother->lChild != nullptr || brother->rChild != nullptr))
                    {
                        if(brother->rChild != nullptr && brother->lChild == nullptr)
                        {
                            brother->rChild->color = BlACK;
                            brother->color = RED;
                            leftRotate(brother);
                            brother = father->lChild;//旋转后要重新寻找兄弟
                        }
                        brother->lChild->color = brother->color;
                        brother->color = father->color;
                        father->color = node->color;
                        rightRotate(father);
                        father->rChild = nullptr;
                        delete node;
                        node = nullptr;
                    }
                    //情况3.2.3：兄弟节点是黑色，并且没有孩子
                    if(brother->color == BlACK && brother->lChild == nullptr && brother->rChild == nullptr)
                    {
                        father->rChild = nullptr;
                        delete node;
                        node = nullptr;
                        while(father->color != RED && father != root)
                        {
                            brother->color = RED;
                            TreeNode<ValueType> *grandFather = father->parent;
                            if(father == grandFather->lChild)
                            {
                                brother = grandFather->rChild;
                            }
                            else
                            {
                                brother = grandFather->lChild;
                            }
                            father = grandFather;
                        }
                        father->color = BlACK;
                    }
                }
            }
        }    
}
    else
    {
        cout << "Don't exist this node" << endl;
    }
}

template<class ValueType>  TreeNode<ValueType>* RedBlackTree<ValueType>::findAfterNode(TreeNode<ValueType> *node)
{
    if(node == nullptr)
    {
        cout << "node is empty" << endl;
        return nullptr;
    }
    if(node->rChild == nullptr)
    {
        cout << "no more node after this node" << endl;
        return nullptr;
    }
    TreeNode<ValueType> *p = node->rChild;
    while(p->lChild != nullptr)
    {
        p = p->lChild;
    }
    return p;
}

int main()
{
    TreeNode<int> *r = new TreeNode<int>(1, 1);
    RedBlackTree<int> rbTree(r);
    rbTree.creatRBTree();
    rbTree.printTree(rbTree.root, 0);
    while(1)
    {
        int k;
        cin >> k;
        rbTree.deleteNode(k);
        rbTree.printTree(rbTree.root, 0);
    }
}