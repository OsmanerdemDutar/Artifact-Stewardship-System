#include "AVLTree.h"

AVLTree::AVLTree()
    : root(nullptr)
{
}

AVLTree::~AVLTree()
{
    clear();
}

void AVLTree::clear()
{
    clear(root);
    root = nullptr;
}

void AVLTree::clear(ArtifactNode *node)
{
    //TODO
    if(node == nullptr){
        return;
    }
    
    // postorder traversal 
    clear(node->left);
    clear(node->right);

    delete node;
    node = nullptr;
}

int AVLTree::height(ArtifactNode *node) const
{
    return node ? node->height : -1;
}

int AVLTree::getBalance(ArtifactNode *node) const
{
    if (!node) return 0;
    return height(node->left) - height(node->right);
}

ArtifactNode *AVLTree::rotateLeft(ArtifactNode *x)
{
    // TODO: Standard AVL left rotation.
    //return new root

    ArtifactNode* xR = x->right;
    x ->right = xR ->left;
    xR ->left = x;

    // The height is updated starting from the root below. 
    x->height = 1 + std::max(height(x->left), height(x->right));

    xR->height = 1 + std::max(height(xR->left), height(xR->right));
    return xR;
}

ArtifactNode *AVLTree::rotateRight(ArtifactNode *y)
{
    // TODO: Standard AVL right rotation (mirror of rotateLeft).

    ArtifactNode* yL = y-> left;
    y->left = yL -> right;
    yL->right = y;

    y->height = 1 + std::max(height(y->left), height(y->right));

    yL->height = 1 + std::max(height(yL->left), height(yL->right));
    return yL;
}

ArtifactNode *AVLTree::findMinNode(ArtifactNode *node) const
{
    // TODO: Return leftmost node in this subtree.
    if(node == nullptr){
        return nullptr;
    }
    ArtifactNode* current = node;
    while(current ->left != nullptr){
        current = current->left;
    }
    
    return current;
}

ArtifactNode *AVLTree::insert(ArtifactNode *node, const Artifact &artifact, bool &inserted)
{
    // TODO:
    // 1) Standard BST insert by artifactID.
    // 2) If duplicate ID, set inserted = false and return node unchanged.
    // 3) On actual insertion, update node->height.
    // 4) Compute balance and apply AVL rotations if needed.
    // 5) Return the (possibly new) root of this subtree.


    // insertion side
    if(node == nullptr){
        // we found a node to add
        inserted = true;
        ArtifactNode* a = new ArtifactNode(artifact);

        return a;
    }

    if(node->data.artifactID == artifact.artifactID){
        inserted = false;
        return node;
    }
    else if(node->data.artifactID > artifact.artifactID){
        node->left = insert(node ->left, artifact, inserted);
    }
    else{
        node->right = insert(node ->right, artifact, inserted);
    }

    // change the height
    int maxSubHeight =(height(node->right) > height(node->left)) ? height(node->right) : height(node->left);
    node->height = 1 + maxSubHeight;

    // control the balance with new heights

    int balance = getBalance(node);

    // left is heavier 
    if(balance > 1){
        // adding the left left , one right rotate is enough
        if(artifact.artifactID < node->left->data.artifactID){
            return rotateRight(node);
        }

        // added the left right , we need double rotate left - right
        else{
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
    }
    
    // right is heavier
    else if(balance < -1){
        // added the right right , one left rotate is enough
        if(artifact.artifactID > node->right->data.artifactID){
            return rotateLeft(node);
        }
        
        // added the right left , we need double rotate right - left
        else{
            node -> right = rotateRight(node->right);
            return rotateLeft(node);
        }

    }
    return node;
}

ArtifactNode *AVLTree::remove(ArtifactNode *node, int artifactID, bool &removed)
{
    // TODO:
    // 1) Standard BST deletion by artifactID.
    // 2) If node not found, leave removed = false.
    // 3) Upon deletion, update heights and rebalance with rotations.
    // 4) Return (possibly new) root of this subtree.


    // find the node
    if(node == nullptr){
        removed = false;
        return nullptr;
    }

    if(node->data.artifactID > artifactID){
        node->left = remove(node->left,artifactID,removed);
    }
    else if(node->data.artifactID < artifactID){
        node->right = remove(node->right,artifactID, removed);
    }

    else{
        // 1) no child
        if(node->right == nullptr && node->left == nullptr){
            delete node;
            removed = true;
            return nullptr;
        }

        // 2) one child
        else if(node->right == nullptr && node->left != nullptr){
            ArtifactNode* temp = node->left;
            delete node;
            node = nullptr;
            node = temp;

            removed = true;
        }
        else if(node->right != nullptr && node->left == nullptr){
            ArtifactNode* temp = node->right;
            delete node;
            node = nullptr;
            node = temp;

            removed = true;
        }

        // 3) two child
        else if(node->right != nullptr && node->left != nullptr){
            
            // find the min element of the right subtree
            ArtifactNode* temp = node->right;
            while(temp->left != nullptr){
                temp = temp->left;
            }
            
            node->data = temp->data;
            bool removeTheMinElement = false;
            node->right = remove(node->right,temp->data.artifactID,removeTheMinElement);

            removed = true;
        }
    }

    if(node == nullptr){
        return node;
    }
    
    // adjust the height
    int maxSubHeight =(height(node->right) > height(node->left)) ? height(node->right) : height(node->left);
    node->height = 1 + maxSubHeight;

    int balance;
    balance = getBalance(node);

    //left is heavier
    if(balance > 1){
        // because of left's left
        // single righ rotate is enough
        if (getBalance(node->left) >= 0) {
             return rotateRight(node);
        }

        // because of left's right
        // firstly its left node leftRotate then node right rotate
        else {
             node->left = rotateLeft(node->left);
             return rotateRight(node);
        }
    }

    // right is heavier
    else if(balance < -1){
        // because of right's right
        // single left rotate is enough
        if(getBalance(node->right) <= 0){
            return rotateLeft(node);
        }
        
        // because right's left
        // firstly its right node RighRotate then node left rotate
        else{
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
    }

    return node;
}

bool AVLTree::insertArtifact(const Artifact &artifact)
{
    bool inserted = false;   //tracks whether operation is succesful
    root = insert(root, artifact, inserted);
    return inserted;
}

bool AVLTree::removeArtifact(int artifactID)
{
    bool removed = false;   //tracks whether operation is succesful
    root = remove(root, artifactID, removed);
    return removed;
}

ArtifactNode *AVLTree::findArtifact(int artifactID) const
{
    return find(root, artifactID);
}

ArtifactNode *AVLTree::find(ArtifactNode *node, int artifactID) const
{
    //TODO
    if(node == nullptr){
        return nullptr;
    }

    if(node->data.artifactID == artifactID){
        return node;
    }

    if(artifactID > node->data.artifactID){
        return find(node -> right, artifactID);
    }
    else{
        return find(node->left, artifactID);
    }

    return nullptr;
}

void AVLTree::setAssignedTo(int artifactID, const std::string &researcherName)
{

    //assign artifact to researcher
    ArtifactNode* a = findArtifact(artifactID);

    if(a != nullptr){
        a->data.assignedToName = researcherName;
    }
    else{
        // a is nullptr
    }
}

void AVLTree::clearAssignedTo(int artifactID)
{
    //TODO
    ArtifactNode* a = findArtifact(artifactID);

    if(a != nullptr){
        a->data.assignedToName = "" ;
    }
} 

void AVLTree::printUnassigned() const
{
    printUnassigned(root);
}

void AVLTree::printUnassigned(ArtifactNode *node) const
{
    // TODO:
    // Inorder traversal.
    // For each node with data.assignedToName == "", print in required format, e.g.:
    // <id> <name> <rarity> <value>

    if(node == nullptr){
        return;
    }

    printUnassigned(node->left);

    if (node->data.assignedToName.empty()) { 
        std::cout << node->data.artifactID << " "
                  << node->data.name << " "
                  << node->data.rarityLevel << " "
                  << node->data.researchValue << std::endl;
    }

    printUnassigned(node->right);
}

int AVLTree::getArtifactCount() const
{
    return getArtifactCount(root);
}

int AVLTree::getArtifactCount(ArtifactNode *node) const
{
    // TODO: return size of subtree.
    if(node == nullptr){
        return 0;
    }

    int result = 0;

    result += getArtifactCount(node->left);
    result += getArtifactCount(node->right);
    
    return 1 + result;
}

int AVLTree::getTotalRarity() const
{
    return getTotalRarity(root);
}

int AVLTree::getTotalRarity(ArtifactNode *node) const
{
    // TODO: sum of rarityLevel over subtree.
    if(node == nullptr){
        return 0;
    }

    int totalRarityLevel = node->data.rarityLevel;

    totalRarityLevel += getTotalRarity(node->left);
    totalRarityLevel += getTotalRarity(node->right);
    
    return totalRarityLevel;
}

// ----------------- I ADDED -----------------------

void AVLTree::traversePostOrderForStats() const
{
    traversePostOrderForStats(root);
}

void AVLTree::traversePostOrderForStats(ArtifactNode *node) const
{
    // TODO:
    // Post-order traversal (left, right, node).
    // Students will decide what exactly to print in PRINT_STATS.

    if (node == nullptr) {
        return;
    }

    traversePostOrderForStats(node->left);

    traversePostOrderForStats(node->right);

    // Standart format: ID Name Rarity Value
    std::cout << node->data.artifactID << " "
              << node->data.name << " "
              << node->data.rarityLevel << " "
              << node->data.researchValue;

    if (node->data.assignedToName.empty()) {
        std::cout << " UNASSIGNED" << std::endl;
    } else {
        std::cout << " " << node->data.assignedToName << std::endl;
    }
}

void AVLTree::printArtifactsWithRarity(int minRarity) const {
    printArtifactsWithRarity(root, minRarity);
}

void AVLTree::printArtifactsWithRarity(ArtifactNode* node, int minRarity) const {

    if (node == nullptr) {
        return;
    }

    printArtifactsWithRarity(node->left, minRarity);

    if (node->data.rarityLevel >= minRarity) {
        // Format: ID Name Rarity Value 
        std::cout << node->data.artifactID << " " 
                  << node->data.name << " " 
                  << node->data.rarityLevel << " " 
                  << node->data.researchValue << " ";

        if (!node->data.assignedToName.empty()) {
            std::cout << "ASSIGNED:" << node->data.assignedToName;
        }
        std::cout << std::endl;
    }

    printArtifactsWithRarity(node->right, minRarity);
}





