#include "RedBlackTree.h"

RedBlackTree::RedBlackTree()
    : root(nullptr)
{
}

RedBlackTree::~RedBlackTree()
{
    clear();
}

void RedBlackTree::clear()
{
    clear(root);
    root = nullptr;
}

void RedBlackTree::clear(ResearcherNode *node)
{
    //TODO:
    if(node == nullptr){
        return;
    }

    clear(node->left);
    clear(node->right);
    
    delete node;
}

ResearcherNode *RedBlackTree::findResearcher(const std::string &fullName) const
{
    return find(root, fullName);
}

ResearcherNode *RedBlackTree::find(ResearcherNode *node, const std::string &fullName) const
{

    //TODO: search and find researcher by name
    if(node == nullptr){
        return nullptr;
    }

    if(node->data.fullName == fullName){
        return node;
    }
    
    if (fullName < node->data.fullName)
        return find(node->left, fullName);
    else
        return find(node->right, fullName);

}

bool RedBlackTree::insertResearcher(const Researcher &researcher)
{
    //attempt to insert, erturn true if success, false otherwise
    bool inserted = false;
    ResearcherNode* nodeToAdd = new ResearcherNode(researcher);

    root = bstInsert(root,nodeToAdd,inserted);
    
    if(!inserted){
        delete nodeToAdd;
        return false;
    }

    insertFixup(nodeToAdd);
    return true;
}

ResearcherNode *RedBlackTree::bstInsert(ResearcherNode *current, ResearcherNode *node, bool &inserted)
{
    // TODO:
    // 1) Standard BST insert by node->data.fullName.
    // 2) If tree is empty, node becomes root (set inserted = true).
    // 3) If name already exists, inserted = false and return current without inserting.
    // 4) Otherwise insert and set inserted = true.
    // 5) Do not modify colors here (node->color already RED).

    if (current == nullptr) {
        inserted = true;
        return node; // new root
    }

    if (node->data.fullName == current->data.fullName) {
        inserted = false;
        return current;
    }

    // LEFT
    if (node->data.fullName < current->data.fullName) {
        ResearcherNode* child = bstInsert(current->left, node, inserted);
        if (inserted) {
            current->left = child;
            child->parent = current;
        }
    }
    // RİGHT
    else {
        ResearcherNode* child = bstInsert(current->right, node, inserted);
        if (inserted) {
            current->right = child;
            child->parent = current;
        }
    }

    return current;
}

//------------------------------------
// I used explanations directed at myself and wrote explanations in Turkish 
// so that I could understand better when I looked back later. 
void RedBlackTree::insertFixup(ResearcherNode *node)
{
    // TODO: Standard Red-Black insertion fixup.
    // While parent is RED, handle cases based on uncle's color and position.
    // Ensure root becomes BLACK at the end.

    // Kural: Ben Kök değilsem VE Babam Kırmızı olduğu sürece dön.
    while (node != root && node->parent != nullptr && node->parent->color == RED) {
        
        ResearcherNode* father = node->parent;
        ResearcherNode* grandpa = father->parent;
        if (!grandpa) break;

        // --- SENARYO 1: Babam SOL tarafta ---
        if (father == grandpa->left) {
            ResearcherNode* uncle = grandpa->right;

            // DURUM 1: Amca da KIRMIZI (Renk Değişimi)
            // Sadece boya badana yapıyoruz, yapı değişmiyor.
            if (uncle != nullptr && uncle->color == RED) {
                father->color = BLACK;      // Babayı Siyah yap
                uncle->color = BLACK;       // Amcayı Siyah yap
                grandpa->color = RED;       // Dedeyi Kırmızı yap
                node = grandpa;             // Sorun yukarı taşınmış olabilir, artık 'node' dede oldu.
            } 
            // DURUM 2: Amca SİYAH veya YOK (Rotasyon Gerekli)
            else {
                // Durum 2a: Zikzak (Ben Sağdayım) -> Düz çizgiye çevir
                if (node == father->right) {
                    node = father;          // Bir üste çık
                    rotateLeft(node);       // Sola döndür
                    // Artık baba ve ben yer değiştirdik, düz çizgi olduk.
                    father = node->parent;  // 'father' pointerını güncelle
                    grandpa = father->parent;
                }
                
                // Durum 2b: Düz Çizgi (Sol-Sol) -> Sağa yatır
                father->color = BLACK;      // Babayı Siyah yap (Tepeye o geçecek)
                grandpa->color = RED;       // Dedeyi Kırmızı yap
                rotateRight(grandpa);       // Dedeyi Sağa devir
            }
        }
        
        // --- SENARYO 2: Babam SAĞ tarafta (Simetrik) ---
        else {
            ResearcherNode* uncle = grandpa->left; // Amca solda

            // DURUM 1: Amca da KIRMIZI
            if (uncle != nullptr && uncle->color == RED) {
                father->color = BLACK;
                uncle->color = BLACK;
                grandpa->color = RED;
                node = grandpa;
            } 
            // DURUM 2: Amca SİYAH veya YOK
            else {
                // Durum 2a: Zikzak (Ben Soldayım) -> Düz çizgiye çevir
                if (node == father->left) {
                    node = father;
                    rotateRight(node);
                    father = node->parent;
                    grandpa = father->parent;
                }

                // Durum 2b: Düz Çizgi (Sağ-Sağ) -> Sola yatır
                father->color = BLACK;
                grandpa->color = RED;
                rotateLeft(grandpa);
            }
        }
    }

    // EN SON KURAL: Kök her zaman SİYAH olmalı.
    if (root){
        root->color = BLACK;
    }
}

// I added it here
void RedBlackTree::transplant(ResearcherNode *u, ResearcherNode *v)
{
    if (u->parent == nullptr)
        root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;

    if (v != nullptr)
        v->parent = u->parent;
}

bool RedBlackTree::removeResearcher(const std::string &fullName)
{
    // TODO:
    // 1) Find node z with data.fullName == fullName.
    // 2) If not found, return false.
    // 3) Perform standard RBT delete:
    //    - Track original color of removed node.
    //    - If a black node is removed, call deleteFixup on the appropriate child.
    // 4) Free node memory.
    // 5) Return true.

    ResearcherNode *z = find(root, fullName);
    if (!z) return false;

    ResearcherNode *y = z;
    ResearcherNode *x = nullptr;
    ResearcherNode *x_parent = nullptr;

    Color y_original_color = y->color;

    // CASE 1: left null
    if (z->left == nullptr)
    {
        x = z->right;
        x_parent = z->parent;
        transplant(z, z->right);
    }
    // CASE 2: right null
    else if (z->right == nullptr)
    {
        x = z->left;
        x_parent = z->parent;
        transplant(z, z->left);
    }
    // CASE 3: two children
    else
    {
        y = minimum(z->right);
        y_original_color = y->color;
        x = y->right;

        if (y->parent == z)
        {
            x_parent = y;
        }
        else
        {
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
            x_parent = y->parent;
        }

        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    delete z;

    if (y_original_color == BLACK)
        deleteFixup(x, x_parent);

    return true;

}

void RedBlackTree::deleteFixup(ResearcherNode *node, ResearcherNode *parent)
{
    while ((node != root) && (node == nullptr || node->color == BLACK))
    {
        if (parent == nullptr) break;

        // node left child
        if (node == parent->left)
        {
            ResearcherNode *w = parent->right;

            if (w == nullptr) {
                node = parent;
                parent = parent->parent;
                continue;
            }

            if (w->color == RED)
            {
                w->color = BLACK;
                parent->color = RED;
                rotateLeft(parent);
                w = parent->right;
                if (!w) continue;
            }

            if ((w->left == nullptr || w->left->color == BLACK) &&
                (w->right == nullptr || w->right->color == BLACK))
            {
                w->color = RED;
                node = parent;
                parent = parent->parent;
            }
            else
            {
                if (w->right == nullptr || w->right->color == BLACK)
                {
                    if (w->left) w->left->color = BLACK;
                    w->color = RED;
                    rotateRight(w);
                    w = parent->right;
                    if (!w) continue;
                }

                w->color = parent->color;
                parent->color = BLACK;
                if (w->right) w->right->color = BLACK;
                rotateLeft(parent);
                node = root;
            }
        }
        else
        {
            ResearcherNode *w = parent->left;

            if (w == nullptr) {
                node = parent;
                parent = parent->parent;
                continue;
            }

            if (w->color == RED)
            {
                w->color = BLACK;
                parent->color = RED;
                rotateRight(parent);
                w = parent->left;
                if (!w) continue;
            }

            if ((w->right == nullptr || w->right->color == BLACK) &&
                (w->left == nullptr || w->left->color == BLACK))
            {
                w->color = RED;
                node = parent;
                parent = parent->parent;
            }
            else
            {
                if (w->left == nullptr || w->left->color == BLACK)
                {
                    if (w->right) w->right->color = BLACK;
                    w->color = RED;
                    rotateLeft(w);
                    w = parent->left;
                    if (!w) continue;
                }

                w->color = parent->color;
                parent->color = BLACK;
                if (w->left) w->left->color = BLACK;
                rotateRight(parent);
                node = root;
            }
        }
    }

    if (node) node->color = BLACK;

}



ResearcherNode *RedBlackTree::minimum(ResearcherNode *node) const
{
    // TODO: Return leftmost node in subtree.
    if(node == nullptr){
        return nullptr;
    }
    while(node->left != nullptr){
        node = node->left;
    }
    return node;
}

void RedBlackTree::rotateLeft(ResearcherNode *x)
{
    // TODO: Standard left rotation.

    ResearcherNode* xR = x->right; 

    x->right = xR->left;
    
    if (xR->left != nullptr) {
        xR->left->parent = x;
    }

    xR->parent = x->parent;

    if (x->parent == nullptr) {
        root = xR; 
    } 
    else if (x == x->parent->left) {
        x->parent->left = xR;
    } 
    else {
        x->parent->right = xR; 
    }

    xR->left = x;
    x->parent = xR;
}

void RedBlackTree::rotateRight(ResearcherNode *y)
{
    // TODO: Standard right rotation.
    ResearcherNode* yL = y->left;

    y->left = yL->right;

    if (yL->right != nullptr) {
        yL->right->parent = y;
    }

    yL->parent = y->parent;

    if (y->parent == nullptr) {
        root = yL; 
    }
    else if (y == y->parent->left) {
        y->parent->left = yL; 
    }
    else {
        y->parent->right = yL; 
    }

    yL->right = y;
    y->parent = yL;}

int RedBlackTree::getResearcherCount() const
{
    return getResearcherCount(root);
}

int RedBlackTree::getResearcherCount(ResearcherNode *node) const
{
    // TODO: return size of subtree.
    if(node == nullptr){
        return 0;
    }

    int size = 0;

    int leftSide = getResearcherCount(node ->left);
    int rightSide = getResearcherCount(node->right);

    size += 1 + leftSide + rightSide;
    return size;

}

int RedBlackTree::getTotalLoad() const
{
    return getTotalLoad(root);
}

int RedBlackTree::getTotalLoad(ResearcherNode *node) const
{
    // TODO: sum of data.numAssigned in subtree.
    if(node == nullptr){
        return 0;
    }

    int total = node->data.numAssigned;

    int leftSide = getTotalLoad(node ->left);
    int rightSide = getTotalLoad(node->right);

    total += leftSide + rightSide;

    return total;
}

void RedBlackTree::traversePreOrderForStats() const
{
    traversePreOrderForStats(root);
}

void RedBlackTree::traversePreOrderForStats(ResearcherNode *node) const
{
    // TODO:
    // Pre-order traversal (node, left, right).
    // Students will decide what exactly to print in PRINT_STATS.

    if (node == nullptr) {
        return;
    }

    std::cout << node->data.fullName << " " 
              << node->data.capacity << " " 
              << node->data.numAssigned << std::endl;
              
    traversePreOrderForStats(node->left);

    traversePreOrderForStats(node->right);
}
