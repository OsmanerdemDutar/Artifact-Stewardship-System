# Artifact Stewardship System

A C++11 artifact management simulator that uses an AVL tree for artifact storage and a Red-Black tree for researcher management, with assignment, return, and consistency tracking between both structures.

---

## Overview

Artifact Stewardship System is a tree-based simulation for managing artifacts and researchers in the same environment.

The project uses:

- An **AVL Tree** to store and balance artifacts
- A **Red-Black Tree** to manage researchers
- A controller layer to keep assignments consistent between both structures

Artifacts can be added, removed, assigned, returned, and queried.  
Researchers can be hired, removed, checked for capacity, and tracked through the system.

---

## Core Features

- AVL-based artifact catalog
- Red-Black Tree based researcher directory
- Artifact assignment and return operations
- Automatic unassignment when a researcher is removed
- Capacity-aware researcher management
- Tree traversal based reporting
- Command-driven file input / stdout output

---

## Main Commands

```text
ADD_ARTIFACT <id> <name> <rarity> <value>
REMOVE_ARTIFACT <id>
HIRE_RESEARCHER <name> <capacity>
FIRE_RESEARCHER <name>
REQUEST <researcherID> <artifactID>
RETURN <researcherID> <artifactID>
RETURN_ALL <researcherID>
RESEARCHER_LOAD <id>
MATCH_RARITY <minRarity>
PRINT_UNASSIGNED
PRINT_STATS
CLEAR
```

---

## System Rules

- Artifacts are stored in an AVL Tree sorted by artifact ID
- Researchers are stored in a Red-Black Tree
- A researcher cannot exceed assignment capacity
- An artifact can only be assigned to one researcher at a time
- Removing a researcher automatically unassigns all their artifacts
- The system keeps both trees synchronized during all updates

---

## Build

```bash
g++ -std=c++11 *.cpp *.h -o HUArchive
```

Optional build methods:

```bash
make all
```

or

```bash
mkdir HUArchive_build
cmake -S . -B HUArchive_build/
make -C HUArchive_build/
```

Run:

```bash
./bin/HUArchive < input.txt
```

---

## Example Input

```text
ADD_ARTIFACT 501 HittiteTablet 8 1400
HIRE_RESEARCHER HaletCilingiroglu 4
REQUEST HaletCilingiroglu 501
RETURN HaletCilingiroglu 501
PRINT_UNASSIGNED
```

---

## File Structure

```text
Artifact.cpp / Artifact.h
Researcher.cpp / Researcher.h
AVLTree.cpp / AVLTree.h
RedBlackTree.cpp / RedBlackTree.h
ArtifactManager.cpp / ArtifactManager.h
main.cpp
```
