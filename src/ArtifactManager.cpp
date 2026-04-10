
#include "ArtifactManager.h"
#include <iostream>
#include <sstream>

ArtifactManager::ArtifactManager()
{
}

ArtifactManager::~ArtifactManager()
{
}

int ArtifactManager::tokenize(const std::string &line, std::string tokens[], int maxTokens) const
{
    std::istringstream iss(line);
    std::string tok;
    int count = 0;
    while (iss >> tok && count < maxTokens)
    {
        tokens[count++] = tok;
    }
    return count;
}

void ArtifactManager::parseAndExecute(const std::string &line)
{
    // TODO: read lines and execuıte each command
    // Print "Error: Unknown command" if command is not known

    std::string tokens[10];
    int count = tokenize(line, tokens, 10);

    // Blank lines... should be ignored.
    if (count == 0) {
        return;
    }

    // COMMAND
    std::string command = tokens[0];

    if (command == "ADD_ARTIFACT") {
        handleAddArtifact(tokens, count);
    }
    else if (command == "REMOVE_ARTIFACT") {
        handleRemoveArtifact(tokens, count);
    }
    else if (command == "HIRE_RESEARCHER") {
        handleHireResearcher(tokens, count);
    }
    else if (command == "FIRE_RESEARCHER") {
        handleFireResearcher(tokens, count);
    }
    else if (command == "REQUEST") {
        handleRequest(tokens, count);
    }
    else if (command == "RETURN") {
        handleReturn(tokens, count);
    }
    else if (command == "RETURN_ALL") {
        handleReturnAll(tokens, count);
    }
    else if (command == "RESEARCHER_LOAD") {
        handleResearcherLoad(tokens, count);
    }
    else if (command == "MATCH_RARITY") {
        handleMatchRarity(tokens, count);
    }
    else if (command == "PRINT_UNASSIGNED") {
        handlePrintUnassigned(tokens, count);
    }
    else if (command == "PRINT_STATS") {
        handlePrintStats(tokens, count);
    }
    else if (command == "CLEAR") {
        handleClear(tokens, count);
    }
    else {
        std::cout << "Error: Unknown command '" << command << "'." << std::endl;
    }
}

// =================== COMMAND HANDLERS ===================

void ArtifactManager::handleAddArtifact(const std::string tokens[], int count)
{
    // Expected: ADD_ARTIFACT <id> <name> <rarity> <value>
    // TODO:
    // 1) Check parameter count.
    // 2) Convert <id>, <rarity>, <value> to integers.
    // 3) Create Artifact and attempt to insert into AVL tree.
    // 4) On success: print "Artifact <id> added."
    // 5) On duplicate: print appropriate error (as in the PDF).

//----------------------------------------------------------------------------------------    

    if (count != 5) {
        std::cout << "Error: Invalid parameter count for ADD_ARTIFACT" << std::endl;
        return;
    }

    int id = std::stoi(tokens[1]);
    std::string name = tokens[2];
    int rarity = std::stoi(tokens[3]);
    int value = std::stoi(tokens[4]);

    // Artifact to Add in AVL
    Artifact newArtifact(id, name, rarity, value);

    // Adding Artifact in AVL
    if (artifactTree.insertArtifact(newArtifact)) {
        std::cout << "Artifact " << id << " added." << std::endl;
    } else {
        std::cout << "Error: Artifact " << id << " already exists." << std::endl;
    }
}

void ArtifactManager::handleRemoveArtifact(const std::string tokens[], int count)
{
    // Expected: REMOVE_ARTIFACT <id>
    // TODO:
    // 1) Parse id.
    // 2) Find artifact in AVL; if not found, print error.
    // 3) If artifact is assigned , find researcher and
    //    remove artifact from their list.
    // 4) Remove artifact from AVL; print success or error message.

//----------------------------------------------------------------------------------------    
    if (count != 2) {
        std::cout << "Error: Invalid parameter count for REMOVE_ARTIFACT" << std::endl;
        return;
    }

    int id = std::stoi(tokens[1]);

    ArtifactNode* artNode = artifactTree.findArtifact(id);
    
    if (artNode == nullptr) {
        std::cout << "Error: Artifact " << id << " not found." << std::endl;
        return;
    }

    // If the work has been assigned to someone, 
    //  first remove it from the researcher's list.
    if (!artNode->data.assignedToName.empty()) {
        std::string resName = artNode->data.assignedToName;
        ResearcherNode* resNode = researcherTree.findResearcher(resName);
        
        if (resNode != nullptr) {
            resNode->data.removeArtifact(id);
        }
    }

    artifactTree.removeArtifact(id);
    std::cout << "Artifact " << id << " removed." << std::endl;
}

void ArtifactManager::handleHireResearcher(const std::string tokens[], int count)
{
    // Expected: HIRE_RESEARCHER <name> <capacity>
    // TODO:
    // 1) Parse name and capacity.
    // 2) Create Researcher and insert into RedBlackTree.
    // 3) On success: "Researcher <name> hired."
    // 4) On duplicate: error message.

//----------------------------------------------------------------------------------------
    
    if (count != 3) {
        std::cout << "Error: Invalid parameter count for HIRE_RESEARCHER" << std::endl;
        return;
    }

    std::string name = tokens[1];
    int capacity = std::stoi(tokens[2]);

    Researcher newResearcher(name, capacity);

    if (researcherTree.insertResearcher(newResearcher)) {
        std::cout << "Researcher " << name << " hired." << std::endl;
    } else {
        std::cout << "Error: Researcher " << name << " already exists." << std::endl;
    }
}

void ArtifactManager::handleFireResearcher(const std::string tokens[], int count)
{
    // Expected: FIRE_RESEARCHER <name>
    // TODO:
    // 1) Find researcher by name. If not found, print error.
    // 2) For each artifact ID in their assignment list:
    //      - clear assignedToName in AVL.
    // 3) Remove researcher from RBT.
    // 4) Print success message.
//----------------------------------------------------------------------------------------

    if (count != 2) {
        std::cout << "Error: Invalid parameter count for FIRE_RESEARCHER" << std::endl;
        return;
    }

    std::string name = tokens[1];

    ResearcherNode* foundedResearcher = researcherTree.findResearcher(name);

    if (foundedResearcher == nullptr) {
        std::cout << "Error: Researcher " << name << " not found." << std::endl;
        return;
    }

    // NOT:We are not deleting the works, we are only clearing the ‘assignedToName’ field. 
    for(int i = 0; i < foundedResearcher->data.numAssigned; i++){
        int artID = foundedResearcher->data.assignedArtifacts[i];
        
        ArtifactNode* artNode = artifactTree.findArtifact(artID);
        
        if (artNode != nullptr) {
            // Remove the lien on the property (make empty string)
            artNode->data.assignedToName = ""; 
        }
    }

    researcherTree.removeResearcher(name);
    std::cout << "Researcher " << name << " fired." << std::endl;
}

void ArtifactManager::handleRequest(const std::string tokens[], int count)
{
    // Expected: REQUEST <researcherName> <artifactID>
    // TODO:
    // 1) Find researcher by name; error if missing.
    // 2) Find artifact by ID; error if missing.
    // 3) Check artifact is unassigned; error if already assigned.
    // 4) Check researcher capacity; error if at full capacity.
    // 5) On success: add artifact to researcher list AND set assignedToName in AVL.
    //    Print "Artifact <id> assigned to <name>."

//----------------------------------------------------------------------------------------

    if (count != 3) {
        std::cout << "Error: Invalid parameter count for REQUEST" << std::endl;
        return;
    }

    std::string resName = tokens[1];
    int artID = std::stoi(tokens[2]);

    // find the researcher
    ResearcherNode* resNode = researcherTree.findResearcher(resName);
    if (resNode == nullptr) {
        std::cout << "Error: Researcher " << resName << " not found." << std::endl;
        return;
    }

    // find the artifact
    ArtifactNode* artNode = artifactTree.findArtifact(artID);
    if (artNode == nullptr) {
        std::cout << "Error: Artifact " << artID << " not found." << std::endl;
        return;
    }

    // Check if the work has already been assigned 
    if (!artNode->data.assignedToName.empty()) {
        std::cout << "Error: Artifact " << artID << " is already assigned to " 
                  << artNode->data.assignedToName << "." << std::endl;
        return;
    }

    // Is the researcher's capacity full? 
    if (resNode->data.numAssigned >= resNode->data.capacity) {
        std::cout << "Error: Researcher " << resName << " is at full capacity." << std::endl;
        return;
    }

    
    resNode->data.addArtifact(artID);

    artNode->data.assignedToName = resName;
    
    artNode->data.updateValueBasedOnUsage();

    std::cout << "Artifact " << artID << " assigned to " << resName << "." << std::endl;
}

void ArtifactManager::handleReturn(const std::string tokens[], int count)
{
    // Expected: RETURN <researcherName> <artifactID>
    // TODO:
    // 1) Validate existence of researcher and artifact.
    // 2) Check that artifact.assignedToName == researcherName.
    // 3) If not, print error.
    // 4) Otherwise, remove artifact from researcher list, clear assignedToName in AVL.
    //    Print "Artifact <id> returned by <name>."

//----------------------------------------------------------------------------------------
    if (count != 3) {
        std::cout << "Error: Invalid parameter count for RETURN" << std::endl;
        return;
    }

    std::string resName = tokens[1];
    int artID = std::stoi(tokens[2]);

    ResearcherNode* resNode = researcherTree.findResearcher(resName);
    ArtifactNode* artNode = artifactTree.findArtifact(artID);

    // Validate existence
    if (resNode == nullptr) {
        std::cout << "Error: Researcher " << resName << " not found." << std::endl;
        return;
    }
    if (artNode == nullptr) {
        std::cout << "Error: Artifact " << artID << " not found." << std::endl;
        return;
    }

    // Check assignment matches
    if (artNode->data.assignedToName != resName) {
        std::cout << "Error: Artifact " << artID << " is not assigned to " << resName << "." << std::endl;
        return;
    }
    
    resNode->data.removeArtifact(artID);

    artNode->data.assignedToName = "";
    
    std::cout << "Artifact " << artID << " returned by " << resName << "." << std::endl;
}

void ArtifactManager::handleReturnAll(const std::string tokens[], int count)
{
    // Expected: RETURN_ALL <researcherName>
    // TODO:
    // 1) Find researcher; error if missing.
    // 2) For each artifact they supervise, clear assignedToName in AVL.
    // 3) Clear researcher's assignment list (removeAllArtifacts()).
    // 4) Print appropriate confirmation message.

//----------------------------------------------------------------------------------------

    if (count != 2) {
        std::cout << "Error: Invalid parameter count for RETURN_ALL" << std::endl;
        return;
    }

    std::string resName = tokens[1];

    ResearcherNode* resNode = researcherTree.findResearcher(resName);

    if (resNode == nullptr) {
        std::cout << "Error: Researcher " << resName << " not found." << std::endl;
        return;
    }

    for (int i = 0; i < resNode->data.numAssigned; ++i) {
        int artID = resNode->data.assignedArtifacts[i];
        
        ArtifactNode* artNode = artifactTree.findArtifact(artID);
        
        if (artNode != nullptr) {
            artNode->data.assignedToName = "";
        }
    }
    std::cout<<"All artifacts returned by "<< resName <<"." << std::endl;
    resNode->data.removeAllArtifacts();                                 
}

void ArtifactManager::handleResearcherLoad(const std::string tokens[], int count)
{
    // Expected: RESEARCHER_LOAD <name>
    // TODO:
    // 1) Find researcher by name.
    // 2) If not found, print error.
    // 3) Otherwise, print number of supervised artifacts in required format.
//------------------------------------------------------------------------------------
    if (count != 2) {
        std::cout << "Error: Invalid parameter count for RESEARCHER_LOAD" << std::endl;
        return;
    }

    std::string name = tokens[1];

    ResearcherNode* resNode = researcherTree.findResearcher(name);

    if (resNode == nullptr) {
        std::cout << "Error: Researcher " << name << " not found." << std::endl;
        return;
    }

    std::cout << "Researcher " << name << " is supervising " 
              << resNode->data.numAssigned << " artifacts." << std::endl;
}

void ArtifactManager::handleMatchRarity(const std::string tokens[], int count)
{
    // Expected: MATCH_RARITY <minRarity>
    // TODO:
    // Traverse AVL tree and print all artifacts with rarity >= minRarity.
    // You may choose any reasonable order (probably inorder) unless specified otherwise
    // in your PDF. Artifacts may be assigned or unassigned; print as required.



    //------------------------------------------------------------------------------------    
    if (count != 2) {
        std::cout << "Error: Invalid parameter count for MATCH_RARITY" << std::endl;
        return;
    }

    int minRarity = std::stoi(tokens[1]);

    std::cout << "=== MATCH_RARITY "<<minRarity<<" ===" << std::endl;
    artifactTree.printArtifactsWithRarity(minRarity);
}

void ArtifactManager::handlePrintUnassigned(const std::string tokens[], int count)
{
    // Expected: PRINT_UNASSIGNED
    // TODO:
    // Print a header if needed, then call artifactTree.printUnassigned().

//------------------------------------------------------------------------------------
    if (count != 1) {
        std::cout << "Error: Invalid parameter count for PRINT_UNASSIGNED" << std::endl;
        return;
    }

    std::cout << "Unassigned artifacts:" << std::endl;
    artifactTree.printUnassigned();
}

void ArtifactManager::handlePrintStats(const std::string tokens[], int count)
{
    // Expected: PRINT_STATS
    // TODO:
    // 1) Compute:
    //    - totalArtifacts (artifactTree.getArtifactCount())
    //    - totalResearchers (researcherTree.getResearcherCount())
    //    - average artifact rarity (floor of totalRarity / totalArtifacts)
    //    - average researcher load (floor of totalLoad / totalResearchers)
    // 2) Print summary lines exactly as in the spec.
    // 3) Then:
    //    - Print researchers using preorder traversal:
    //      researcherTree.traversePreOrderForStats()
    //    - Print artifacts using postorder traversal:
    //      artifactTree.traversePostOrderForStats()
    //    (Exact formatting defined in your PDF.)

//------------------------------------------------------------------------------------
    if (count != 1) {
        std::cout << "Error: Invalid parameter count for PRINT_STATS" << std::endl;
        return;
    }

    // calculate the statistic
    int totalArtifacts = artifactTree.getArtifactCount();
    int totalResearchers = researcherTree.getResearcherCount();
    int totalRarity = artifactTree.getTotalRarity();
    int totalLoad = researcherTree.getTotalLoad(); // numAssigned toplamı

    // avarage
    int avgRarity = (totalArtifacts == 0) ? 0 : (totalRarity / totalArtifacts);
    int avgLoad = (totalResearchers == 0) ? 0 : (totalLoad / totalResearchers);

    // tables
    std::cout<<"=== SYSTEM STATISTICS ==="<< std::endl;
    std::cout << "Artifacts: " << totalArtifacts << std::endl;
    std::cout << "Researchers: " << totalResearchers << std::endl;
    std::cout << "Average rarity: " << avgRarity << std::endl;
    std::cout << "Average load: " << avgLoad << std::endl;

    // lists    
    std::cout<< "Researchers:"<<std::endl;
    researcherTree.traversePreOrderForStats();
    
    std::cout<< "Artifacts:"<<std::endl;
    artifactTree.traversePostOrderForStats();
}

void ArtifactManager::handleClear(const std::string tokens[], int count)
{
    // Expected: CLEAR
    // TODO:
    // Clear both trees and print confirmation message.
    artifactTree.clear();
    researcherTree.clear();
    // e.g. "All data cleared."
    std::cout << "All data cleared." << std::endl;
}
