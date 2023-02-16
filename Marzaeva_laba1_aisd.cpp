#include <queue>
#include <iostream>
#include <vector>
#include <random>
using namespace std;

const int N = 15;
int chek = 0;

void GetRandomNumericalSequence(vector<int>& (vec)) //random number generation
{
    srand(time(NULL));
    mt19937_64 rnd(time(0));
    for (int i = 0; i < N; i++)
    {
        int chek = 0;
        int tmp = rnd() % 100;
        vec[i] = tmp;
        for (int j = 0; j < i; j++) //checking for the same numbers
        {
            if (tmp == vec[j])
                chek = 1;
        }
        if (chek == 0)
            vec[i] = tmp;
        else
            i--;
    }
    cout << "Set initial values:\n";
    for (int i = 0; i < N; i++)
    {
        cout << vec[i] << " ";
    }
    cout << "\n";
}

struct RBT //structure for red-black tree
{
    int data;
    char color;
    RBT* left, * right, * parent;
    RBT(int data)
    {
        this->data = data;
        left = right = parent = nullptr;
        this->color = 'r';
    }
};

class RBTree //class for red-black tree
{
private:
    RBT* root;
protected:
    void rotateLeft(RBT*&, RBT*&);
    void rotateRight(RBT*&, RBT*&);
    void fixViolation(RBT*&, RBT*&);
    void draw_tree_rbt(RBT*&);
    int height(RBT*&);
public:
    RBTree() { root = nullptr; }
    void insert(const int& n, int i);
};

RBT* BSTInsert(RBT* root, RBT* pt) //insertion as in a binary search tree
{
    if (root == nullptr)
        return pt;
    if (pt->data < root->data)
    {
        root->left = BSTInsert(root->left, pt);
        root->left->parent = root;
    }
    else if (pt->data > root->data)
    {
        root->right = BSTInsert(root->right, pt);
        root->right->parent = root;
    }
    return root;
}

void RBTree::rotateLeft(RBT*& root, RBT*& pt) //turn left
{
    RBT* pt_right = pt->right;

    pt->right = pt_right->left;

    if (pt->right != nullptr)
        pt->right->parent = pt;

    pt_right->parent = pt->parent;

    if (pt->parent == nullptr)
        root = pt_right;

    else if (pt == pt->parent->left)
        pt->parent->left = pt_right;

    else
        pt->parent->right = pt_right;

    pt_right->left = pt;
    pt->parent = pt_right;
}

void RBTree::rotateRight(RBT*& root, RBT*& pt) //turn right
{
    RBT* pt_left = pt->left;

    pt->left = pt_left->right;

    if (pt->left != nullptr)
        pt->left->parent = pt;

    pt_left->parent = pt->parent;

    if (pt->parent == nullptr)
        root = pt_left;

    else if (pt == pt->parent->left)
        pt->parent->left = pt_left;

    else
        pt->parent->right = pt_left;

    pt_left->right = pt;
    pt->parent = pt_left;
}

void RBTree::fixViolation(RBT*& root, RBT*& pt) //correction of violations
{
    RBT* parent_pt = nullptr;
    RBT* grand_parent_pt = nullptr;

    while ((pt != root) && (pt->color != 'b') && (pt->parent->color == 'r'))
    {

        parent_pt = pt->parent;
        grand_parent_pt = pt->parent->parent;

        if (parent_pt == grand_parent_pt->left)
        {

            RBT* uncle_pt = grand_parent_pt->right;

            if (uncle_pt != nullptr && uncle_pt->color == 'r')
            {
                grand_parent_pt->color = 'r';
                parent_pt->color = 'b';
                uncle_pt->color = 'b';
                pt = grand_parent_pt;
            }

            else
            {
                if (pt == parent_pt->right)
                {
                    rotateLeft(root, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }

                rotateRight(root, grand_parent_pt);
                swap(parent_pt->color, grand_parent_pt->color);
                pt = parent_pt;
            }
        }

        else
        {
            RBT* uncle_pt = grand_parent_pt->left;

            if ((uncle_pt != nullptr) && (uncle_pt->color == 'r'))
            {
                grand_parent_pt->color = 'r';
                parent_pt->color = 'b';
                uncle_pt->color = 'b';
                pt = grand_parent_pt;
            }
            else
            {
                if (pt == parent_pt->left)
                {
                    rotateRight(root, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }

                rotateLeft(root, grand_parent_pt);
                swap(parent_pt->color, grand_parent_pt->color);
                pt = parent_pt;
            }
        }
    }
    root->color = 'b';
}

void RBTree::insert(const int& data, int i) //insert in rbt
{
    RBT* pt = new RBT(data);
    root = BSTInsert(root, pt);
    fixViolation(root, pt);
    if (i == N - 1) {
        cout << "\nRed-Black Tree:\n";
        draw_tree_rbt(root);
        cout << "The height of the red-black tree is \n" << height(root) + 1;
    }
}

int tab = 0;

void RBTree::draw_tree_rbt(RBT*& root) { //draw the rb tree
    if (!root) return;
    tab += 5;
    draw_tree_rbt(root->right);
    for (int i = 0; i < tab; i++)
        cout << " ";
    cout << "->";
    cout << root->data << " " << root->color << endl;
    draw_tree_rbt(root->left);
    tab -= 5;
    return;
}

int RBTree::height(RBT*& root) //finding the depth of the tree
{
    if (root == nullptr) {
        return 0;
    }
    if (root->color == 'b')
        return 1 + max(height(root->left), height(root->right));
    else
        return max(height(root->left), height(root->right));
}

struct Elem { //binary tree structure
    int key = 0;
    Elem* left = nullptr, * right = nullptr, * parent = nullptr;
};

void add(int elem, Elem*& root) { // adding a new randomly generated element to the tree
    if (!root) {
        root = new Elem;
        root->key = elem;
        root->left = 0;
        root->right = 0;
        return;
    }
    else {
        if (root->key > elem) add(elem, root->left);
        else add(elem, root->right);
    }
}

void sort_tree(Elem*& root, vector<int>& (vec))
{
    root = nullptr;
    for (int i = 0; i < N; i++)
        add(vec[i], root);
}

void draw_tree(Elem* root) { //draw the tree
    if (!root) return;
    tab += 5;
    draw_tree(root->right);
    for (int i = 0; i < tab; i++) cout << " ";
    cout << "->";
    cout << root->key << endl;
    draw_tree(root->left);
    tab -= 5;
    return;
}

int height(Elem* root)
{
    if (root == nullptr) {
        return 0;
    }
    return 1 + max(height(root->left), height(root->right));
}

class AVL
{
    struct node
    {
        int data;
        node* left;
        node* right;
        int height;
    };

    node* root;

    void draw_tree(node*&);
    int height_AVL(node*&);

    node* insert(int x, node* t, int i)
    {
        if (i == N - 1)
            chek++;
        if (t == NULL)
        {
            t = new node;
            t->data = x;
            t->height = 0;
            t->left = t->right = NULL;
        }
        else if (x < t->data)
        {
            t->left = insert(x, t->left, i);
            if (height(t->left) - height(t->right) == 2)
            {
                if (x < t->left->data)
                    t = singleRightRotate(t);
                else
                    t = doubleRightRotate(t);
            }
        }
        else if (x > t->data)
        {
            t->right = insert(x, t->right, i);
            if (height(t->right) - height(t->left) == 2)
            {
                if (x > t->right->data)
                    t = singleLeftRotate(t);
                else
                    t = doubleLeftRotate(t);
            }
        }

        t->height = max(height(t->left), height(t->right)) + 1;
        if (i == N - 1)
            chek--;
        if (i == N - 1 && chek == 0) {
            cout << "\nAVL Tree:\n";
            draw_tree(t);
            cout << "The height of the AVL tree is \n" << height_AVL(t);
        }
        return t;
    }

    node* singleRightRotate(node*& t)
    {
        node* u = t->left;
        t->left = u->right;
        u->right = t;
        t->height = max(height(t->left), height(t->right)) + 1;
        u->height = max(height(u->left), t->height) + 1;
        return u;
    }

    node* singleLeftRotate(node*& t)
    {
        node* u = t->right;
        t->right = u->left;
        u->left = t;
        t->height = max(height(t->left), height(t->right)) + 1;
        u->height = max(height(t->right), t->height) + 1;
        return u;
    }

    node* doubleLeftRotate(node*& t)
    {
        t->right = singleRightRotate(t->right);
        return singleLeftRotate(t);
    }

    node* doubleRightRotate(node*& t)
    {
        t->left = singleLeftRotate(t->left);
        return singleRightRotate(t);
    }

    int height(node* t)
    {
        return (t == NULL ? -1 : t->height);
    }

public:
    AVL()
    {
        root = NULL;
    }

    void insert(int x, int i)
    {
        root = insert(x, root, i);
    }
};

void AVL::draw_tree(node*& root) {
    if (!root) return;
    tab += 5;
    draw_tree(root->right);
    for (int i = 0; i < tab; i++) cout << " ";
    cout << "->";
    cout << root->data << endl;
    draw_tree(root->left);
    tab -= 5;
    return;
}

int AVL::height_AVL(node*& root)
{
    if (root == nullptr) {
        return 0;
    }
    return 1 + max(height_AVL(root->left), height_AVL(root->right));
}

int main()
{
    RBTree tree;
    vector <int> vec(N);
    int num;
    int ch = 0;
    GetRandomNumericalSequence(vec);
    while (1) {
        cout << "\nEnter the number of the tree you want to build\nPress 0 to end the program\n";
        cout << "1. BS tree\n" << "2. RB tree\n" << "3. AVL tree\n";
        cin >> num;
        if (num == 1)
        {
            ch++;
            Elem* root = new Elem;
            clock_t start = clock();
            sort_tree(root, vec);
            clock_t end = clock();
            cout << "\nBinary search tree:\n";
            draw_tree(root);
            cout << "The height of the binary search tree is \n" << height(root);
            double seconds = (double)(end - start) / CLOCKS_PER_SEC;
            cout << "\nThe time:" << seconds << "seconds\n";
        }
        if (num == 2)
        {
            ch++;
            clock_t start = clock();
            for (int i = 0; i < N; i++)
                tree.insert(vec[i], i);
            clock_t end = clock();
            double seconds = (double)(end - start) / CLOCKS_PER_SEC;
            cout << "\nThe time:" << seconds << "seconds\n";
        }
        if (num == 3)
        {
            ch++;
            AVL b;
            int i = 0;
            clock_t start = clock();
            for (i = 0; i < N; i++)
                b.insert(vec[i], i);
            clock_t end = clock();
            double seconds = (double)(end - start) / CLOCKS_PER_SEC;
            cout << "\nThe time:" << seconds << "seconds\n";
        }
        if (num == 0)
            return 0;
        if (ch == 3)
            return 0;
    }
    return 0;
}